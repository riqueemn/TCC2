import 'dart:convert';
import 'dart:ffi';

import 'package:flutter/services.dart';

import 'widgets/action_button.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:flutter/material.dart';
import 'package:permission_handler/permission_handler.dart';
import 'package:syncfusion_flutter_sliders/sliders.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_spinbox/material.dart'; // or flutter_spinbox.dart for both

class MainPage extends StatefulWidget {
  const MainPage({super.key});

  @override
  State<MainPage> createState() => _MainPageState();
}

class _MainPageState extends State<MainPage> with WidgetsBindingObserver {
  double _currentValue = 3;
  double _firstMarkerValue = 20;
  double _secondMarkerValue = 80;
  double _value = 0.5;
  bool _currentMode = true;
  bool _onPressedGenericButton = false;
  bool _onPressedEmergencyButton = false;
  bool _onPressedSendButton = false;
  bool _onPressedStartButton = false;
  bool _onPressedStopButton = false;
  bool _onPressedReversaoButton = false;

  final _bluetooth = FlutterBluetoothSerial.instance;
  bool _bluetoothState = false;
  bool _isConnecting = false;
  BluetoothConnection? _connection;
  List<BluetoothDevice> _devices = [];
  BluetoothDevice? _deviceConnected;
  int times = 0;

  @override
  void initState() {
    WidgetsBinding.instance!.addObserver(this);
    SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp]);
    
    super.initState();

    _requestPermission();

    _bluetooth.state.then((state) {
      setState(() => _bluetoothState = state.isEnabled);
    });

    _bluetooth.onStateChanged().listen((state) {
      switch (state) {
        case BluetoothState.STATE_OFF:
          setState(() => _bluetoothState = false);
          break;
        case BluetoothState.STATE_ON:
          setState(() => _bluetoothState = true);
          break;
        // case BluetoothState.STATE_TURNING_OFF:
        //   break;
        // case BluetoothState.STATE_TURNING_ON:
        //   break;
      }
    });
  }

  void dispose() {
    WidgetsBinding.instance!.removeObserver(this);
    super.dispose();
  }

  void didChangeAppLifecycleState(AppLifecycleState state) {
    if (state != AppLifecycleState.resumed) {
      _sendData("EMERGENCY_STOP");

      _connection?.finish();
      _bluetooth.requestDisable();
      setState(() => _deviceConnected = null);
    }
  }

  void _handleFirstPointerValueChanged(double value) {
    if (value < _secondMarkerValue) {
      setState(() {
        _firstMarkerValue = value;
      });
    }
  }

  void _handleFirstPointerValueChanging(ValueChangingArgs args) {
    if (args.value < _secondMarkerValue) {
      _firstMarkerValue = args.value;
    } else {
      args.cancel = true;
    }
  }

  void _handleSecondPointerValueChanging(ValueChangingArgs args) {
    if (_firstMarkerValue < args.value) {
      _secondMarkerValue = args.value;
    } else {
      args.cancel = true;
    }
  }

  void _handleSecondPointerValueChanged(double value) {
    if (_firstMarkerValue < value) {
      setState(() {
        _secondMarkerValue = value;
      });
    }
  }

  void _getDevices() async {
    var res = await _bluetooth.getBondedDevices();
    setState(() => _devices = res);
  }

  void _receiveData() {
    _connection?.input?.listen((event) {
      if (String.fromCharCodes(event) == "p") {
        setState(() => times = times + 1);
      }
    });
  }

  void _sendData(String data) {
    if (_connection?.isConnected ?? false) {
      _connection?.output.add(ascii.encode(data));
    }
  }

  void _requestPermission() async {
    await Permission.location.request();
    await Permission.bluetooth.request();
    await Permission.bluetoothScan.request();
    await Permission.bluetoothConnect.request();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        toolbarHeight: 20,
        //centerTitle: true,
        //title: const Text('Teste'),
      ),
      body: Column(
        children: [
          //_controlBT(),
          _infoDevice(),
          Expanded(child: _listDevices()),
          _buttons(),
        ],
      ),
    );
  }

  Widget _controlBT() {
    return SwitchListTile(
      value: _bluetoothState,
      onChanged: (bool value) async {
        if (value) {
          await _bluetooth.requestEnable();
        } else {
          await _bluetooth.requestDisable();
        }
      },
      tileColor: Colors.black26,
      title: Text(
        _bluetoothState ? "Bluetooth ligado" : "Bluetooth desligado",
      ),
    );
  }

  Widget _infoDevice() {
    return ListTile(
      tileColor: Colors.black12,
      title: Text("Conectado a: ${_deviceConnected?.name ?? "ninguém"}"),
      trailing: _connection?.isConnected ?? false
          ? TextButton(
              onPressed: () async {
                await _connection?.finish();
                setState(() => _deviceConnected = null);
              },
              child: const Text("Desconectar"),
            )
          : TextButton(
              onPressed: _getDevices,
              child: const Text("Ver dispositivos"),
            ),
    );
  }

  Widget _listDevices() {
    return _isConnecting
        ? const Center(child: CircularProgressIndicator())
        : SingleChildScrollView(
            child: Container(
              color: Colors.grey.shade100,
              child: Column(
                children: [
                  ...[
                    for (final device in _devices)
                      ListTile(
                        title: Text(device.name ?? device.address),
                        trailing: TextButton(
                          child: const Text('conectar'),
                          onPressed: () async {
                            setState(() => _isConnecting = true);

                            _connection = await BluetoothConnection.toAddress(
                                device.address);
                            _deviceConnected = device;
                            _devices = [];
                            _isConnecting = false;

                            _receiveData();

                            setState(() {});
                          },
                        ),
                      )
                  ]
                ],
              ),
            ),
          );
  }

  void InitState() {
    _onPressedSendButton = false;
    _onPressedReversaoButton = false;
    _onPressedStartButton = false;
    _onPressedStopButton = false;
    _onPressedEmergencyButton = false;
  }

  Widget Send() {
    return Expanded(
      child: FloatingActionButton(
          backgroundColor: Colors.blue,
          splashColor: Colors.black,
          onPressed: () {
            setState(() {
              InitState();
              _onPressedSendButton = !_onPressedSendButton;
            });

            _sendData(_currentValue.toString());
          },
          child: Text("SEND"),
          shape: _onPressedSendButton
              ? RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(15),
                  side: BorderSide(color: Colors.white, width: 2))
              : null),
    );
  }

  Widget GenericButton(String s, Color c, bool state) {
    return Expanded(
      child: FloatingActionButton(
          backgroundColor: c,
          splashColor: Colors.black,
          onPressed: () {
            setState(() {
              state = !state;
            });
            _sendData(s);
          },
          child: Text(s),
          shape: state
              ? Border.all(width: 2.0, color: const Color(0xFFFFFFFF))
              : null),
    );
  }

  Widget Start(String s, Color c) {
    return Expanded(
      child: FloatingActionButton(
          backgroundColor: c,
          splashColor: Colors.black,
          onPressed: () {
            setState(() {
              InitState();
              _onPressedStartButton = !_onPressedStartButton;
            });
            _sendData(s);
          },
          child: Text(s),
          shape: _onPressedStartButton
              ? RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(15),
                  side: BorderSide(color: Colors.white, width: 2))
              : null),
    );
  }

  Widget Stop(String s, Color c) {
    return Expanded(
      child: FloatingActionButton(
          backgroundColor: c,
          splashColor: Colors.black,
          onPressed: () {
            setState(() {
              InitState();
              _onPressedStopButton = !_onPressedStopButton;
            });
            _sendData(s);
          },
          child: Text(s),
          shape: _onPressedStopButton
              ? RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(15),
                  side: BorderSide(color: Colors.white, width: 2))
              : null),
    );
  }

  Widget Reversao(String s, Color c) {
    return Expanded(
      child: FloatingActionButton(
          backgroundColor: c,
          splashColor: Colors.black,
          onPressed: () {
            setState(() {
              InitState();
              _onPressedReversaoButton = !_onPressedReversaoButton;
            });
            _sendData(s);
          },
          child: Text(s),
          shape: _onPressedReversaoButton
              ? RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(15),
                  side: BorderSide(color: Colors.white, width: 2))
              : null),
    );
  }

  Widget EmergencyButton() {
    return Expanded(
      child: FloatingActionButton(
          backgroundColor: Colors.red,
          splashColor: Colors.black,
          onPressed: () {
            _sendData("EMERGENCY_STOP");
            setState(() {
              InitState();
              _currentValue = 3;
              _onPressedEmergencyButton = !_onPressedEmergencyButton;
            });
          },
          child: Text("EMERGÊNCIA"),
          shape: _onPressedEmergencyButton
              ? RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(15),
                  side: BorderSide(color: Colors.white, width: 2))
              : null),
    );
  }

  Widget AlterarFrequencia() {
    return Container(
        width: 180,
        child: SpinBox(
          min: 0.0,
          max: 15.0,
          step: 0.1,
          decimals: 1,
          value: _currentValue,
          onChanged: (value) {
            setState(() {
              _currentValue = value;
            });
          },
        ));
  }

  Widget switchMode() {
    return Expanded(
      child: Switch.adaptive(
        applyCupertinoTheme: false,
        value: _currentMode,
        onChanged: (bool value) {
          setState(() {
            _currentMode = value;
          });

          if (_currentMode) {
            _sendData("MODO_APP");
          } else {
            _sendData("MODO_QUADRO");
          }
        },
      ),
    );
  }

  Widget AnguloRotacao() {
    return SfRadialGauge(
        title: GaugeTitle(
            text: 'Ângulo',
            textStyle: TextStyle(
                fontSize: 10,
                fontWeight: FontWeight.bold,
                color: Colors.black,
                fontFamily: 'Arial'),
            alignment: GaugeAlignment.near),
        axes: <RadialAxis>[
          RadialAxis(
            minimum: 0,
            maximum: 360,
            interval: 10,
            minorTicksPerInterval: 10,
            startAngle: 250,
            endAngle: 250,
            pointers: <GaugePointer>[
              MarkerPointer(
                value: _firstMarkerValue,
                markerHeight: 20,
                markerWidth: 20,
                enableDragging: true,
                markerType: MarkerType.circle,
                onValueChanged: _handleFirstPointerValueChanged,
                onValueChanging: _handleFirstPointerValueChanging,
              ),
              MarkerPointer(
                value: _secondMarkerValue,
                markerHeight: 20,
                markerWidth: 20,
                enableDragging: true,
                markerType: MarkerType.circle,
                onValueChanged: _handleSecondPointerValueChanged,
                onValueChanging: _handleSecondPointerValueChanging,
              )
            ],
            ranges: <GaugeRange>[
              GaugeRange(
                  startValue: _firstMarkerValue,
                  endValue: _secondMarkerValue,
                  sizeUnit: GaugeSizeUnit.factor,
                  startWidth: 0.06,
                  endWidth: 0.06)
            ],
          )
        ]);
  }

  Widget _buttons() {
    return Container(
      padding: const EdgeInsets.symmetric(vertical: 10.0, horizontal: 8.0),
      color: Colors.black12,
      child: Column(
        children: [
          const SizedBox(height: 10.0),
          Row(
            children: [
              Send(),
              SizedBox(width: 10),
              Start("START", Colors.green),
              SizedBox(width: 10),
              Reversao("REVERSAO", Colors.grey)
            ],
          ),
          const SizedBox(height: 10.0),
          Row(children: [
            Stop("STOP", Colors.orange),
            SizedBox(width: 10),
            EmergencyButton()
          ]),
          const SizedBox(height: 10.0),
          Row(
            children: [
              Center(
                child: AlterarFrequencia(),
              ),
              SizedBox(width: 10),
              Text("Quadro"),
              switchMode(),
              Text("App")
            ],
          ),
          Container(height: 260, width: 260, child: AnguloRotacao()),
        ],
      ),
    );
  }
}

import 'dart:convert';
import 'dart:ffi';

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

class _MainPageState extends State<MainPage> {
  double _currentValue = 4;
  double _firstMarkerValue = 20;
  double _secondMarkerValue = 80;
  double _value = 0.5;

  final _bluetooth = FlutterBluetoothSerial.instance;
  bool _bluetoothState = false;
  bool _isConnecting = false;
  BluetoothConnection? _connection;
  List<BluetoothDevice> _devices = [];
  BluetoothDevice? _deviceConnected;
  int times = 0;

  /// Update the first thumb value to the range.
  void _handleFirstPointerValueChanged(double value) {
    if (value < _secondMarkerValue) {
      setState(() {
        _firstMarkerValue = value;
      });
    }
  }

  /// Cancel the dragging when pointer value reaching the axis end/start value, greater/less than another.
  /// pointer value.
  void _handleFirstPointerValueChanging(ValueChangingArgs args) {
    if (args.value < _secondMarkerValue) {
      _firstMarkerValue = args.value;
    } else {
      args.cancel = true;
    }
  }

  /// Cancel the dragging when pointer value reaching the axis end/start value, greater/less than another.
  /// pointer value.
  void _handleSecondPointerValueChanging(ValueChangingArgs args) {
    if (_firstMarkerValue < args.value) {
      _secondMarkerValue = args.value;
    } else {
      args.cancel = true;
    }
  }

  /// Update the second thumb value to the range.
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
  void initState() {
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

  Widget button1(){
    return Container(
    width: 250,
    child: Slider(
      value: _currentValue,
      min: 0,
      max: 10,
      divisions: 100,
      label: _currentValue.toString(),
      activeColor: Colors.blue,
      thumbColor: Colors.red,
      onChanged: (value) {
        setState(() {
          _currentValue = value;
        });
      },
    ));
  }

  Widget button2(){
    return
    Container(
        width: 20,
        child: TextField(
            obscureText: true,
            decoration: InputDecoration(
              border: OutlineInputBorder(),
              labelText: 'Password',
            )));
}

  Widget button3(){
    return
    Container(
        width: 300,
        child: SpinBox(
  min: 0.0,
  max: 30.0,
  step: 0.1,
  decimals: 1,
  value: _currentValue,
  onChanged: (value) {
    print(value);
    setState(() {
          _currentValue = value;
    });
    },
)
            );
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
              Expanded(
                child: ActionButton(
                  text: "Send",
                  color: Colors.blue,
                  //onTap: () => _sendData(_currentValue.toString()+"/"+_firstMarkerValue.toString()+"/"+_secondMarkerValue.toString()+"\n"),
                  onTap: () => _sendData(_currentValue.toString()),
                ),
              ),
              Expanded(
                child: ActionButton(
                  text: "Start",
                  color: Colors.green,
                  onTap: () => _sendData("START"),
                ),
              ),
            ],
          ),
          Container(
            margin: EdgeInsets.all(10),
            child: Row(children: [
              Expanded(
                child: ActionButton(
                  text: "Stop",
                  color: Colors.orange,
                  onTap: () => _sendData("STOP"),
                ),
              ),
              Expanded(
                child: ActionButton(
                    text: "Emergency Stop",
                    color: Colors.red,
                    onTap: () => _sendData("EMERGENCY_STOP")),
              ),
            ]),
          ),
          Row(
            children: [
              //Text(_currentValue.toString(), style: TextStyle(fontSize: 12)),
              //button2(),
              //button1()
              button3(),
            ],
          ),
          Container(
            height: 300,
            width: 300,
            child: SfRadialGauge(
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
                ]),
          ),
        ],
      ),
    );
  }
}

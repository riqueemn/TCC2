import 'dart:ffi';

import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';
import 'package:syncfusion_flutter_sliders/sliders.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  double _firstMarkerValue = 20;
  double _secondMarkerValue = 80;
  double _value = 0.5;

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

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
        home: Scaffold(
            body: SafeArea(
                child: Center(
                    child: Column(children: <Widget>[
      Column(
        children: <Widget>[
          Text('Velocidade'),
          Container(
              child: SfSlider(
            value: _value,
            onChanged: (dynamic newValue) {
              setState(() {
                _value = newValue;
              });
            },
          ))
        ],
      ),
      Container(
        height: 300,
        width: 300,
        child: SfRadialGauge(
            title: GaugeTitle(
                text: 'Ângulo',
                textStyle: TextStyle(
                    fontSize: 15,
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
      )
    ])))));
  }
}

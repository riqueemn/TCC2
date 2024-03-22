import 'package:flutter/material.dart';

class HomePage extends StatefulWidget {
  const HomePage({Key? key}) : super(key: key);

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  double _currentValue = 0;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: Column(
      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
      children: [
        Text(_currentValue.toString(), style: TextStyle(fontSize: 20)),
        Slider(
          value: _currentValue,
          min: 0,
          max: 10,
          divisions: 4,
          label: _currentValue.toString(),
          activeColor: Colors.blue,
          thumbColor: Colors.red,
          onChanged: (value) {
            setState(() {
              _currentValue = value;
            });
          },
        ),
      ],
    ));
  }
}

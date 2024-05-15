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
      appBar: AppBar(
        title: const Text("Bluetooth Connect"),
        floatingActionButton: const FloatingActionButton(
          onPressed: null,
          child: Icon(Icons.wifi_tethering),
          backgroundColor: Colors.grey,
        )
      ),
      body: Column(
      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
      children: [
        Row(
          mainAxisAlignment: mainAxisAlignment.spaceBetween,
          children:[
            Text(
              "off",
              style: TextStyle(color: Colors.red)
            ),
            ElevateButton(
              onPressed: null, 
              child: const Text("Bonded Devices"),
            ),
          ],
        ),
        Center(child: Text("Turn bluetooth on")),
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

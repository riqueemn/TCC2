#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif


BluetoothSerial SerialBT;

char leitura;

void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32teste");
}

void loop() {
  if(SerialBT.available() > 0){
      leitura = (char)SerialBT.read();
      Serial.print(leitura);
  }
}

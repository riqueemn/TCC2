
#include "BluetoothSerial.h"
#include "REG_CFW100.h"
#include <ModbusMaster.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

ModbusMaster node;
BluetoothSerial SerialBT;

char c;
String strVelocidade = "";

#define MAX485_DE_RE 22  // DE e RE
#define SERIAL_RX_PIN 16  // RO
#define SERIAL_TX_PIN 17  // DI

void preTransmission(){
    digitalWrite(MAX485_DE_RE, 1);
}

void postTransmission(){
    digitalWrite(MAX485_DE_RE, 0);
}

float HexToFloat(uint32_t x){
    return (*(float*)&x);
}

float FloatToHex(float x){
    return (*(uint32_t*)&x);
}

float UintToInt(uint64_t x){
    return (*(int64_t*)&x);
}

float HexToDouble(uint32_t x){
    return (*(double*)&x);
}

void Error(uint16_t REG){
  Serial.print("Falha na Conexão modbus. REG >>> ");
  Serial.println(REG);
  delay(1000);
}

float Read_Meter_Float(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_INVERSOR,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    //if(REG != 12) {
        result = node.readHoldingRegisters(REG, 2);
        delay(500);

        if(result == node.ku8MBSuccess){
            for(j = 0; j < 2; j++){
                data[j] = (node.getResponseBuffer(j));
            }
            Serial.print(data[1],HEX);
            Serial.println(data[0],HEX);

            value = data[0];
            value = value << 16;
            value += data[1];

            i = HexToFloat(value);

            return i;
        } else {
            Serial.print("Falha na Conexão modbus. REG >>> ");
            Serial.println(REG);
            delay(1000);
            return 0;
        }
    //}
}

void Start(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_INVERSOR,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    //if(REG != 12) {
        result = node.writeSingleRegister(REG, 0x0003);
        delay(100);

        if(result != node.ku8MBSuccess){
            Error(REG);
        }
    //}
}


void Stop(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_INVERSOR,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    //if(REG != 12) {
        result = node.writeSingleRegister(REG, 0x0000);
        delay(100);

        if(result != node.ku8MBSuccess){
          Error(REG);
        }
    //}
}

void Write_Multiple_Register(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_INVERSOR,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    //if(REG != 12) {
        result = node.writeMultipleRegisters(REG, 2);
        data[0] = 3;
        data[1] = 4096;
        node.setTransmitBuffer(0, data[0]);
        node.setTransmitBuffer(1, data[1]);
        delay(100);

        Serial.print(data[1]);

        if(result != node.ku8MBSuccess){
          Error(REG);
        }
    //}
}

void Emergency_Stop(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_INVERSOR,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    //if(REG != 12) {
        result = node.writeSingleRegister(REG, 64);
        data[0] = 3;
        data[1] = 4096;
        node.setTransmitBuffer(0, data[0]);
        node.setTransmitBuffer(1, data[1]);
        delay(100);

        Serial.print(data[1]);

        if(result != node.ku8MBSuccess){
          Error(REG);
        }
    //}
}



void SetVelocity(char addr, uint16_t REG, int vel){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_INVERSOR,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    int v = (vel * 8192) / 60;

    //if(REG != 12) {
        result = node.writeSingleRegister(REG, v);
        delay(100);

        if(result != node.ku8MBSuccess){
          Error(REG);
        }
    //}
}

void GET_MEDIDA(){
    delay(700);
    for(char i = 0; i < 2; i++){
        DATA_MEDIDA[i] = Read_Meter_Float(ID_INVERSOR, REG_ADDR_READ[i]);
    }
}

void ExecMedida(){
  GET_MEDIDA();
  Serial.println("Lendo as tensões do dispositivo");
  Serial.print("V = "); Serial.print(DATA_MEDIDA[0],3);Serial.println(" V");
  Serial.print("A = "); Serial.print(DATA_MEDIDA[1],3);Serial.println(" A");
  Serial.print("");
  
  delay(10000);
}

void Teste(){
  Start(ID_INVERSOR, REG_ADDR_WRITE[0]);
  SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], 10);
  delay(4000);
  Stop(ID_INVERSOR, REG_ADDR_WRITE[0]);
  delay(4000);
  Start(ID_INVERSOR, REG_ADDR_WRITE[0]);
  SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], 15);
  delay(4000);
  Emergency_Stop(ID_INVERSOR, REG_ADDR_WRITE[0]);
  delay(4000);
}

void ControleBluetooth(){
  while(SerialBT.available()){
      c = (char)SerialBT.read();

      strVelocidade += c; 
  }

  if(strVelocidade.length() > 0){ 
    if(strVelocidade == "START"){
      Start(ID_INVERSOR, REG_ADDR_WRITE[0]);
    } else if(strVelocidade == "STOP"){
      Stop(ID_INVERSOR, REG_ADDR_WRITE[0]);
    } else if(strVelocidade == "EMERGENCY_STOP"){
      Emergency_Stop(ID_INVERSOR, REG_ADDR_WRITE[0]);
    } else{
      int v = strVelocidade.toInt();     
      SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], v);
    }

    strVelocidade = "";
  }
}

void setup(){
    Serial.begin(9600);
    Serial1.begin(9600, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    pinMode(MAX485_DE_RE, OUTPUT);

    SerialBT.begin("ESP32");

    digitalWrite(MAX485_DE_RE, 0);
}

void loop(){

  //ExecMedida();
  //ControleBluetooth();
  Teste();
}

#include "REG_CFW100.h"
#include <ModbusMaster.h>

ModbusMaster node;

#define MAX485_DE 22
#define SERIAL_RX_PIN 16
#define SERIAL_TX_PIN 17

void preTransmission(){
    digitalWrite(MAX485_DE, 1);
}

void postTransmission(){
    digitalWrite(MAX485_DE, 0);
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

float Read_Meter_Float(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_METER,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    if(REG != 12) {
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
            Serial.print("Connect modbus fail. REG >>> ");
            Serial.println(REG);
            delay(1000);
            return 0;
        }
    }
}

void Start(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_METER,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    if(REG != 12) {
        result = node.writeSingleRegister(REG, 0x0003);
        delay(100);

        if(result == node.ku8MBSuccess){
            /*
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
            */
        } else {
            Serial.print("Connect modbus fail. REG >>> ");
            Serial.println(REG);
            delay(1000);
            //return 0;
        }
    }
}


void Stop(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_METER,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    if(REG != 12) {
        result = node.writeSingleRegister(REG, 0x0000);
        delay(100);

        if(result == node.ku8MBSuccess){
            /*
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
            */
        } else {
            Serial.print("Connect modbus fail. REG >>> ");
            Serial.println(REG);
            delay(1000);
            //return 0;
        }
    }
}

void Write_Multiple_Register(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_METER,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    if(REG != 12) {
        result = node.writeMultipleRegisters(REG, 2);
        uint16_t data[] = {3, 4096};
        node.setTransmitBuffer(0, data[0]);
        node.setTransmitBuffer(1, data[1]);
        delay(100);

        Serial.print(data[1]);
    }
}

void Emergency_Stop(char addr, uint16_t REG){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_METER,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    if(REG != 12) {
        result = node.writeSingleRegister(REG, 64);
        uint16_t data[] = {3, 4096};
        node.setTransmitBuffer(0, data[0]);
        node.setTransmitBuffer(1, data[1]);
        delay(100);

        Serial.print(data[1]);
    }
}



void SetVelocity(char addr, uint16_t REG, int vel){
    float i = 0;
    uint8_t result, j;

    uint16_t data[2];
    uint32_t value = 0;
    node.begin(ID_METER,Serial1);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

    int v = (vel * 8192) / 60;

    if(REG != 12) {
        result = node.writeSingleRegister(REG, v);
        delay(100);

        if(result == node.ku8MBSuccess){
            /*
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
            */
        } else {
            Serial.print("Connect modbus fail. REG >>> ");
            Serial.println(REG);
            delay(1000);
            //return 0;
        }
    }
}

void GET_METER(){
    delay(700);
    for(char i = 0; i < 2; i++){
        DATA_METER[i] = Read_Meter_Float(ID_METER, REG_ADDR_READ[i]);
    }
}

void execMeter(){
  GET_METER();
  Serial.println("Lendo as tensões do dispositivo");
  Serial.print("Vab = "); Serial.print(DATA_METER[0],3);Serial.println(" V");
  Serial.print("Vbc = "); Serial.print(DATA_METER[1],3);Serial.println(" V");
  Serial.print("Vca = "); Serial.print(DATA_METER[2],3);Serial.println(" V");
  Serial.print("Vavg = "); Serial.print(DATA_METER[3],3);Serial.println(" V");
  Serial.print("");
  Serial.println("Lendo as correntes do dispositivo");
  Serial.print("Ia = "); Serial.print(DATA_METER[4],3);Serial.println(" A");
  Serial.print("Ib = "); Serial.print(DATA_METER[5],3);Serial.println(" A");
  Serial.print("Ic = "); Serial.print(DATA_METER[6],3);Serial.println(" A");
  Serial.print("Iavg = "); Serial.print(DATA_METER[7],3);Serial.println(" A");
  Serial.print("");
  Serial.println("Lendo as potências do dispositivo");
  Serial.print("Wt = "); Serial.print(DATA_METER[8],3);Serial.println(" KW");
  Serial.print("Qt = "); Serial.print(DATA_METER[9],3);Serial.println(" KVAr");
  Serial.print("St = "); Serial.print(DATA_METER[10],3);Serial.println(" KVA");
  Serial.print("FPt "); Serial.print(DATA_METER[11],6);Serial.println(" ");
  Serial.print("");
  Serial.println("Lendo dos THDs por Fase");
  Serial.print("THDAB = "); Serial.print(DATA_METER[12]); Serial.println(" %");
  Serial.print("THDBC = "); Serial.print(DATA_METER[13]); Serial.println(" %");
  Serial.print("THDCA = "); Serial.print(DATA_METER[14]); Serial.println(" %");
  Serial.print("");
  
  delay(10000);
}

void setup(){
    Serial.begin(9600);
    Serial1.begin(9600, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    pinMode(MAX485_DE, OUTPUT);

    digitalWrite(MAX485_DE, 0);
}

void loop(){
  Start(ID_METER, REG_ADDR_WRITE[0]);
  SetVelocity(ID_METER, REG_ADDR_WRITE[1], 50);
  delay(5000);
  Emergency_Stop(ID_METER, REG_ADDR_WRITE[0]);
  delay(10000);
  
  /*Start(ID_METER, REG_ADDR_WRITE[0]);
  SetVelocity(ID_METER, REG_ADDR_WRITE[1], 50);
  delay(5000);
  Stop(ID_METER, REG_ADDR_WRITE[0]);
  delay(10000);*/

  //Write_Multiple_Register(ID_METER, 682);
}

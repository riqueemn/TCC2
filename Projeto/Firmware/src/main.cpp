
#include "BluetoothSerial.h"
#include "REG_CFW100.h"
#include <ModbusMaster.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define MAX485_RE 19  // RE
#define MAX485_DE 18  // DE
#define SERIAL_RX_PIN 16  // RO
#define SERIAL_TX_PIN 17  // DI

struct Automacao_Triciclo{
  bool isConnected;
  bool isTest;

};

bool isTest = true;


ModbusMaster node;
BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 4);


char c;
String strVelocidade = "";
bool isConnected;






void preTransmission(){
    digitalWrite(MAX485_DE, 1);
    digitalWrite(MAX485_RE, 1);
}

void postTransmission(){
    digitalWrite(MAX485_DE, 0);
    digitalWrite(MAX485_RE, 0);
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
    lcd.clear();
    lcd.print("Start");

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
    lcd.clear();
    lcd.print("Stop");

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
    lcd.clear();
    lcd.print("Emergency Stop");

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
  delay(1000);
  Stop(ID_INVERSOR, REG_ADDR_WRITE[0]);
  delay(1000);
  Start(ID_INVERSOR, REG_ADDR_WRITE[0]);
  SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], 15);
  delay(1000);
  Emergency_Stop(ID_INVERSOR, REG_ADDR_WRITE[0]);
  delay(1000);
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

void StatusCarregandoDisplay(){
	lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desconectado...");
  
  int i = 0;
  int l = 6;
  
  while(!isConnected){
    isConnected = SerialBT.connected();
    if(i < l){
      lcd.setCursor(i, 1);
    	lcd.print("|");
    }else if(i < 16){
      lcd.setCursor(i-l+1, 1);
      for(int k=0; k<l; k++){
        lcd.print("|");
      }
      lcd.setCursor(i-l, 1);
      lcd.print(" ");
    } else {
      lcd.setCursor(i-l, 1);
      lcd.print(" ");
    }
      
    delay(500);
    i++;
    if(i >= (15+l+1)){
     	i = 0;
    }
  }
  lcd.clear();
  lcd.print("Conexao Aceita");
  delay(4000);
  lcd.clear();
  lcd.print("Controle");
  lcd.setCursor(0, 1);
  lcd.print("Liberado");
}

void StatusDisplay(String s){
  lcd.print(s);
}

void setup(){
    Serial.begin(9600);
    Serial1.begin(9600, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
    pinMode(MAX485_DE, OUTPUT);
    pinMode(MAX485_RE, OUTPUT);

    SerialBT.begin("AUTOMACAO_TRICICLO");
    
    
    digitalWrite(MAX485_DE, 0);
    digitalWrite(MAX485_RE, 0);

    lcd.init();
	  lcd.backlight();
}

void loop(){
  //ExecMedida();
  
  if(isTest){
    Teste();
  }else {
    isConnected = SerialBT.connected();
    if(!isConnected){
      StatusCarregandoDisplay();
    } else {
      ControleBluetooth();  
    }
  }
}


#include "BluetoothSerial.h"
#include "REG_CFW100.h"
#include <ModbusMaster.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define MAX485_RE 19     // RE
#define MAX485_DE 18     // DE
#define SERIAL_RX_PIN 16 // RO
#define SERIAL_TX_PIN 17 // DI

struct Automacao_Triciclo
{
  bool isConnected = false;
  bool isTest = false;
  bool isConfig = true;
  bool isReset = true;
  bool sentidoGiro = 1;

  int typeConfig = 1;
  float frequencia = 0;

  String strCMD = "";
  char c;

  LiquidCrystal_I2C lcd();
  ModbusMaster node;
  BluetoothSerial SerialBT;
};

bool isTest = false;
bool isConfig = false;
int typeConfig = 1;
bool isReset = false;
float frequencia = 0;
bool sentidoGiro = 1;

ModbusMaster node;
BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 4);

char c;
String strCMD = "";
bool isConnected;

void preTransmission()
{
  digitalWrite(MAX485_DE, 1);
  digitalWrite(MAX485_RE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_DE, 0);
  digitalWrite(MAX485_RE, 0);
}

void Error(uint16_t REG)
{
  Serial.print("Falha na Conexão modbus. REG >>> ");
  Serial.println(REG);
  delay(1000);
}

float Read_Meter_Float(char addr, uint16_t REG)
{
  float i = 0;
  uint8_t result, j;

  uint16_t data[2];
  uint32_t value = 0;
  node.begin(ID_INVERSOR, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // if(REG != 12) {
  result = node.readHoldingRegisters(REG, 2);
  delay(500);

  if (result == node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = (node.getResponseBuffer(j));
    }
    Serial.print(data[1], HEX);
    Serial.println(data[0], HEX);

    value = data[0];
    value = value << 16;
    value += data[1];

    i = HexToFloat(value);

    return i;
  }
  else
  {
    Serial.print("Falha na Conexão modbus. REG >>> ");
    Serial.println(REG);
    delay(1000);
    return 0;
  }
  //}
}

void WriteSingleRegister(uint16_t REG, uint16_t value, String s)
{
  lcd.clear();
  lcd.print(s);
  float i = 0;
  uint8_t result, j;
  uint16_t data[2];
  node.begin(ID_INVERSOR, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  // if(REG != 12) {
  result = node.writeSingleRegister(REG, value);
  delay(100);
  if (result != node.ku8MBSuccess)
  {
    Error(REG);
  }
  //}
}

void WriteSingleRegister2(uint16_t REG, uint16_t value)
{
  float i = 0;
  uint8_t result, j;
  uint16_t data[2];
  node.begin(ID_INVERSOR, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  // if(REG != 12) {
  result = node.writeSingleRegister(REG, value);
  delay(100);
  if (result != node.ku8MBSuccess)
  {
    Error(REG);
  }
  //}
}

void Start()
{
  if (sentidoGiro)
  {
    WriteSingleRegister(REG_ADDR_WRITE[0], 0x0007, "Start");
  }
  else
  {
    WriteSingleRegister(REG_ADDR_WRITE[0], 0x0003, "Start Reversao");
  }
}

void Stop()
{
  WriteSingleRegister(REG_ADDR_WRITE[0], 0x0000, "Stop");
}

void Emergency_Stop()
{
  WriteSingleRegister(REG_ADDR_WRITE[0], 0x0040, "Emergency Stop");
}

void Reversao()
{
  Emergency_Stop();
  sentidoGiro = !sentidoGiro;
}

void ConfigControlBornes()
{
  Stop();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Configurando");
  lcd.setCursor(0, 1);
  lcd.print("Modo Serial");
  lcd.setCursor(0, 2);

  for (int i = 0; i < 9; i++)
  {
    WriteSingleRegister2(REG_ADDR_CONFIG[i], REG_ADDR_CONFIG_VALUE_2[i]);
    lcd.setCursor(i, 2);
    lcd.print("|");
    lcd.setCursor(0, 3);
    lcd.print(String(i * 10) + "%");

    delay(100);
  }
}

void ConfigControlSerial()
{
  Stop();


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Configurando");
  lcd.setCursor(0, 1);
  lcd.print("Modo Serial");
  lcd.setCursor(0, 2);

  for (int i = 0; i < 10; i++)
  {
    WriteSingleRegister2(REG_ADDR_CONFIG[i], REG_ADDR_CONFIG_VALUE_1[i]);
    lcd.setCursor(i, 2);
    lcd.print("|");
    lcd.setCursor(0, 3);
    lcd.print(String(i * 10) + "%");

    delay(100);
  }

}

void Reset()
{
  Emergency_Stop();
  WriteSingleRegister(204, 5, "Reset");
}

void Config()
{
  Stop();
  lcd.clear();
  lcd.print("Configurando...");

  WriteSingleRegister2(399, 78.2);
  WriteSingleRegister2(400, 220);
  WriteSingleRegister2(401, 1.77);
  WriteSingleRegister2(402, 1700);
  WriteSingleRegister2(403, 60);
  WriteSingleRegister2(404, 3);
  WriteSingleRegister2(407, 0.7);
  WriteSingleRegister2(134, 150);
  WriteSingleRegister2(120, 0);

  if (typeConfig == 0)
  {
    ConfigControlBornes();
  }
  else if (typeConfig == 1)
  {
    ConfigControlSerial();
  }

  isConfig = true;

  lcd.clear();

  delay(1000);
}

void Write_Multiple_Register(char addr, uint16_t REG)
{
  float i = 0;
  uint8_t result, j;

  uint16_t data[2];
  uint32_t value = 0;
  node.begin(ID_INVERSOR, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // if(REG != 12) {
  result = node.writeMultipleRegisters(REG, 2);
  data[0] = 3;
  data[1] = 4096;
  node.setTransmitBuffer(0, data[0]);
  node.setTransmitBuffer(1, data[1]);
  delay(100);

  Serial.print(data[1]);

  if (result != node.ku8MBSuccess)
  {
    Error(REG);
  }
  //}
}

void SetVelocity(char addr, uint16_t REG, float vel)
{
  float i = 0;
  uint8_t result, j;

  uint16_t data[2];
  uint32_t value = 0;
  node.begin(ID_INVERSOR, Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  int v = (vel * 8192) / 60;

  // if(REG != 12) {
  result = node.writeSingleRegister(REG, v);
  delay(100);

  if (result != node.ku8MBSuccess)
  {
    Error(REG);
  }
  //}
}

void GET_MEDIDA()
{
  delay(700);
  for (char i = 0; i < 2; i++)
  {
    DATA_MEDIDA[i] = Read_Meter_Float(ID_INVERSOR, REG_ADDR_READ[i]);
  }
}

void ExecMedida()
{
  GET_MEDIDA();
  Serial.println("Lendo as tensões do dispositivo");
  Serial.print("V = ");
  Serial.print(DATA_MEDIDA[0], 3);
  Serial.println(" V");
  Serial.print("A = ");
  Serial.print(DATA_MEDIDA[1], 3);
  Serial.println(" A");
  Serial.print("");

  delay(1000);
}

void Teste()
{
  Start();
  SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], 10);
  delay(1000);
  Stop();
  delay(1000);
  Start();
  SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], 15);
  delay(1000);
  Emergency_Stop();
  delay(1000);
}

void ControleBluetooth()
{
  while (SerialBT.available())
  {
    c = (char)SerialBT.read();

    strCMD += c;
  }

  if (strCMD.length() > 0)
  {
    if (strCMD == "START")
    {
      Start();
    }
    else if (strCMD == "STOP")
    {
      Stop();
    }
    else if (strCMD == "EMERGENCY_STOP")
    {
      Emergency_Stop();
    }
    else if (strCMD == "REVERSAO")
    {
      Reversao();
    } else if (strCMD == "MODO_QUADRO")
    {
      ConfigControlBornes();
    } else if (strCMD == "MODO_APP")
    {
      ConfigControlSerial();
    }
    else
    {
      frequencia = strCMD.toFloat();
      SetVelocity(ID_INVERSOR, REG_ADDR_WRITE[1], frequencia);
    }

    strCMD = "";
  }
}

void StatusCarregandoDisplay()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desconectado...");

  int i = 0;
  int l = 6;

  while (!isConnected)
  {
    isConnected = SerialBT.connected();
    if (i < l)
    {
      lcd.setCursor(i, 1);
      lcd.print("|");
    }
    else if (i < 16)
    {
      lcd.setCursor(i - l + 1, 1);
      for (int k = 0; k < l; k++)
      {
        lcd.print("|");
      }
      lcd.setCursor(i - l, 1);
      lcd.print(" ");
    }
    else
    {
      lcd.setCursor(i - l, 1);
      lcd.print(" ");
    }

    delay(500);
    i++;
    if (i >= (15 + l + 1))
    {
      i = 0;
    }
  }
  lcd.clear();
  lcd.print("Conexao Aceita");
  delay(1000);
  lcd.clear();
  lcd.print("Controle");
  lcd.setCursor(0, 1);
  lcd.print("Liberado");
}

void StatusDisplay(String s)
{
  lcd.print(s);
}

void setup()
{
  Serial.begin(19200);
  Serial1.begin(19200, SERIAL_8E1, SERIAL_RX_PIN, SERIAL_TX_PIN);
  pinMode(MAX485_DE, OUTPUT);
  pinMode(MAX485_RE, OUTPUT);

  SerialBT.begin("AUTOMACAO_TRICICLO");

  digitalWrite(MAX485_DE, 0);
  digitalWrite(MAX485_RE, 0);

  lcd.init();
  lcd.backlight();
}

void loop()
{

  if (isReset)
  {
    Reset();
  }

  if (!isConfig)
  {
    Config();
  }

  
  else
  {
    if (isTest)
    {
      Teste();
    }
    else
    {
      isConnected = SerialBT.connected();
      if (!isConnected)
      {
        StatusCarregandoDisplay();
        Emergency_Stop();
      }
      else
      {
        ControleBluetooth();
      }
    }
  }
}

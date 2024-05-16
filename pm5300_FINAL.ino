#include "REG_PM5300.h"
#include <ModbusMaster.h>

#define baud 9600
#define timeout 100
#define  polling 100
#define retry_count 10

ModbusMaster node;

#define MAX485_DE      22
#define MAX485_RE_NEG  21

void preTransmission()
{
  digitalWrite(MAX485_RE_NEG, 1);
  digitalWrite(MAX485_DE, 1);
}

void postTransmission()
{
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}


float HexTofloat(uint32_t x) 
{
  return (*(float*)&x);
}

uint32_t FloatTohex(float x) 
{
  return (*(uint32_t*)&x);
}
int64_t HexToint(uint32_t x) 
{
  return (*(int32_t*)&x);
}

float HextoDouble(uint32_t x) 
{
  return (*(double*)&x);
}

//------------------------------------------------

float Read_Meter_float(char addr , uint16_t  REG) 
{
  float i = 0;
  uint8_t result,j;

  uint16_t data[2];
  uint32_t value = 0;
  node.begin(ID_meter,Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  result = node.readHoldingRegisters(REG,2); ///< Modbus function 0x03 Read Holding Registers
  delay(200);
  if (result == node.ku8MBSuccess) 
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = (node.getResponseBuffer(j));
    }

        value = data[0];
        value = value << 16;
        value = value + data[1];
   
    i = HexTofloat(value);
    
    return i;
  }  
     else 
  {
    Serial.print("Connect modbus fail. REG >>> "); Serial.println(REG); // Debug
    delay(1000); 
    return 0;
  } 
}

int32_t Read_Meter_energy(char addr , uint16_t  REGS) 
{
  float i = 0;
  uint8_t result,j;

  uint16_t data[4];
  uint32_t value = 0;
  node.begin(ID_meter,Serial1);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
 
  result = node.readHoldingRegisters(REGS,4); ///< Modbus function 0x03 Read Holding Registers
  delay(500);
  if (result == node.ku8MBSuccess) 
  {
    for (j = 0; j < 4; j++)
    {
      data[j] = (node.getResponseBuffer(j));
    }   
        value = data[3];
        value = value << 16;
        value = value + data[2];

    i = HexToint(value);
    
    return i;
  }  
     else 
  {
    Serial.print("Connect modbus fail. REGS >>> "); Serial.println(REGS); // Debug
    delay(1000); 
    return 0;
  } 
}


void GET_METER() 
{     // Update read all data
  delay(700);                            
    for (char i = 0; i < Total_of_Reg_Meter ; i++)
    {
      DATA_METER [i] = Read_Meter_float(ID_meter, Reg_addr[i]);
    }
    for (char i =0; i <Total_of_Reg_Energy; i++)
    {
      DATA_ENERGY[i] = Read_Meter_energy(ID_meter, Regs_addr[i]);  
    }
}


void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600,SERIAL_8N2);

  Serial.println(F("Teste"));
  pinMode(MAX485_RE_NEG, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  // Init in receive mode
  digitalWrite(MAX485_RE_NEG, 0);
  digitalWrite(MAX485_DE, 0);
}

void loop() 
{
  GET_METER();
  Serial.println("Lendo as tensões do dispositivo");
  Serial.print("Vab = "); Serial.print(DATA_METER[0],3);Serial.println(" V"); // Tensão de linha AB
  Serial.print("Vbc = "); Serial.print(DATA_METER[1],3);Serial.println(" V"); // Tensão de linha BC
  Serial.print("Vca = "); Serial.print(DATA_METER[2],3);Serial.println(" V"); // Tensão de linha CA
  Serial.print("Vavg = "); Serial.print(DATA_METER[3],3);Serial.println(" V"); // Tensão de linha média
  Serial.print("");
  Serial.println("Lendo as correntes do dispositivo");
  Serial.print("Ia = "); Serial.print(DATA_METER[4],3);Serial.println(" A"); // Corrente de linha A
  Serial.print("Ib = "); Serial.print(DATA_METER[5],3);Serial.println(" A"); // Corrente de linha B
  Serial.print("Ic = "); Serial.print(DATA_METER[6],3);Serial.println(" A"); // Corrente de linha C
  Serial.print("Iavg = "); Serial.print(DATA_METER[7],3);Serial.println(" A"); // Corrente de linha média
  Serial.print("");
  Serial.println("Lendo as potências do dispositivo");
  Serial.print("Wt = "); Serial.print(DATA_METER[8],3);Serial.println(" W");   // potência Ativa
  Serial.print("Qt = "); Serial.print(DATA_METER[9],3);Serial.println(" VAr"); // potência Reativa
  Serial.print("St = "); Serial.print(DATA_METER[10],3);Serial.println(" VA"); // potência Aparente
  Serial.print("FPt = "); Serial.print(DATA_METER[11],6);Serial.println("  "); // Fator de potência
  Serial.print("");
  Serial.println("Lendo dos THDs por Fase");
  Serial.print("THDAB = "); Serial.print(DATA_METER[12]); Serial.println(" %"); // Harmônicos totais da tensão AB
  Serial.print("THDBC = "); Serial.print(DATA_METER[13]); Serial.println(" %"); // Harmônicos totais da tensão BC
  Serial.print("THDCA = "); Serial.print(DATA_METER[14]); Serial.println(" %"); // Harmônicos totais da tensão CA
  Serial.print("");
  Serial.println("Lendo os hamônicos de tensão por fase - Fase AB do 1° ao 9° ");
  Serial.print("THDAB1 = "); Serial.print(DATA_METER[15]); Serial.println(" %");
  Serial.print("THDAB2 = "); Serial.print(DATA_METER[16]); Serial.println(" %");
  Serial.print("THDAB3 = "); Serial.print(DATA_METER[17]); Serial.println(" %");
  Serial.print("THDAB4 = "); Serial.print(DATA_METER[18]); Serial.println(" %");
  Serial.print("THDAB5 = "); Serial.print(DATA_METER[19]); Serial.println(" %");
  Serial.print("THDAB6 = "); Serial.print(DATA_METER[20]); Serial.println(" %");
  Serial.print("THDAB7 = "); Serial.print(DATA_METER[21]); Serial.println(" %");
  Serial.print("THDAB8 = "); Serial.print(DATA_METER[22]); Serial.println(" %");
  Serial.print("THDAB9 = "); Serial.print(DATA_METER[23]); Serial.println(" %");
  Serial.println("Lendo os hamônicos de tensão por fase - Fase BC do 1° ao 9°");
  Serial.print("THDBC1 = "); Serial.print(DATA_METER[24]); Serial.println(" %");
  Serial.print("THDBC2 = "); Serial.print(DATA_METER[25]); Serial.println(" %");
  Serial.print("THDBC3 = "); Serial.print(DATA_METER[26]); Serial.println(" %");
  Serial.print("THDBC4 = "); Serial.print(DATA_METER[27]); Serial.println(" %");
  Serial.print("THDBC5 = "); Serial.print(DATA_METER[28]); Serial.println(" %");
  Serial.print("THDBC6 = "); Serial.print(DATA_METER[29]); Serial.println(" %");
  Serial.print("THDBC7 = "); Serial.print(DATA_METER[30]); Serial.println(" %");
  Serial.print("THDBC8 = "); Serial.print(DATA_METER[31]); Serial.println(" %");
  Serial.print("THDBC9 = "); Serial.print(DATA_METER[32]); Serial.println(" %");
  Serial.println("Lendo os hamônicos de tensão por fase - Fase CA do 1° ao 9° ");
  Serial.print("THDCA1 = "); Serial.print(DATA_METER[33]); Serial.println(" %");
  Serial.print("THDCA2 = "); Serial.print(DATA_METER[34]); Serial.println(" %");
  Serial.print("THDCA3 = "); Serial.print(DATA_METER[35]); Serial.println(" %");
  Serial.print("THDCA4 = "); Serial.print(DATA_METER[36]); Serial.println(" %");
  Serial.print("THDCA5 = "); Serial.print(DATA_METER[37]); Serial.println(" %");
  Serial.print("THDCA6 = "); Serial.print(DATA_METER[38]); Serial.println(" %");
  Serial.print("THDCA7 = "); Serial.print(DATA_METER[39]); Serial.println(" %");
  Serial.print("THDCA8 = "); Serial.print(DATA_METER[40]); Serial.println(" %");
  Serial.print("THDCA9 = "); Serial.print(DATA_METER[41]); Serial.println(" %");
  
  Serial.print("");
  Serial.println("Lendo a Energia Acumulada do sistema ");
  Serial.print("AEDI = "); Serial.print(DATA_ENERGY[0]); Serial.println(" Wh");
  Serial.print("AEDO = "); Serial.print(DATA_ENERGY[1]); Serial.println(" Wh");
  Serial.print("AEDRM = "); Serial.print(DATA_ENERGY[2]); Serial.println(" Wh");
  Serial.print("AEDRm = "); Serial.print(DATA_ENERGY[3]); Serial.println(" Wh");
  Serial.print("QEDI = "); Serial.print(DATA_ENERGY[4]); Serial.println(" VArh");
  Serial.print("QEDO = "); Serial.print(DATA_ENERGY[5]); Serial.println(" VArh");
  Serial.print("QEDRM = "); Serial.print(DATA_ENERGY[6]); Serial.println(" VArh");
  Serial.print("QEDRm = "); Serial.print(DATA_ENERGY[7]); Serial.println(" VArh");
  Serial.print("SEDI = "); Serial.print(DATA_ENERGY[8]); Serial.println(" VAh");
  Serial.print("SEDO = "); Serial.print(DATA_ENERGY[9]); Serial.println(" VAh");
  Serial.print("SEDRM = "); Serial.print(DATA_ENERGY[10]); Serial.println(" VAh");
  Serial.print("SEDRm = "); Serial.print(DATA_ENERGY[11]); Serial.println(" VAh");
  
  delay(9000);
}

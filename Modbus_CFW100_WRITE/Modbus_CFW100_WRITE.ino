#include<ModbusRtu.h>

uint16_t au16data[16];
uint8_t u8state;



Modbus master(0, 1, 0);

modbus_t telegram[1];
unsigned long u32wait;


void setup() {
  Serial.begin(19200);

  master.start();
  master.setTimeOut(1000);
  u32wait = millis();
}

void loop() {
  switch(u8state) {
    case 0:
      if(millis() > (u32wait + 250)){
        uint8_t aux;

        au16data[0] = 0x12;

        if(digitalRead(B10) == 1) au16data[0] != 0x01;
        if(digitalRead(B11) == 1) au16data[0] != 0x04;

        au16data[1] = 0;

        for(aux=0; aux <= 4; aux++){
          au16data[1] += (int16_t) 250;
        }

        au16data[1] >>= 1;

        u8state++;
      }
      break;
    
    case 1:
      telegram[0].u8id = 1;
      telegram[0].u8fct = 16;
      telegram[0].u16RegAdd = 682;
      telegram[0].u16CoilsNo = 2;
      telegram[0].au16reg = au16data;
      master.query(telegram[0]);
      u8state++;
      break;

    case 2:
      master.poll();
      if(master.getState() == COM_IDLE) u8state++;

      break;
    case 3:
      telegram[0].u8id = 1;
      telegram[0].u8fct = 3;
      telegram[0].u16RegAdd = 5;
      telegram[0].u16CoilsNo = 1;
      telegram[0].au16reg = au16data;
      master.query(telegram[0]);
      u8state++;
      break;

    case 4:
      master.poll();
      if(master.getState() == COM_IDLE){

        Serial.print("\n");
        Serial.print((float)au16data[0]/10, 1);
        Serial.print("Hz\n");


        u8state = 0;
        u32wait = millis();
      }

      break;
  }
}

#include <Arduino.h>
#include <ModbusMaster.h>

// Define os pinos da Serial2
#define SERIAL2_RX_PIN 16 // GPIO16
#define SERIAL2_TX_PIN 17 // GPIO17
#define LED 2 //GPIO
#define TRANC_485 4 // Pino de Transmissão
#define TESTE 26

uint8_t valor_lido;
char retorno_MODBUS;

//uint8_t buffer[8] = {0x01, 0x06, 0x02, 0xAB, 0x10, 0x00, 0xF4, 0x52};
//uint8_t buffer[8] = {0x01, 0x06, 0x08, 0xAB, 0x10, 0x00, 0xF7, 0x8A}; //dado teste error
uint8_t buffer[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t buffer_rec[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void setup() {
  // Inicializa a comunicação serial para monitoramento
  Serial.begin(9600);
  // Inicializa a comunicação serial2
  Serial2.begin(9600, SERIAL_8E1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  //Inicializa LED2
  pinMode(LED,OUTPUT);
  pinMode(TRANC_485,OUTPUT);
  pinMode(TESTE,OUTPUT);
}

void loop() {

   if(Serial.available()>0){
    // for (int i = 0; i<8; i ++) {
    //   buffer[i] = Serial.read();
    // }
    Serial.readBytes(buffer, 8);
    digitalWrite(TRANC_485,HIGH);
    Serial2.write(buffer, 8);
    Serial2.flush();
    digitalWrite(TRANC_485,LOW);
  }

  if(Serial2.available()>0){
    // for (int i = 0; i<8; i ++) {
    //   buffer[i] = Serial2.read();
    // }
    Serial2.readBytes(buffer_rec,16);
    Serial2.flush();
    Serial.write(buffer_rec, 16);
    Serial.flush();
    Serial.println("recebeu");
  // }
  }
}

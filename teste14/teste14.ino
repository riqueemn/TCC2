#define SERIAL2_RX_PIN 16
#define SERIAL2_TX_PIN 17
#define LED 2
#define TRANC_485 8

uint8_t valor_lido;
char retorno_MODBUS;

uint8_t buffer[8] = {};
uint8_t buffer_rec[16] = {};

void setup() {
  Serial.begin(9600);
  //Serial2.begin(9600, SERIAL_8E1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  Serial2.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(TRANC_485, OUTPUT);
}

void loop() {
  if(Serial.available() > 0){
      Serial.readBytes(buffer, 8);
      digitalWrite(TRANC_485, HIGH);
      Serial2.write(buffer, 8);
      Serial2.flush();
      digitalWrite(TRANC_485, LOW);
    }

   if(Serial2.available() > 0){
      Serial2.readBytes(buffer_rec, 16);
      Serial2.flush();
      Serial.write(buffer_rec, 16);
      Serial.flush();
      Serial.println("recebeu");
    }
}

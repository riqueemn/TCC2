#define SERIAL2_RX_PIN 16
#define SERIAL2_TX_PIN 17

uint8_t telegram[64];


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8E1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  pinMode(22, OUTPUT);
}

void gira(){
  digitalWrite(22, HIGH);

  telegram[0] = 0x01; // Endereço do escravo
  telegram[1] = 0x06; // Função
  telegram[2] = 0x02; // MEI Type
  telegram[3] = 0xAA; // Código de leitura
  telegram[4] = 0x00; // Código de leitura
  telegram[5] = 0x0B; // Número do Objeto
  telegram[6] = 0xE9; // CRC-
  telegram[7] = 0x95; // CRC+

  Serial1.write(telegram, 8);
  delay(100);
  digitalWrite(22, LOW);  
}

void para(){
  digitalWrite(22, HIGH);
  
  telegram[0] = 0x01; // Endereço do escravo
  telegram[1] = 0x06; // Função
  telegram[2] = 0x02; // MEI Type
  telegram[3] = 0xAA; // Código de leitura
  telegram[4] = 0x00; // Código de leitura
  telegram[5] = 0x0A; // Número do Objeto
  telegram[6] = 0x28; // CRC-
  telegram[7] = 0x55; // CRC+

  Serial1.write(telegram, 8);
  delay(100);
  digitalWrite(22, LOW);  
}

void velocidadeDoMotor(){
  digitalWrite(22, HIGH);

  telegram[0] = 0x01; // Endereço do escravo
  telegram[1] = 0x06; // Função
  telegram[2] = 0x02; // MEI Type
  telegram[3] = 0xAB; // Código de leitura
  telegram[4] = 0x10; // Código de leitura
  telegram[5] = 0x00; // Número do Objeto
  telegram[6] = 0xF4; // CRC-
  telegram[7] = 0x52; // CRC+

  Serial1.write(telegram, 8);
  delay(100);
  digitalWrite(22, LOW);
}

void loop() {
  gira();
  velocidadeDoMotor();
  delay(2000);
  para();
  delay(2000);
}

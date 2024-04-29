uint8_t telegram[64];

void setup() {
  Serial.begin(19200);
  pinMode(8, OUTPUT);
}

void loop() {

  // Controle dos pinos DE e RE

  digitalWrite(8, HIGH);

  // Montagem do pacote

  telegram[0] = 0x01; // Endereço do escravo
  telegram[1] = 0x2B; // Função
  telegram[2] = 0x0E; // MEI Type
  telegram[3] = 0x01; // Código de leitura
  telegram[4] = 0x02; // Número do Objeto
  telegram[5] = 0x70; // CRC-
  telegram[6] = 0x77; // CRC+

  Serial.write(telegram, 7);


  digitalWrite(8, LOW);

  delay(1000);
}

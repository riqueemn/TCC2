#include <ModbusMaster.h>

// Definições de pinos para o ESP32
#define RS485_TX_PIN 16 // Pino de transmissão TX do ESP32 conectado ao pino DI do módulo RS485
#define RS485_RX_PIN 17 // Pino de recepção RX do ESP32 conectado ao pino RO do módulo RS485
#define RS485_TRANSCEIVER_PIN 2 // Pino de controle do transceptor RS485 conectado ao pino DE/RE do módulo RS485
#define LED_PIN 2 // Pino do LED do ESP32

// Objeto ModbusMaster para comunicação Modbus RTU
ModbusMaster node;

// Endereço do dispositivo Modbus do inversor
const int INVERTER_ADDRESS = 1;

// Definir o endereço do registro para controlar a frequência do inversor
#define FREQUENCY_CONTROL_REGISTER_ADDRESS 0x1000

// Definir o endereço do registro para ler a frequência do inversor
#define FREQUENCY_READ_REGISTER_ADDRESS 0x1001

// Variável para armazenar a frequência desejada do inversor (em Hz)
float desiredFrequency = 50; // Frequência desejada inicial

// Declaração da função calculateCRC
uint16_t calculateCRC(uint8_t address, uint16_t register_address, uint16_t register_value);

// Função para configurar o inversor
void configureInverter() {
  // Configurar os parâmetros do inversor para comunicação Modbus
  // Você precisa consultar a documentação do inversor para encontrar os valores corretos para a configuração.
  // Os valores abaixo são apenas um exemplo genérico.

  node.writeSingleRegister(1901, INVERTER_ADDRESS); // Endereço Modbus do inversor
  node.writeSingleRegister(1902, 9600); // Baud rate para comunicação Modbus
  // Outros parâmetros podem ser configurados conforme necessário
}

// Função para definir a frequência do inversor
void setInverterFrequency(float frequency) {
  // Calcular o valor do registro de frequência com base na frequência desejada
  // O registro de frequência normalmente é um valor de 16 bits, então precisamos converter a frequência em um valor apropriado
  // Consulte a documentação do inversor para obter informações sobre como calcular esse valor adequadamente

  // Exemplo de cálculo: supondo que o inversor aceite valores de frequência entre 0 e 60 Hz, e o registro de frequência varie de 0 a 65535
  uint16_t frequencyRegisterValue = map(frequency, 0, 60, 0, 65535);

  // Enviar o comando para definir a frequência do inversor
  uint16_t crc = calculateCRC(INVERTER_ADDRESS, FREQUENCY_CONTROL_REGISTER_ADDRESS, frequencyRegisterValue);
  node.writeSingleRegister(FREQUENCY_CONTROL_REGISTER_ADDRESS, frequencyRegisterValue);
}

// Função para ler a frequência atual do inversor
float getInverterFrequency() {
  // Ler o valor do registro de frequência do inversor
  uint8_t result = node.readHoldingRegisters(FREQUENCY_CONTROL_REGISTER_ADDRESS, 1); // Ler 1 registro a partir do endereço fornecido

  if (result == node.ku8MBSuccess) {
    // Leitura bem-sucedida, recuperar os dados
    uint16_t frequencyRegisterValue = node.getResponseBuffer(0); // Obter o valor lido do registro

    // Calcular a frequência com base no valor do registro
    // Você precisa ajustar esse cálculo com base na faixa de frequência suportada pelo inversor
    // Exemplo: supondo que o inversor aceite frequência de 0 a 60 Hz e o registro de frequência varie de 0 a 65535
    float frequency = map(frequencyRegisterValue, 0, 65535, 0, 60);

    return frequency;
  } else {
    // Falha na leitura do registro Modbus
    Serial.println("Erro na leitura do registro Modbus.");
    return 0.0; // Retornar 0 Hz em caso de falha na leitura
  }
}

void setup() {
  // Inicializar a comunicação serial para debug
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguardar até que a porta serial esteja disponível
  }
  Serial.println("Serial pronta.");

  // Inicializar a comunicação RS485
  pinMode(RS485_TRANSCEIVER_PIN, OUTPUT);
  digitalWrite(RS485_TRANSCEIVER_PIN, LOW); // Definir como modo de transmissão
  Serial1.begin(9600, SERIAL_8N1, RS485_RX_PIN, RS485_TX_PIN);

  // Inicializar o objeto ModbusMaster
  node.begin(INVERTER_ADDRESS, Serial1); // Endereço do dispositivo Modbus e comunicação serial

  // Configurar o LED como saída
  pinMode(LED_PIN, OUTPUT);

  // Configurar o inversor para operar no modo Modbus
  configureInverter();
}

void loop() {
  // Ler a frequência do inversor e exibir no monitor serial
  float frequency = getInverterFrequency();
  Serial.print("Frequência atual do inversor: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  // Piscar o LED
  digitalWrite(LED_PIN, HIGH); // Ligar o LED
  delay(100); // Manter o LED aceso por 100 ms
  digitalWrite(LED_PIN, LOW); // Desligar o LED

  // Aguardar 5 segundos antes de ler novamente
  delay(5000);
}

uint16_t calculateCRC(uint8_t address, uint16_t register_address, uint16_t register_value) {
  // Esta função calcula o CRC (Cyclic Redundancy Check) para Modbus RTU

  uint16_t crc = 0xFFFF;
  uint8_t data[6];

  // Converter os parâmetros para bytes
  data[0] = address;
  data[1] = 0x06; // Código de função para escrita única
  data[2] = (register_address >> 8) & 0xFF; // Endereço do registro de alta ordem
  data[3] = register_address & 0xFF; // Endereço do registro de baixa ordem
  data[4] = (register_value >> 8) & 0xFF; // Valor do registro de alta ordem
  data[5] = register_value & 0xFF; // Valor do registro de baixa ordem

  // Calcular CRC
  for (uint8_t i = 0; i < 6; ++i) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; ++j) {
      if (crc & 0x0001) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }

  return crc;
}
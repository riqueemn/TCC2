#define ID_INVERSOR 1
#define TOTAL_DE_REG 2

#define REG_CTRL 682
#define REG_VELOCIDADE 683

#define REG_V 002
#define REG_A 003

uint16_t REG_ADDR_READ[2] = {
    REG_V,
    REG_A,
};

uint16_t REG_ADDR_WRITE[2] = {
    REG_CTRL,
    REG_VELOCIDADE,
};

float DATA_MEDIDA[TOTAL_DE_REG];

#define ID_INVERSOR 1
#define TOTAL_DE_REG 2

#define REG_CTRL 682
#define REG_VELOCIDADE 683

#define REG_CONFIG_220 220
#define REG_CONFIG_222 222
#define REG_CONFIG_226 226
#define REG_CONFIG_227 227
#define REG_CONFIG_308 308
#define REG_CONFIG_310 310
#define REG_CONFIG_263 263
#define REG_CONFIG_264 264
#define REG_CONFIG_265 265
#define REG_CONFIG_266 266

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

uint16_t REG_ADDR_CONFIG[10] = {
    REG_CONFIG_220,
    REG_CONFIG_222,
    REG_CONFIG_226,
    REG_CONFIG_227,
    REG_CONFIG_308,
    REG_CONFIG_310,
    REG_CONFIG_263,
    REG_CONFIG_264,
    REG_CONFIG_265,
    REG_CONFIG_266
};

uint16_t REG_ADDR_CONFIG_VALUE_1[10] = {
    1,
    9,
    5,
    2,
    1,
    1,
    1,
    8,
    0,
    0
};

uint16_t REG_ADDR_CONFIG_VALUE_2[10] = {
    1,
    7,
    4,
    1,
    1,
    1,
    1,
    11,
    12,
    5
};

float DATA_MEDIDA[TOTAL_DE_REG];

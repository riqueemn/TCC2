#define ID_METER 1
#define TOTAL_OF_REG 15

#define REG_CTRL 682

#define REG_VELOCITY 683

#define REG_VAB 3019
#define REG_VBC 3019
#define REG_VCA 3019
#define REG_VAVG 3019

#define REG_IA 3019
#define REG_IB 3019
#define REG_IC 3019
#define REG_IAVG 3019

#define REG_WT 3019
#define REG_QT 3019
#define REG_ST 3019

#define REG_FPT 3019

#define REG_THDVAB 3019
#define REG_THDVBC 3019
#define REG_THDVCA 3019

uint16_t REG_ADDR_READ[15] = {
    REG_VAB,
    REG_VBC,
    REG_VCA,
    REG_VAVG,
    REG_IA,
    REG_IB,
    REG_IC,
    REG_IAVG,
    REG_WT,
    REG_QT,
    REG_ST,
    REG_FPT,
    REG_THDVAB,
    REG_THDVBC,
    REG_THDVCA,
};

uint16_t REG_ADDR_WRITE[2] = {
    REG_CTRL,
    REG_VELOCITY,
};

float DATA_METER[TOTAL_OF_REG];

#define ID_meter  1
#define Total_of_Reg_Meter  42
#define Total_of_Reg_Energy 12
#define Total_of_Reg 54

// tensões 
#define Reg_VAB   3019 //
#define Reg_VBC   3021  
#define Reg_VCA   3023 // 
#define Reg_VAVG  3025 // Tensão Media das Fases
// correntes 
#define Reg_IA    2999 // Corrente da linha A
#define Reg_IB    3001 // Corrente da linha B 
#define Reg_IC    3003 // Corrente da linha C 
#define Reg_IAVG  3009 // Corrente Media

// potências totais
#define Reg_WT    3059 // Potencia Ativa   
#define Reg_QT    3067 // Potencia Reativa
#define Reg_ST    3075 // Potencia Aparente

#define Reg_FPT   3083 // Fator de Potência
// THD
#define Reg_THDVAB  21321
#define Reg_THDVBC  21323
#define Reg_THDVCA  21325

// THD harmônicas 1 Tensão de linha (1° até a 9°)

#define Reg_THDVAB1 21711
#define Reg_THDVAB2 21717
#define Reg_THDVAB3 21723
#define Reg_THDVAB4 21729
#define Reg_THDVAB5 21735
#define Reg_THDVAB6 21741
#define Reg_THDVAB7 21747
#define Reg_THDVAB8 21753
#define Reg_THDVAB9 21759
// THD harmônicas 2 Tensão de linha (1° até a 9°)

#define Reg_THDVBC1 22099
#define Reg_THDVBC2 22105
#define Reg_THDVBC3 22111
#define Reg_THDVBC4 22117
#define Reg_THDVBC5 22123
#define Reg_THDVBC6 22129
#define Reg_THDVBC7 22135
#define Reg_THDVBC8 22141
#define Reg_THDVBC9 22147

// THD harmônicas 3 Tensão de linha (1° até a 9°)

#define Reg_THDVCA1 22487
#define Reg_THDVCA2 22493
#define Reg_THDVCA3 22499
#define Reg_THDVCA4 22505
#define Reg_THDVCA5 22511
#define Reg_THDVCA6 22517
#define Reg_THDVCA7 22523
#define Reg_THDVCA8 22529
#define Reg_THDVCA9 22535


// Registros de Energia Admissíveis
// Energia Ativa
#define Regs_AEDI  3203
#define Regs_AEDO  3207
#define Regs_AEDRM 3211
#define Regs_AEDRm 3215
// Energia Reativa
#define Regs_QEDI  3219
#define Regs_QEDO  3223
#define Regs_QEDRM 3227
#define Regs_QEDRm 3231
// Energia Aparente 
#define Regs_SEDI  3235
#define Regs_SEDO  3239
#define Regs_SEDRM 3243
#define Regs_SEDRm 3247
uint16_t Reg_addr[42] = {
  Reg_VAB,
  Reg_VBC,
  Reg_VCA,
  Reg_VAVG,
  Reg_IA,
  Reg_IB,
  Reg_IC,
  Reg_IAVG,
  Reg_WT,
  Reg_QT,
  Reg_ST,
  Reg_FPT,
  Reg_THDVAB,
  Reg_THDVBC,
  Reg_THDVCA,

  Reg_THDVAB1,
  Reg_THDVAB2,
  Reg_THDVAB3,
  Reg_THDVAB4,
  Reg_THDVAB5,
  Reg_THDVAB6,
  Reg_THDVAB7,
  Reg_THDVAB8,
  Reg_THDVAB9,

  Reg_THDVBC1,
  Reg_THDVBC2,
  Reg_THDVBC3,
  Reg_THDVBC4,
  Reg_THDVBC5,
  Reg_THDVBC6,
  Reg_THDVBC7,
  Reg_THDVBC8,
  Reg_THDVBC9,
  
  Reg_THDVCA1,
  Reg_THDVCA2,
  Reg_THDVCA3,
  Reg_THDVCA4,
  Reg_THDVCA5,
  Reg_THDVCA6,
  Reg_THDVCA7,
  Reg_THDVCA8,
  Reg_THDVCA9,
 
};

uint16_t Regs_addr[12] = {
  Regs_AEDI,
  Regs_AEDO,
  Regs_AEDRM,
  Regs_AEDRm,
  Regs_QEDI,
  Regs_QEDO,
  Regs_QEDRM,
  Regs_QEDRm,
  Regs_SEDI,
  Regs_SEDO,
  Regs_SEDRM,
  Regs_SEDRm,
  
};

float DATA_METER [42] ;
uint32_t DATA_ENERGY[12]; 

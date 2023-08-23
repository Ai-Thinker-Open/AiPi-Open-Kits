#ifndef INC_UNI_CMD_CODE_H_
#define INC_UNI_CMD_CODE_H_

typedef struct {
  uni_u8      cmd_code; /* cmd code fro send base on SUCP */
  const char  *cmd_str; /* action string on UDP */;
} cmd_code_map_t;

const cmd_code_map_t g_cmd_code_arry[] = {
  {0x0, "volumeUpUni"},
  {0x1, "volumeDownUni"},
  {0x2, "wakeup_uni"},
  {0x3, "exitUni"},
  {0x4, "LearnMatchAC"},
  {0x5, "NightLampOn"},
  {0x6, "NightLampOff"},
  {0x7, "ACTurnOn"},
  {0x8, "ACTurnOff"},
  {0x9, "ACModeAuto"},
  {0xa, "ACModeCool"},
  {0xb, "ACModeHeat"},
  {0xc, "ACModeDry"},
  {0xd, "ACModeFan"},
  {0xe, "ACWindAuto"},
  {0xf, "ACWindLow"},
  {0x10, "ACWindMid"},
  {0x11, "ACWindHigh"},
  {0x12, "ACWindInc"},
  {0x13, "ACWindDec"},
  {0x14, "ACTmp16"},
  {0x15, "ACTmp17"},
  {0x16, "ACTmp18"},
  {0x17, "ACTmp19"},
  {0x18, "ACTmp20"},
  {0x19, "ACTmp21"},
  {0x1a, "ACTmp22"},
  {0x1b, "ACTmp23"},
  {0x1c, "ACTmp24"},
  {0x1d, "ACTmp25"},
  {0x1e, "ACTmp26"},
  {0x1f, "ACTmp27"},
  {0x20, "ACTmp28"},
  {0x21, "ACTmp29"},
  {0x22, "ACTmp30"},
  {0x23, "RubbishWord"},
  {0x24, "ACTmpInc"},
  {0x25, "ACTmpDec"},
  {0x26, "ACSweptOn"},
  {0x27, "ACSweptOff"},
  {0x28, "ACSweptVert"},
  {0x29, "ACSweptCross"},
  {0x2a, "ACScanStart"},
  {0x2b, "ACScanStop"},
  {0x2c, "SetRestore"},
};

#endif

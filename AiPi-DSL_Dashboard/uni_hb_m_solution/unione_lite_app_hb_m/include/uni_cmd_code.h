#ifndef INC_UNI_CMD_CODE_H_
#define INC_UNI_CMD_CODE_H_

typedef struct {
  uni_u8      cmd_code; /* cmd code fro send base on SUCP */
  const char  *cmd_str; /* action string on UDP */;
} cmd_code_map_t;

const cmd_code_map_t g_cmd_code_arry[] = {
  {0x0, "wakeup_uni"},
  {0x1, "exitUni"},
  {0x2, "openL"},
  {0x3, "closeL"},
  {0x4, "checkWaether"},
  {0x5, "connectMQTT"},
  {0x6, "connectWiFi"},
  {0x7, "openAPP1"},
  {0x8, "openAPP2"},
  {0x9, "openAPP3"},
  {0xa, "openAPP4"},
  {0xb, "openAPP5"},
  {0xc, "openAPP6"},
  {0xd, "status"},
  {0xe, "wifiScan"},
};

#endif

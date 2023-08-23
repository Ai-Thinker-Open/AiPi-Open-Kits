#ifndef INC_UNI_CMD_CODE_H_
#define INC_UNI_CMD_CODE_H_

typedef struct {
  uni_u8      cmd_code; /* cmd code fro send base on SUCP */
  const char  *cmd_str; /* action string on UDP */;
} cmd_code_map_t;

const cmd_code_map_t g_cmd_code_arry[] = {
  {0x0, "wakeup_uni"},
  {0x1, "exitUni"},
  {0x2, "TurnOn"},
  {0x3, "TurnOff"},
  {0x4, "tmpadd"},
  {0x5, "tmpsub"},
  {0x6, "wind"},
  {0x7, "mode"},
};

#endif

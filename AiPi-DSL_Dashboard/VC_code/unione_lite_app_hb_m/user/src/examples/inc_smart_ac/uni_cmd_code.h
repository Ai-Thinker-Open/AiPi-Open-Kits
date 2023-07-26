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
  {0x4, "ac_power_on"},
  {0x5, "ac_power_off"},
  {0x6, "ac_speed_1"},
  {0x7, "ac_speed_2"},
  {0x8, "ac_speed_3"},
  {0x9, "ac_mode_nor"},
  {0xa, "ac_mode_nat"},
  {0xb, "ac_mode_sleep"},
  {0xc, "ac_speed_inc"},
  {0xd, "ac_speed_dec"},
  {0xe, "ac_timing_1hr"},
  {0xf, "ac_timing_2hr"},
  {0x10, "ac_timing_4hr"},
  {0x11, "ac_timing_8hr"},
  {0x12, "ac_shake_on"},
  {0x13, "ac_shake_off"},
  {0x14, "ac_timing_cancel"},
  {0x15, "ac_timing_inc"},
  {0x16, "ac_timing_dec"},
};

#endif

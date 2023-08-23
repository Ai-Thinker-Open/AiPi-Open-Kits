#ifndef INC_UNI_NLU_CONTENT_H_
#define INC_UNI_NLU_CONTENT_H_

typedef struct {
  uni_u32 key_word_hash_code; /* 存放识别词汇对应的hashcode */
  uni_u8  nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */
  char    *hash_collision_orginal_str /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */;
} uni_nlu_content_mapping_t;

enum {
  eCMD_wakeup_uni,
  eCMD_exitUni,
  eCMD_TurnOn,
  eCMD_TurnOff,
  eCMD_tmpadd,
  eCMD_tmpsub,
  eCMD_wind,
  eCMD_mode,
};

const char* const g_nlu_content_str[] = {
[eCMD_wakeup_uni] = "{\"asr\":\"小安小安\",\"cmd\":\"wakeup_uni\",\"pcm\":\"[103, 104, 105]\"}",
[eCMD_exitUni] = "{\"asr\":\"退下\",\"cmd\":\"exitUni\",\"pcm\":\"[106]\"}",
[eCMD_TurnOn] = "{\"asr\":\"打开空调\",\"cmd\":\"TurnOn\",\"pcm\":\"[108]\"}",
[eCMD_TurnOff] = "{\"asr\":\"关闭空调\",\"cmd\":\"TurnOff\",\"pcm\":\"[109]\"}",
[eCMD_tmpadd] = "{\"asr\":\"温度升高\",\"cmd\":\"tmpadd\",\"pcm\":\"[110]\"}",
[eCMD_tmpsub] = "{\"asr\":\"温度降低\",\"cmd\":\"tmpsub\",\"pcm\":\"[111]\"}",
[eCMD_wind] = "{\"asr\":\"风速模式\",\"cmd\":\"wind\",\"pcm\":\"[112]\"}",
[eCMD_mode] = "{\"asr\":\"更改模式\",\"cmd\":\"mode\",\"pcm\":\"[113]\"}",
};

/*TODO perf sort by hashcode O(logN), now version O(N)*/
const uni_nlu_content_mapping_t g_nlu_content_mapping[] = {
  {3495814520U/*小安小安*/, eCMD_wakeup_uni, NULL},
  {2497873774U/*退下*/, eCMD_exitUni, NULL},
  {3484489468U/*打开空调*/, eCMD_TurnOn, NULL},
  {2543416495U/*关闭空调*/, eCMD_TurnOff, NULL},
  {693937137U/*温度升高*/, eCMD_tmpadd, NULL},
  {819710482U/*温度降低*/, eCMD_tmpsub, NULL},
  {2666464909U/*风速模式*/, eCMD_wind, NULL},
  {3821503493U/*更改模式*/, eCMD_mode, NULL},
};

#endif

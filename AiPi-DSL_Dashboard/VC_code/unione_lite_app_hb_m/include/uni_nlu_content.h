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
  eCMD_openL,
  eCMD_closeL,
  eCMD_checkWaether,
  eCMD_connectMQTT,
  eCMD_connectWiFi,
  eCMD_openAPP1,
  eCMD_openAPP2,
  eCMD_openAPP3,
  eCMD_openAPP4,
  eCMD_openAPP5,
  eCMD_openAPP6,
  eCMD_status,
  eCMD_wifiScan,
};

const char* const g_nlu_content_str[] = {
[eCMD_wakeup_uni] = "{\"asr\":\"你好小安\",\"cmd\":\"wakeup_uni\",\"pcm\":\"[103, 104, 105]\"}",
[eCMD_exitUni] = "{\"asr\":\"退下\",\"cmd\":\"exitUni\",\"pcm\":\"[106]\"}",
[eCMD_openL] = "{\"asr\":\"帮我开灯\",\"cmd\":\"openL\",\"pcm\":\"[108]\"}",
[eCMD_closeL] = "{\"asr\":\"帮我关灯\",\"cmd\":\"closeL\",\"pcm\":\"[109]\"}",
[eCMD_checkWaether] = "{\"asr\":\"天气情况\",\"cmd\":\"checkWaether\",\"pcm\":\"[110, 111]\"}",
[eCMD_connectMQTT] = "{\"asr\":\"连接服务器\",\"cmd\":\"connectMQTT\",\"pcm\":\"[112, 113]\"}",
[eCMD_connectWiFi] = "{\"asr\":\"连接路由\",\"cmd\":\"connectWiFi\",\"pcm\":\"[112, 114]\"}",
[eCMD_openAPP1] = "{\"asr\":\"打开薇哎斯抠得\",\"cmd\":\"openAPP1\",\"pcm\":\"[115, 116]\"}",
[eCMD_openAPP2] = "{\"asr\":\"打开百度网盘\",\"cmd\":\"openAPP2\",\"pcm\":\"[115]\"}",
[eCMD_openAPP3] = "{\"asr\":\"打开谷歌浏览器\",\"cmd\":\"openAPP3\",\"pcm\":\"[115]\"}",
[eCMD_openAPP4] = "{\"asr\":\"打开串口助手\",\"cmd\":\"openAPP4\",\"pcm\":\"[115]\"}",
[eCMD_openAPP5] = "{\"asr\":\"打开录制软件\",\"cmd\":\"openAPP5\",\"pcm\":\"[115]\"}",
[eCMD_openAPP6] = "{\"asr\":\"打开嘉立创\",\"cmd\":\"openAPP6\",\"pcm\":\"[115]\"}",
[eCMD_status] = "{\"asr\":\"检查\",\"cmd\":\"status\",\"pcm\":\"[117, 118, 119, 120, 121, 122]\"}",
[eCMD_wifiScan] = "{\"asr\":\"搜索路由\",\"cmd\":\"wifiScan\",\"pcm\":\"[123, 124]\"}",
};

/*TODO perf sort by hashcode O(logN), now version O(N)*/
const uni_nlu_content_mapping_t g_nlu_content_mapping[] = {
  {2835565650U/*你好小安*/, eCMD_wakeup_uni, NULL},
  {3495814520U/*小安小安*/, eCMD_wakeup_uni, NULL},
  {2497873774U/*退下*/, eCMD_exitUni, NULL},
  {3709081824U/*帮我开灯*/, eCMD_openL, NULL},
  {3196940412U/*打开灯*/, eCMD_openL, NULL},
  {2438769644U/*开灯*/, eCMD_openL, NULL},
  {3093277224U/*把灯打开*/, eCMD_openL, NULL},
  {3659807510U/*帮我关灯*/, eCMD_closeL, NULL},
  {745154473U/*关闭灯*/, eCMD_closeL, NULL},
  {3061906845U/*把灯关掉*/, eCMD_closeL, NULL},
  {2389495330U/*关灯*/, eCMD_closeL, NULL},
  {1312586924U/*天气情况*/, eCMD_checkWaether, NULL},
  {1102865295U/*查询天气*/, eCMD_checkWaether, NULL},
  {376280926U/*天气怎么样*/, eCMD_checkWaether, NULL},
  {1339402959U/*天气查询*/, eCMD_checkWaether, NULL},
  {1344808313U/*连接服务器*/, eCMD_connectMQTT, NULL},
  {1483479035U/*连接远程*/, eCMD_connectMQTT, NULL},
  {1955111065U/*开始连接服务器*/, eCMD_connectMQTT, NULL},
  {1476656314U/*连接路由*/, eCMD_connectWiFi, NULL},
  {2013429722U/*连接路由器*/, eCMD_connectWiFi, NULL},
  {1452675747U/*连接网络*/, eCMD_connectWiFi, NULL},
  {3120065891U/*网络连接*/, eCMD_connectWiFi, NULL},
  {2031540385U/*打开薇哎斯抠得*/, eCMD_openAPP1, NULL},
  {801847557U/*打开编码软件*/, eCMD_openAPP1, NULL},
  {3921548943U/*打开代码编辑*/, eCMD_openAPP1, NULL},
  {131744263U/*打开百度网盘*/, eCMD_openAPP2, NULL},
  {1560013340U/*打开谷歌浏览器*/, eCMD_openAPP3, NULL},
  {3519491887U/*打开谷歌*/, eCMD_openAPP3, NULL},
  {3091615794U/*打开浏览器*/, eCMD_openAPP3, NULL},
  {1834152344U/*打开串口助手*/, eCMD_openAPP4, NULL},
  {3473432948U/*打开录制软件*/, eCMD_openAPP5, NULL},
  {2768640941U/*打开欧闭埃斯*/, eCMD_openAPP5, NULL},
  {46081273U/*打开欧币斯*/, eCMD_openAPP5, NULL},
  {547309774U/*打开嘉立创*/, eCMD_openAPP6, NULL},
  {3484932234U/*打开立创*/, eCMD_openAPP6, NULL},
  {45678104U/*打开立创伊滴诶*/, eCMD_openAPP6, NULL},
  {2444310729U/*检查*/, eCMD_status, NULL},
  {1796170919U/*搜索路由*/, eCMD_wifiScan, NULL},
  {1772190352U/*搜索网络*/, eCMD_wifiScan, NULL},
};

#endif

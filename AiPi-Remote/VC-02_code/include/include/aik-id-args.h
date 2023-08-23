/*
 * Copyright 2019 Unisound AI Technology Co., Ltd.
 * Author: Yuan Shi Feng
 * All Rights Reserved.
 */
#ifndef INCLUDE_AIK_ID_ARGS_H_
#define INCLUDE_AIK_ID_ARGS_H_

#ifndef _WIN32
#define AIK_EXPORT __attribute__((visibility("default")))
#ifdef __cplusplus
extern "C" {
#endif
#else
#ifdef DLL_EXPORT
#define AIK_EXPORT extern "C" __declspec(dllexport)
#else
#define AIK_EXPORT extern "C" __declspec(dllimport)
#endif
#endif

/* Args of AIK_ID_SSP_SET_WAKEUP_ARGS */
typedef struct {
  float start_sec;  // kws start time
  float end_sec;    // kws end time
} AikSspWkrArgs;

/*
 * 学习模式获取临时grammar参数结构体 @ref AIK_ID_KWS_LP_GET_STUDY_GRAMMAR
 */
typedef struct {
  int8_t* tmp_grammar;
  int32_t max_grammar_size;
  int32_t* tmp_grammar_size;
} UalAikKwsGetTmpGrammarArgs;

/*
 * 学习模式比较临时grammar相似度参数结构体
 * @ref AIK_ID_KWS_LP_GET_GRAMMAR_COMPARE
 */
typedef struct {
  int8_t* tmp_grammar1;
  int8_t* tmp_grammar2;
  int32_t* score;
} UalAikKwsGetGrammarSimilarityArgs;

/*
 * 学习模式合并grammar参数结构体 @ref AIK_ID_KWS_LP_GET_GRAMMAR_MERGE
 */
typedef struct {
  int8_t* base_grammar;
  int8_t* tmp_grammar;
  int8_t* merged_grammar;
  int32_t max_merged_grammar_size;
  int32_t* merged_grammar_size;
  uint8_t is_wakeup;
} UalAikKwsGetGrammarMergedArgs;

#ifndef _WIN32
#ifdef __cplusplus
}
#endif
#endif

#endif  // INCLUDE_AIK_ID_ARGS_H_

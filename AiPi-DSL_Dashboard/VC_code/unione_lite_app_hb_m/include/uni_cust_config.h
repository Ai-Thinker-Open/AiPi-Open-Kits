#ifndef __UNI_CUST_CONFIG_H__
#define __UNI_CUST_CONFIG_H__

#define UNI_ASR_TIMEOUT           10    //ASR timeout Xs

#define UNI_MIC_ARRY_TYPE_LINER   0
#define UNI_MIC_ARRY_TYPE_CIRCLE  1
#define ACOUSTIC_MODEL_FIELD_NEAR 0
#define ACOUSTIC_MODEL_FIELD_FAR  1

/*---MIC config---*/
#define AACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_FAR
#define UNI_MIC_ARRY_NUM          1

#define LOCAL_TONE_MAX_VOLUME      "[101]"
#define LOCAL_TONE_MIN_VOLUME      "[102]"
#define DEFAULT_PCM_WAKEUP        "[103, 104, 105]"
#define DEFAULT_PCM_ASR_TIMEOUT   "[106]"
#define DEFAULT_PCM_SLEEP         "[106]"
#define DEFAULT_PCM_WELCOME       "[107]"

#endif /*__UNI_CUST_CONFIG_H__*/

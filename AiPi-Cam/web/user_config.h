#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__
#include "wm_include.h"
#include <ctype.h>
#include "wm_psram.h"
#include "libcamera.h"
#include "libjpeg.h"
#include "mlwip_https.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define USER_DEBUG		0
#if USER_DEBUG
#define USER_PRINT printf
#else
#define USER_PRINT(fmt, ...)
#endif

#define USER_QUEUE_SIZE				16		// �û���Ϣ���е�size
#define USER_TASK_SIZE					1024	// �û�����ջ��size
#define USER_TASK_PRO					50		// �û��������ȼ�
#define MSG_NET_UP						0x01	// �����ɹ���Ϣ
#define MSG_NET_DOWN					0x02	// ������Ϣ
#define MSG_STA_UP						0x03	
#define MSG_STA_DOWN					0x04	
#define MSG_SK_CAP						0x05
#define MSG_QVGA_DST1					0x06
#define MSG_QVGA_DST2					0x07
#define MSG_VGA_DST1					0x08
#define MSG_VGA_DST2					0x09

#define MSG_KEY_1						0x10			
#define MSG_KEY_2						0x11
#define MSG_LKEY_1						0x12			
#define MSG_LKEY_2						0x13

// �̼��汾
#define USER_SSID						"Ai-WB1-CAM"
#define USER_KEY						""			

// ���涨��ѷ���
#define USER_MALLOC					tls_mem_alloc
#define USER_FREE						tls_mem_free

// min 2ms 
#define USER_DELAY_TICK(time) 			tls_os_time_delay(time)
#define USER_TIME_GET					tls_os_get_time	

// extern void tls_sys_reset(void);

#endif

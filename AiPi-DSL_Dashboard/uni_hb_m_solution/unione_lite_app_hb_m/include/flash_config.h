#ifndef __FLASH_CONFIG_H__
#define __FLASH_CONFIG_H__

//���ڶ���Flash ����ʹ�����
//Ĭ��flashΪ2M Byte�������С�б仯ע������߽�

#define	CODE_ADDR				0x0
#define CONST_DATA_ADDR    		0x150000//0x120000
#define FLASHFS_ADDR			0x1B0000//ȱʡ���ò�����
#define BP_DATA_ADDR     		0x1D0000
#define BT_DATA_ADDR     		0x1D8000
#define USER_DATA_ADDR     		0x1DC000//0x1D8000
#define AUDIO_EFFECT_ADDR  		0x1e0000

/**��ƵSDK�汾�ţ�V1.0.0**/
/*1A��оƬ26��01����汾�ţ� 00��С�汾�ţ� 00���û��޶��ţ����û��趨���ɽ�ϲ����ţ���ʵ�ʴ洢�ֽ���1A 01 00 00 ����������sdk�汾*/
/*����flash_bootʱ����flashboot����usercode��boot��������code����(��0xB8��0xB8+0x10000)����ֵ�᲻ͬ��ǰ����burner��¼ʱ�汾��������mva�汾���ע*/
#define	 CFG_SDK_VER_CHIPID			(0x1A)
#define  CFG_SDK_MAJOR_VERSION		(0x2)
#define  CFG_SDK_MINOR_VERSION		(0x0)
#define  CFG_SDK_PATCH_VERSION		(0x1)

#endif


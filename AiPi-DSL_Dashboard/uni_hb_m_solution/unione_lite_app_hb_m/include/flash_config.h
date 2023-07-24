#ifndef __FLASH_CONFIG_H__
#define __FLASH_CONFIG_H__

//用于定义Flash 区域使用情况
//默认flash为2M Byte，如果大小有变化注意调整边界

#define	CODE_ADDR				0x0
#define CONST_DATA_ADDR    		0x150000//0x120000
#define FLASHFS_ADDR			0x1B0000//缺省配置不启用
#define BP_DATA_ADDR     		0x1D0000
#define BT_DATA_ADDR     		0x1D8000
#define USER_DATA_ADDR     		0x1DC000//0x1D8000
#define AUDIO_EFFECT_ADDR  		0x1e0000

/**音频SDK版本号：V1.0.0**/
/*1A：芯片26，01：大版本号， 00：小版本号， 00：用户修订号（由用户设定，可结合补丁号）；实际存储字节序：1A 01 00 00 ，略区别于sdk版本*/
/*开启flash_boot时，用flashboot升级usercode后，boot明码区和code明码(如0xB8和0xB8+0x10000)两个值会不同，前者是burner烧录时版本，后者是mva版本需关注*/
#define	 CFG_SDK_VER_CHIPID			(0x1A)
#define  CFG_SDK_MAJOR_VERSION		(0x2)
#define  CFG_SDK_MINOR_VERSION		(0x0)
#define  CFG_SDK_PATCH_VERSION		(0x1)

#endif


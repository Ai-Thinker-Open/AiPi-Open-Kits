// SPDX-License-Identifier: MIT
// Copyright 2023 NXP
/*
 * lv_conf_ext.h for custom lvconf file.
 * Created on: Feb 8, 2023
 * example : 
 *	#undef LV_FONT_FMT_TXT_LARGE
 *  #define LV_FONT_FMT_TXT_LARGE 1
 */
 
#ifndef LV_CONF_EXT_H
#define LV_CONF_EXT_H


/* common code  begin  */


/* common code end */


#if LV_USE_GUIDER_SIMULATOR
/* code for simulator begin  */


/* code for simulator end */
#else
/* code for board begin */


/* code for board end */	
#endif



#endif  /* LV_CONF_EXT_H */	
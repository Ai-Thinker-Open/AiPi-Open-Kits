#ifndef __LIBJPEG_H__
#define __LIBJPEG_H__

#include <stdint.h>


/**
 * @brief           This function is used to convert YUV422 to JPEG
 *
 * @param      	    data    		The address of the YUV422 image
 *					image_width		The width of the YUV422 image
 *					image_height	The height of the YUV433 image
 *					dest			The address of the JPEG image
 *					dest_buf_len	The size of the available space in the destination buffer
 *					quality			compression ratio 0~ 100
 *
 * @retval          timer's	current  value
 *
 * @note            none
 */

int yuv422_to_jpeg(unsigned char *data, int image_width, int image_height, uint8_t *dest, uint32_t dest_buf_len, int quality);

#endif


#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "wm_pwm.h"
#include "wm_gpio_afsel.h"
#include <string.h>
#include "wm_psram.h"
#include "wm_hspi.h"

#if 0
#define CAMERA_PWDN		WM_IO_PB_08
#define CAMERA_MCLK		WM_IO_PB_26
``
#define CAMERA_SPI_CS		WM_IO_PB_14
#define CAMERA_SPI_CLK		WM_IO_PB_12
#define CAMERA_SPI_D0	WM_IO_PB_15
#define CAMERA_SPI_D1	WM_IO_PB_16

#define I2C_SCL				WM_IO_PB_07
#define I2C_SDA				WM_IO_PB_06
#else
#define CAMERA_PWDN			WM_IO_PA_07
#define CAMERA_MCLK			WM_IO_PA_01

#define CAMERA_SPI_CS		WM_IO_PB_09
#define CAMERA_SPI_CLK		WM_IO_PB_06
#define CAMERA_SPI_D0		WM_IO_PB_10
#define CAMERA_SPI_D1		WM_IO_PB_11

#define I2C_SCL				WM_IO_PB_07
#define I2C_SDA				WM_IO_PB_08

#endif

#define CAMERA_ADDR		0x42

typedef enum
{
	CAP_IMG_VGA_YUV422 = 1, // 640 * 480 YUV422
	CAP_IMG_QVGA_YUV422,	 // 320 * 240 YUV422
	CAP_IMG_VGA_Y,			// 640 * 480 only y
	CAP_IMG_QVGA_Y,			// 320 * 240 only y
	CAP_IMG_VGA_RGB565,		// 640 * 480 RGB565
	CAP_IMG_QVGA_RGB565,	// 320 * 240 RGB565
}resolution_type;

typedef struct
{
	enum tls_io_name scl;
	enum tls_io_name sda;
	enum tls_io_name pdwn;
	enum tls_io_name mclk;
	enum tls_io_name cs;
	enum tls_io_name clk;
	enum tls_io_name d0;
	enum tls_io_name d1;
	uint8_t camera_addr;
}CameraPinDef;

/**
 * @brief          This function is used to initialize the camera
 *
 * @param      	   type    This parameter can be a value of @fef resolution_type
 *
 *				   pin	   io required for camera initialization
 *
 * @return         None
 *
 * @note           None
 */
void CameraInit(resolution_type type, CameraPinDef pin);

/**
 * @brief          This function is used to register image processing callback function.
 *				   When the image capture is completed, the callback will be called to
 *				   process the image.
 *
 * @param      	   cb    The image processing callback
 *						 data	image storage address. Two image data buffers are set up in the
 *								library, and the addresses are 0x08000000 and 0x08025800/0x08096000,
 *								and the length is 320*240*2 or 640*480*2. So the starting address of
 *								the user in PSRAM should be 0x0804B000 or 0x0912c0000 depending on
 *								CAP_IMG_QVGA or CAP_IMG_VGA.
 *						 len	image data length, 320*240*2 or 640*480*2.
 *
 * @return         None
 *
 * @note           None
 */
void CameraDataProcessCbRegister(void(*Cb)(uint8_t *data, uint32_t len));

/**
 * @brief          This function is used to set the orientation of the image, 
 *					including flipping up, down, left, and right.
 *
 * @param      	   mirror_en    enable mirror
 *				   flip_en		enable flip
 *
 * @return         None
 *
 * @note           None
 */
void CameraSetMirrorFlip(uint8_t mirror_en, uint8_t flip_en);

#endif

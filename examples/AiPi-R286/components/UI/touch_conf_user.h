#ifndef _TOUCH_CONF_USER_H_
#define _TOUCH_CONF_USER_H_

/* spi interface
    TOUCH_SPI_XPT2046  // Not currently supported
*/

/* i2c interface
    TOUCH_I2C_FT6X36
    TOUCH_I2C_GT911
*/

/* Select Touch Type */
#define TOUCH_I2C_FT6X36

/* touch interface */
#define TOUCH_INTERFACE_SPI 1
#define TOUCH_INTERFACE_I2C 2

/* touch interface pin config */
#define TOUCH_I2C_SCL_PIN   GPIO_PIN_10
#define TOUCH_I2C_SDA_PIN   GPIO_PIN_11

/* now do not support */
#if 0
#define TOUCH_SPI_CS_PIN GPIO_PIN_6
#define TOUCH_SPI_SCLK_PIN
#define TOUCH_SPI_MOSI_PIN
#define TOUCH_SPI_MISO_PIN
#endif

#endif // _TOUCH_CONF_USER_H_

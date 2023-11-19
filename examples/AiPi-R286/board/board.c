#include "bflb_uart.h"
#include "bflb_gpio.h"
#include "bflb_clock.h"
#include "bflb_rtc.h"
#include "bflb_flash.h"
#include "bflb_acomp.h"
#include "board.h"
#include "bl616_tzc_sec.h"
#include "bl616_psram.h"
#include "bl616_glb.h"

#include "mem.h"

#define WB_4MB_PSRAM (1)

#ifdef CONFIG_BSP_SDH_SDCARD
#include "sdh_sdcard.h"
#endif

#if (0)
#define BOARD_DBG(a, ...) printf("[dbg]:" a, ##__VA_ARGS__)
#else
#define BOARD_DBG(a, ...)
#endif

extern void log_start(void);

extern uint32_t __HeapBase;
extern uint32_t __HeapLimit;
extern uint32_t __psram_heap_base;
extern uint32_t __psram_limit;

static struct bflb_device_s* uart0;

#if (defined(CONFIG_LUA) || defined(CONFIG_BFLOG) || defined(CONFIG_FATFS))
static struct bflb_device_s* rtc;
#endif

static void system_clock_init(void)
{
#if 1
    /* wifipll/audiopll */
    GLB_Power_On_XTAL_And_PLL_CLK(GLB_XTAL_40M, GLB_PLL_WIFIPLL | GLB_PLL_AUPLL);
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_TOP_WIFIPLL_320M);
#else
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_RC32M);
    GLB_Power_On_XTAL_And_PLL_CLK(GLB_XTAL_40M, GLB_PLL_WIFIPLL);
    GLB_Config_AUDIO_PLL_To_384M();
    GLB_Set_MCU_System_CLK(GLB_MCU_SYS_CLK_TOP_AUPLL_DIV1);
    GLB_Set_MCU_System_CLK_Div(0, 3);
#endif
    CPU_Set_MTimer_CLK(ENABLE, BL_MTIMER_SOURCE_CLOCK_MCU_XCLK, Clock_System_Clock_Get(BL_SYSTEM_CLOCK_XCLK) / 1000000 - 1);
}

static void peripheral_clock_init(void)
{
    PERIPHERAL_CLOCK_ADC_DAC_ENABLE();
    PERIPHERAL_CLOCK_SEC_ENABLE();
    PERIPHERAL_CLOCK_DMA0_ENABLE();
    PERIPHERAL_CLOCK_UART0_ENABLE();
    PERIPHERAL_CLOCK_UART1_ENABLE();
    PERIPHERAL_CLOCK_SPI0_ENABLE();
    PERIPHERAL_CLOCK_I2C0_ENABLE();
    PERIPHERAL_CLOCK_PWM0_ENABLE();
    PERIPHERAL_CLOCK_TIMER0_1_WDG_ENABLE();
    PERIPHERAL_CLOCK_IR_ENABLE();
    PERIPHERAL_CLOCK_I2S_ENABLE();
    PERIPHERAL_CLOCK_USB_ENABLE();
    PERIPHERAL_CLOCK_CAN_ENABLE();
    PERIPHERAL_CLOCK_AUDIO_ENABLE();

    GLB_Set_UART_CLK(ENABLE, HBN_UART_CLK_XCLK, 0);
    GLB_Set_SPI_CLK(ENABLE, GLB_SPI_CLK_MCU_MUXPLL_160M, 0);
    GLB_Set_DBI_CLK(ENABLE, GLB_SPI_CLK_MCU_MUXPLL_160M, 0);
    GLB_Set_I2C_CLK(ENABLE, GLB_I2C_CLK_XCLK, 0);
    GLB_Set_ADC_CLK(ENABLE, GLB_ADC_CLK_XCLK, 1);
    GLB_Set_DIG_CLK_Sel(GLB_DIG_CLK_XCLK);
    GLB_Set_DIG_512K_CLK(ENABLE, ENABLE, 0x4E);
    GLB_Set_PWM1_IO_Sel(GLB_PWM1_IO_SINGLE_END);
    GLB_Set_IR_CLK(ENABLE, GLB_IR_CLK_SRC_XCLK, 19);
    GLB_Set_CAM_CLK(ENABLE, GLB_CAM_CLK_WIFIPLL_96M, 3);

    GLB_Set_PKA_CLK_Sel(GLB_PKA_CLK_MCU_MUXPLL_160M);
#ifdef CONFIG_BSP_SDH_SDCARD
    PERIPHERAL_CLOCK_SDH_ENABLE();
    GLB_AHB_MCU_Software_Reset(GLB_AHB_MCU_SW_EXT_SDH);
#endif

    GLB_Set_USB_CLK_From_WIFIPLL(1);
    GLB_Swap_MCU_SPI_0_MOSI_With_MISO(0);
}

static void bflb_init_psram_gpio(void)
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");
    for (uint8_t i = 0; i < 12; i++) {
        bflb_gpio_init(gpio, (41 + i), GPIO_INPUT | GPIO_FLOAT | GPIO_SMT_EN | GPIO_DRV_0);
    }
}

static void psram_winbond_default_init(void)
{
    PSRAM_Ctrl_Cfg_Type default_psram_ctrl_cfg = {
        .vendor = PSRAM_CTRL_VENDOR_WINBOND,
        .ioMode = PSRAM_CTRL_X8_MODE,
        .size = PSRAM_SIZE_4MB,
        .dqs_delay = 0xfff0,
    };

    PSRAM_Winbond_Cfg_Type default_winbond_cfg = {
        .rst = DISABLE,
        .clockType = PSRAM_CLOCK_DIFF,
        .inputPowerDownMode = DISABLE,
        .hybridSleepMode = DISABLE,
        .linear_dis = ENABLE,
        .PASR = PSRAM_PARTIAL_REFRESH_FULL,
        .disDeepPowerDownMode = ENABLE,
        .fixedLatency = DISABLE,
        .brustLen = PSRAM_WINBOND_BURST_LENGTH_64_BYTES,
        .brustType = PSRAM_WRAPPED_BURST,
        .latency = PSRAM_WINBOND_6_CLOCKS_LATENCY,
        .driveStrength = PSRAM_WINBOND_DRIVE_STRENGTH_35_OHMS_FOR_4M_115_OHMS_FOR_8M,
    };

    PSram_Ctrl_Init(PSRAM0_ID, &default_psram_ctrl_cfg);
    // PSram_Ctrl_Winbond_Reset(PSRAM0_ID);
    PSram_Ctrl_Winbond_Write_Reg(PSRAM0_ID, PSRAM_WINBOND_REG_CR0, &default_winbond_cfg);
}

uint32_t board_psram_x8_init(void)
{
    uint16_t reg_read = 0;

    GLB_Set_PSRAMB_CLK_Sel(ENABLE, GLB_PSRAMB_EMI_WIFIPLL_320M, 0);

    bflb_init_psram_gpio();

    /* psram init*/
    psram_winbond_default_init();
    /* check psram work or not */
    PSram_Ctrl_Winbond_Read_Reg(PSRAM0_ID, PSRAM_WINBOND_REG_ID0, &reg_read);
    return reg_read;
}

void bl_show_log(void)
{
    BOARD_DBG("\r\n");
    BOARD_DBG("  ____               __  __      _       _       _     \r\n");
    BOARD_DBG(" |  _ \\             / _|/ _|    | |     | |     | |    \r\n");
    BOARD_DBG(" | |_) | ___  _   _| |_| |_ __ _| | ___ | | __ _| |__  \r\n");
    BOARD_DBG(" |  _ < / _ \\| | | |  _|  _/ _` | |/ _ \\| |/ _` | '_ \\ \r\n");
    BOARD_DBG(" | |_) | (_) | |_| | | | || (_| | | (_) | | (_| | |_) |\r\n");
    BOARD_DBG(" |____/ \\___/ \\__,_|_| |_| \\__,_|_|\\___/|_|\\__,_|_.__/ \r\n");
    BOARD_DBG("\r\n");
    BOARD_DBG("Build:%s,%s\r\n", __TIME__, __DATE__);
    BOARD_DBG("Copyright (c) 2022 Bouffalolab team\r\n");
}

void bl_show_flashinfo(void)
{
    spi_flash_cfg_type flashCfg;
    uint8_t* pFlashCfg = NULL;
    uint32_t flashCfgLen = 0;
    uint32_t flashJedecId = 0;

    flashJedecId = bflb_flash_get_jedec_id();
    bflb_flash_get_cfg(&pFlashCfg, &flashCfgLen);
    arch_memcpy((void*)&flashCfg, pFlashCfg, flashCfgLen);
    BOARD_DBG("=========== flash cfg ==============\r\n");
    BOARD_DBG("jedec id   0x%06X\r\n", flashJedecId);
    BOARD_DBG("mid            0x%02X\r\n", flashCfg.mid);
    BOARD_DBG("iomode         0x%02X\r\n", flashCfg.io_mode);
    BOARD_DBG("clk delay      0x%02X\r\n", flashCfg.clk_delay);
    BOARD_DBG("clk invert     0x%02X\r\n", flashCfg.clk_invert);
    BOARD_DBG("read reg cmd0  0x%02X\r\n", flashCfg.read_reg_cmd[0]);
    BOARD_DBG("read reg cmd1  0x%02X\r\n", flashCfg.read_reg_cmd[1]);
    BOARD_DBG("write reg cmd0 0x%02X\r\n", flashCfg.write_reg_cmd[0]);
    BOARD_DBG("write reg cmd1 0x%02X\r\n", flashCfg.write_reg_cmd[1]);
    BOARD_DBG("qe write len   0x%02X\r\n", flashCfg.qe_write_reg_len);
    BOARD_DBG("cread support  0x%02X\r\n", flashCfg.c_read_support);
    BOARD_DBG("cread code     0x%02X\r\n", flashCfg.c_read_mode);
    BOARD_DBG("burst wrap cmd 0x%02X\r\n", flashCfg.burst_wrap_cmd);
    BOARD_DBG("=====================================\r\n");
}

extern void bflb_uart_set_console(struct bflb_device_s* dev);

static void console_init()
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_uart_init(gpio, GPIO_PIN_21, GPIO_UART_FUNC_UART0_TX);
    bflb_gpio_uart_init(gpio, GPIO_PIN_22, GPIO_UART_FUNC_UART0_RX);

    struct bflb_uart_config_s cfg;
    cfg.baudrate = 2000000;
    cfg.data_bits = UART_DATA_BITS_8;
    cfg.stop_bits = UART_STOP_BITS_1;
    cfg.parity = UART_PARITY_NONE;
    cfg.flow_ctrl = 0;
    cfg.tx_fifo_threshold = 7;
    cfg.rx_fifo_threshold = 7;

    uart0 = bflb_device_get_by_name("uart0");

    bflb_uart_init(uart0, &cfg);
    bflb_uart_set_console(uart0);
}

void board_init(void)
{
    int ret = -1;
    uintptr_t flag;
    size_t heap_len;

    flag = bflb_irq_save();

    ret = bflb_flash_init();

    system_clock_init();
    peripheral_clock_init();
    bflb_irq_initialize();

    console_init();

#ifdef CONFIG_PSRAM
    board_psram_x8_init();
    Tzc_Sec_PSRAMB_Access_Release();

    heap_len = ((size_t)&__psram_limit - (size_t)&__psram_heap_base);
    pmem_init((void*)&__psram_heap_base, heap_len);
#endif

    heap_len = ((size_t)&__HeapLimit - (size_t)&__HeapBase);
    kmem_init((void*)&__HeapBase, heap_len);

    bl_show_log();
    if (ret != 0) {
        BOARD_DBG("flash init fail!!!\r\n");
    }
    bl_show_flashinfo();
#ifdef CONFIG_PSRAM
    BOARD_DBG("dynamic memory init success, ocram heap size = %d Kbyte, psram heap size = %d Kbyte\r\n",
           ((size_t)&__HeapLimit - (size_t)&__HeapBase) / 1024,
           ((size_t)&__psram_limit - (size_t)&__psram_heap_base) / 1024);
#else
    BOARD_DBG("dynamic memory init success, ocram heap size = %d Kbyte \r\n", ((size_t)&__HeapLimit - (size_t)&__HeapBase) / 1024);
#endif

    BOARD_DBG("sig1:%08x\r\n", BL_RD_REG(GLB_BASE, GLB_UART_CFG1));
    BOARD_DBG("sig2:%08x\r\n", BL_RD_REG(GLB_BASE, GLB_UART_CFG2));
    BOARD_DBG("cgen1:%08x\r\n", getreg32(BFLB_GLB_CGEN1_BASE));

    log_start();

#if (defined(CONFIG_LUA) || defined(CONFIG_BFLOG) || defined(CONFIG_FATFS))
    rtc = bflb_device_get_by_name("rtc");
#endif
#ifdef CONFIG_MBEDTLS
    extern void bflb_sec_mutex_init(void);
    bflb_sec_mutex_init();
#endif
    bflb_irq_restore(flag);
}

void board_i2c0_gpio_init()
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");
    /* I2C0_SDA */
    bflb_gpio_init(gpio, GPIO_PIN_10, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    /* I2C0_SCL */
    bflb_gpio_init(gpio, GPIO_PIN_11, GPIO_FUNC_I2C0 | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
}

void board_i2s_gpio_init()
{
    struct bflb_device_s* gpio;

    gpio = bflb_device_get_by_name("gpio");
    bflb_gpio_init(gpio, GPIO_PIN_13, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_14, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_15, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_20, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
    bflb_gpio_init(gpio, GPIO_PIN_23, GPIO_FUNC_I2S | GPIO_ALTERNATE | GPIO_PULLUP | GPIO_SMT_EN | GPIO_DRV_1);
}

#ifdef CONFIG_BFLOG
__attribute__((weak)) uint64_t bflog_clock(void)
{
    return bflb_mtimer_get_time_us();
}

__attribute__((weak)) uint32_t bflog_time(void)
{
    return BFLB_RTC_TIME2SEC(bflb_rtc_get_time(rtc));
}

__attribute__((weak)) char* bflog_thread(void)
{
    return "";
}
#endif

#ifdef CONFIG_LUA
__attribute__((weak)) clock_t luaport_clock(void)
{
    return (clock_t)bflb_mtimer_get_time_us();
}

__attribute__((weak)) time_t luaport_time(time_t* seconds)
{
    time_t t = (time_t)BFLB_RTC_TIME2SEC(bflb_rtc_get_time(rtc));
    if (seconds != NULL) {
        *seconds = t;
    }

    return t;
}
#endif

#ifdef CONFIG_FATFS
#include "bflb_timestamp.h"
__attribute__((weak)) uint32_t get_fattime(void)
{
    bflb_timestamp_t tm;

    bflb_timestamp_utc2time(BFLB_RTC_TIME2SEC(bflb_rtc_get_time(rtc)), &tm);

    return ((uint32_t)(tm.year - 1980) << 25) /* Year 2015 */
        | ((uint32_t)tm.mon << 21)         /* Month 1 */
        | ((uint32_t)tm.mday << 16)        /* Mday 1 */
        | ((uint32_t)tm.hour << 11)        /* Hour 0 */
        | ((uint32_t)tm.min << 5)          /* Min 0 */
        | ((uint32_t)tm.sec >> 1);         /* Sec 0 */
}
#endif

#ifdef CONFIG_SHELL
#include "shell.h"

static void reboot_cmd(int argc, char** argv)
{
    GLB_SW_POR_Reset();
}
SHELL_CMD_EXPORT_ALIAS(reboot_cmd, reboot, reboot);
#endif

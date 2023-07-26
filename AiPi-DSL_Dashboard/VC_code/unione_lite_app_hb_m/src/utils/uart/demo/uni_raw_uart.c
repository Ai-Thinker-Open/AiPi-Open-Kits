#include "uni_uart.h"
#include <stdio.h>

/**
 * 接收串口发送来的数据回调，数据处理逻辑完全交给客户控制
 * 数据是1字节1字节送出来的
 */
static void __receive_uart_data(char *buf, int len) {
}

int main() {
  /* 初始化串口 */
  UartConfig config;
  config.device = UNI_UART1;
  config.parity = UNI_PARITY_NONE;
  config.speed = UNI_B_115200;
  config.stop = UNI_ONE_STOP_BIT;
  config.data_bit = 8;
  
  if (0 != UartInitialize(&config, __receive_uart_data)) {
    printf("uart init failed\n");
  }

  while (1) {
    char buf[] = {1, 2, 3, 4, 5, 6};
    int len;
    /* 向串口写数据 */
    len = UartWrite(buf, sizeof(buf));
    if (len != sizeof(buf)) {
      printf("uart write failed, write len=%d, actual len=%d\n", sizeof(buf), len);
    }
    uni_msleep(2000);
  }
  return 0;
}

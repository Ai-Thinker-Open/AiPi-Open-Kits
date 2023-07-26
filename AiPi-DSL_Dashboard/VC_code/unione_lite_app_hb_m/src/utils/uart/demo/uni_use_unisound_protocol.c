#include "uni_uart.h"
#include "uni_communication.h"

typedef enum {
  UNI_SOUND = 0,
  GE_LI,
  MEI_DI
} CustomerType;

typedef enum {
  POWER_ON = 0,
  POWER_OFF,
} Command;

static void __recv_comm_packet(CommPacket *packet) {

}

static void __send_demo_without_payload_need_acked() {
  CommAttribute attr;
  attr.need_acked = true;
  attr.timeout_msec = 1000;
  /* 不带payload必须要对方主控ack的发送方式 */
  CommProtocolPacketAssembleAndSend(GE_LI, POWER_ON, NULL, 0, &attr);

  /* 不带payload且不需要对方主控ack */
  CommProtocolPacketAssembleAndSend(GE_LI, POWER_ON, NULL, 0, NULL);

  /* 带有payload且不需要对方主控ack */
  char payload[] = {1, 2, 3, 4};
  CommProtocolPacketAssembleAndSend(GE_LI, POWER_ON, payload, sizeof(payload), NULL);
}

int main() {
  UartConfig config;
  config.device = UNI_UART1;
  config.parity = UNI_PARITY_NONE;
  config.speed = UNI_B_115200;
  config.stop = UNI_ONE_STOP_BIT;
  config.data_bit = 8;

  UartInitialize(&config, CommProtocolReceiveUartData);
  CommProtocolInit(UartWrite, __recv_comm_packet);
  while (1) {
    /* 不带payload必须要对方主控ack的发送方式 */
    __send_demo_without_payload_need_acked();
    uni_msleep(2000);
  }
  return 0;
}

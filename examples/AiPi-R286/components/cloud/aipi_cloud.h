/**
 * @file aipi-cloud.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-11-15
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef AIPI_CLOUD_H
#define AIPI_CLOUD_H

#include "bluetooth.h"
#include "conn.h"
#include "conn_internal.h"
#include "btble_lib_api.h"
#include "bl616_glb.h"
#include "rfparam_adapter.h"

#include "aiio_os_port.h"
#include "aiio_common.h"
#include "aiio_wan_service.h"
#include "aiio_cloud_receive.h"

#include "ble_tp_svc.h"
#include "hci_driver.h"
#include "hci_core.h"

void aipi_aithinker_cloud_start(void);
void iot_connect_wifi(char* ssid, char* passwd);
#endif
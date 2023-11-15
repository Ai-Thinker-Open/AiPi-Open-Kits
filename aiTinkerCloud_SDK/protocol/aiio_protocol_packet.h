/**
 * @brief   Declare packing protocol interface of cloud communication
 * 
 * @file    aiio_protocol_packet.h
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note        This file is decribing about the protocol of cloud communication, mainly declare packing protocol interface.
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-15          <td>1.0.0            <td>zhuolm             <td> Packing protocol interface of cloud communication
 */
#ifndef __AIIO_PROTOCOL_PACKET_H_
#define __AIIO_PROTOCOL_PACKET_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "aiio_protocol_comm.h"




/**
 * @brief   Pack device infomation according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   device_info             Device information that needs to be packaged
 * @param[in]   wifi_info               Wifi information that needs to be packaged
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   token_status            This element is setting whether to report activation messages to the cloud, 
 *                                      if the element is setting to true, this function only report device infomation to cloud.
 *                                      if the element is setting to false, this function report device infomation to cloud with activaty infomation.
 * @param[in]   current_stamp           Time stamp of the local current time
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketDeviceInfoData(char *packet_data, uint32_t packet_data_len, aiio_device_info_t *device_info, aiio_wifi_info_t *wifi_info, char *msgMid, char *from, bool token_status, uint32_t current_stamp);


/**
 * @brief   Pack device offline infomation according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketOfflineData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp);

/**
 * @brief   Pack device online infomation according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketOnlineData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp);


/**
 * @brief   Pack ack data for responsing cloud configuration command according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   code                    The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketConfigAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code);



/**
 * @brief   Pack ack data for responsing cloud control command according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   code                    The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketControlAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code);


/**
 * @brief   Pack ack data for responsing cloud restore command according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   code                    The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketRestoreAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code);



/**
 * @brief   Pack ack data for responsing cloud reboot command according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   code                    The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketRebootAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code);



/**
 * @brief   Pack ack data for responsing cloud ota command according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   code                    The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketOtaAckeData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, int32_t code);


/**
 * @brief   Pack the progress of device report according to the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   ota_info                ota structure information to report progress during OTA, it is including error code, progress, ota version to be updated, OTA download progress and the ID of releasing OTA information. 
 *                                      Detailed description of the elements in the aiio_ota_info_t structure
 *                                      rate: When the element is equal to 0xffffffff, the interface does not report progress.
 *                                      otaStatus：If the element is empty, the progress status is not reported
 *                                      OtaVer：If the element is empty, the ota version to be update is not reported
 *                                      PubId：If the element is empty, the release message id is not reported
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketReportOtaRateData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, aiio_ota_info_t *ota_info);



/**
 * @brief   Package protocol data for requesting utc data from the cloud under the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketRequesUTCData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp);




/**
 * @brief   Setting the type of device report
 * @note    This function can change the type of data reported by the device according to the type of request reported by the cloud
 * 
 * @param[in]   type    The type of report
 *              DEVICE_REPORT_TYPE：device auto report
 *              CLOUD_QUERY_TYPE：cloud request report
 *              CLOUD_ALL_QUERY_TYPE：cloud requests all status reporting
 */
void aiio_SetPacketReportAttributesType(uint8_t type);


/**
 * @brief   Package protocol data for device attribute data under the cloud protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   device_att              The attribute data in json string format
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketReportAttrbutesData(char *packet_data, uint32_t packet_data_len, char *msgMid, char *from, uint32_t current_stamp, char *device_att);


/**
 * @brief   Packing protocol data under the cloud protocol
 * @note    Package different types of ack protocol data according to the command space and command name on the protocol
 * 
 * @param[in]   packet_data             The buffer space that receives the packaged data
 * @param[in]   packet_data_len         The length of the buffer space that receives the packaged data
 * @param[in]   ns                      The command space on the protocol
 * @param[in]   name                    The command name on the protocol
 * @param[in]   msgMid                  The message ID that is from cloud, if set to NULL, This function will generate the msgid randomly
 * @param[in]   from                    The source of configuration data, it maybe come from app or cloud, if set to NULL, This function is set to default to device.
 * @param[in]   current_stamp           Time stamp of the local current time
 * @param[in]   code                    The error code that needs to response to cloud. all kind of error code that is defined in this file that is called aiio_protocol_code.h
 * 
 * @return  int  Reture all kind of error code that is defined in this file that is called aiio_protocol_code.h
 */
int32_t aiio_PacketAckData(char *packet_data, uint32_t packet_data_len, char *ns, char *name, char *msgMid, char *from, uint32_t current_stamp, int32_t code);



#endif

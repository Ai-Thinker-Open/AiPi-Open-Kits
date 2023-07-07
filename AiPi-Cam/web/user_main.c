/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Copyright (c) 2014 Winner Micro Electronic Design Co., Ltd.
 * All rights reserved.
 *
 * Author : houxf
 *
 * Date : 2021-12-1
 *****************************************************************************/
#include "user_config.h"

#define IMG_VER_DEF "1.0.0"

// task
static tls_os_queue_t *gsUserTaskQueue = NULL;
static OS_STK UserTaskStk[USER_TASK_SIZE];
//
static int CreateUserTask(void);
static void UserTaskProc(void *param);
static void UserWlanStatusChangedEvent(u8 status);
static void UserWlanApEvent(u8 *mac, enum tls_wifi_client_event_type event);
static int UserCreateAp(void);
static void UserSkCreateTCP(void);
static void CameraDataProcessCb(uint8_t *data, uint32_t len);

#define TCP_LOCAL_PORT 62345
static struct tls_socket_desc gsTcpSKDesc;
static uint16_t gsUserSkFd = 0;
at_os_mutex_t MuxSem_Handle = NULL;
at_os_mutex_t MuxSemCamera_Handle = NULL;

uint32_t res_len;
u8 server_buf[20000];
void server_task_process(void *msg)
{
	mhttp_server_init();
}

/***************************************************************************
 * Description: UserDeviceInit
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
void UserDeviceInit(void)
{
	printf("WM SMQ %s, %s, %s\r\n", IMG_VER_DEF, __DATE__, __TIME__);
	MuxSem_Handle = xSemaphoreCreateMutex();
	if (NULL != MuxSem_Handle)
	{
		printf("MuxSem_Handle 互斥量创建成功!\r\n");
	}
	MuxSemCamera_Handle = xSemaphoreCreateMutex();
	if (NULL != MuxSemCamera_Handle)
	{
		printf("MuxSemCamera_Handle 互斥量创建成功!\r\n");
	}
	//�û��Լ���task
	CreateUserTask();
}
/***************************************************************************
 * Description: UserSendMsg
 *
 * Auth: houxf
 *
 *Date: 2017-12-29
 ****************************************************************************/
void UserSendMsg(uint32_t msg)
{
	tls_os_queue_send(gsUserTaskQueue, (void *)msg, 0);
}
/***************************************************************************
 * Description: �����û�����
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static int CreateUserTask(void)
{
	tls_os_queue_create(&gsUserTaskQueue, USER_QUEUE_SIZE);
	tls_os_task_create(NULL, NULL, UserTaskProc, NULL,
					   (void *)UserTaskStk,			 /* ����ջ����ʼ��ַ */
					   USER_TASK_SIZE * sizeof(u32), /* ����ջ�Ĵ�С     */
					   USER_TASK_PRO,
					   0);
	return WM_SUCCESS;
}
/***************************************************************************
 * Description: �û��������ص�����
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static void UserTaskProc(void *param)
{
	void *msg;
	extern void tls_sys_clk_set(u32 clk);
	extern int tls_flash_unlock(void);
	CameraPinDef pin;

	tls_flash_unlock();

	//	CLK
	tls_sys_clk_set(2);
	//  psram init
	wm_psram_config(0);
	psram_init(PSRAM_QPI);
	//	Camera Init
	pin.camera_addr = CAMERA_ADDR;
	pin.scl = I2C_SCL;
	pin.sda = I2C_SDA;
	pin.pdwn = CAMERA_PWDN;
	pin.mclk = CAMERA_MCLK;
	pin.cs = CAMERA_SPI_CS;
	pin.clk = CAMERA_SPI_CLK;
	pin.d0 = CAMERA_SPI_D0;
	pin.d1 = CAMERA_SPI_D1;
	CameraInit(CAP_IMG_QVGA_YUV422, pin);
	// CameraSetMirrorFlip(0, 0);
	CameraSetMirrorFlip(1, 0);
	
	CameraDataProcessCbRegister(CameraDataProcessCb);

	// ע������״̬�ص�����
	tls_netif_add_status_event(UserWlanStatusChangedEvent);
	tls_wifi_softap_client_event_register(UserWlanApEvent);
	int res = UserCreateAp();
	// if(res == WM_SUCCESS)
	// {
	// 	axk_at_printf("\r\nAP Create success!\r\n");
	// }
	// else
	// {
	// 	axk_at_printf("\r\nAP Create fail!\r\n");
	// }
	for (;;)
	{
		tls_os_queue_receive(gsUserTaskQueue, (void **)&msg, 0, 0);
		switch ((u32)msg)
		{
		case MSG_NET_UP: // �����ɹ�
						 // UserSkCreateTCP();
			tls_os_task_create(NULL, NULL,
							   server_task_process,
							   NULL,
							   (void *)server_buf,
							   20000 * sizeof(u8),
							   50,
							   0);
			break;

		case MSG_STA_UP:
			USER_PRINT("houxf debug MSG_STA_UP\r\n");
			break;

		case MSG_STA_DOWN:
			USER_PRINT("houxf debug MSG_STA_DOWN\r\n");
			break;

		default:
			break;
		}
	}
}
/***************************************************************************
 * Description: ����״̬�仯�ص�����
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static void UserWlanStatusChangedEvent(u8 status)
{
	switch (status)
	{
	case NETIF_WIFI_JOIN_SUCCESS:
		USER_PRINT("houxf debug NETIF_WIFI_JOIN_SUCCESS\r\n");
		break;
	case NETIF_WIFI_JOIN_FAILED:
		USER_PRINT("houxf debug NETIF_WIFI_JOIN_FAILED\r\n");
		break;
	case NETIF_WIFI_DISCONNECTED:
		USER_PRINT("houxf debug NETIF_WIFI_DISCONNECTED\r\n");
		tls_os_queue_send(gsUserTaskQueue, (void *)MSG_NET_DOWN, 0);
		break;
	case NETIF_IP_NET2_UP:
		USER_PRINT("houxf debug NETIF_IP_NET2_UP\r\n");
		tls_os_queue_send(gsUserTaskQueue, (void *)MSG_NET_UP, 0);
		break;
	default:
		break;
	}
}

/***************************************************************************
 * Description: UserWlanApEvent
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static void UserWlanApEvent(u8 *mac, enum tls_wifi_client_event_type event)
{
	if (event)
	{
		tls_os_queue_send(gsUserTaskQueue, (void *)MSG_STA_DOWN, 0);
	}
	else
	{
		tls_os_queue_send(gsUserTaskQueue, (void *)MSG_STA_UP, 0);
	}
	USER_PRINT("client is %s\r\n", event ? "offline" : "online");
}

/***************************************************************************
 * Description: ����softap
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static int  UserCreateAp(void)
{
	struct tls_softap_info_t apinfo;
	struct tls_ip_info_t ipinfo;
	u8 ret = 0;
	u8 ssid_set = 0;
	char ssid[32];
	u8 ssid_len;
	u8 *mac = NULL;
	extern u8 *wpa_supplicant_get_mac(void);

	/*����BSSID�㲥��־*/
	tls_param_get(TLS_PARAM_ID_BRDSSID, (void *)&ssid_set, (bool)0);
	if (0 == ssid_set)
	{
		ssid_set = 1;
		tls_param_set(TLS_PARAM_ID_BRDSSID, (void *)&ssid_set, (bool)1);
	}
	mac = wpa_supplicant_get_mac();
	memset(ssid, 0, sizeof(ssid));
	sprintf(ssid, "%s-%02x%02x", USER_SSID, *(mac + 4), *(mac + 5));
	ssid_len = strlen(ssid);
	for (u8 i = ssid_len - 4; i < ssid_len; i++)
	{
		ssid[i] = toupper(ssid[i]);
	}
	MEMCPY(apinfo.ssid, ssid, ssid_len);
	apinfo.ssid[ssid_len] = '\0';
	/*0:open, 1:wep64, 2:wep128*/
	apinfo.encrypt = IEEE80211_ENCRYT_NONE;
	apinfo.channel = 1;
	/*�����ʽ:0��hex��ʽ��1��ascii��ʽ*/
	apinfo.keyinfo.format = 1;
	/*wep����*/
	apinfo.keyinfo.index = 1;
	apinfo.keyinfo.key_len = strlen(USER_KEY); /*���볤��*/
	memset(apinfo.keyinfo.key, 0, 64);
	MEMCPY(apinfo.keyinfo.key, USER_KEY, apinfo.keyinfo.key_len);
	/*ip������Ϣ:ip��ַ�����룬dns����*/
	ipinfo.ip_addr[0] = 192;
	ipinfo.ip_addr[1] = 168;
	ipinfo.ip_addr[2] = 4;
	ipinfo.ip_addr[3] = 1;
	ipinfo.netmask[0] = 255;
	ipinfo.netmask[1] = 255;
	ipinfo.netmask[2] = 255;
	ipinfo.netmask[3] = 0;
	MEMCPY(ipinfo.dnsname, "wm.com", sizeof("wm.com"));
	ret = tls_wifi_softap_create((struct tls_softap_info_t *)&apinfo, (struct tls_ip_info_t *)&ipinfo);
	printf("houxf debug user ap create %x ! \n", ret);
	// axk_at_printf("\r\n%s\r\n",apinfo.ssid);
	return ret;
}

/***************************************************************************
 * Description: UserSkTcp1Connected
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static err_t UserSkTcpConnected(uint8_t skt_num, err_t err)
{
	USER_PRINT("houxf debug tcp connected socket num=%d, err=%d\r\n", skt_num, err);
	if (ERR_OK == err)
	{
		gsUserSkFd = skt_num;
		tls_os_queue_send(gsUserTaskQueue, (void *)MSG_SK_CAP, 0);
	}
	return ERR_OK;
}
/***************************************************************************
 * Description: UserSkTcp1Err
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static void UserSkTcpErr(uint8_t skt_num, err_t err)
{
	USER_PRINT("houxf debug tcp close socket num=%d, err=%d\r\n", skt_num, err);
	tls_socket_close(gsUserSkFd);
	gsUserSkFd = 0;
}
/***************************************************************************
 * Description: UserSkTcpRecv
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static err_t UserSkTcpRecv(uint8_t skt_num, struct pbuf *p, err_t err)
{
	if (p)
	{
		pbuf_free(p);
	}
	return ERR_OK;
}
/***************************************************************************
 * Description: UserSkCreateTCP
 *
 * Auth: houxf
 *
 *Date: 2016-12-11
 ****************************************************************************/
static void UserSkCreateTCP(void)
{
	memset(&gsTcpSKDesc, 0, sizeof(struct tls_socket_desc));
	gsTcpSKDesc.cs_mode = SOCKET_CS_MODE_SERVER;
	gsTcpSKDesc.protocol = SOCKET_PROTO_TCP;
	gsTcpSKDesc.port = TCP_LOCAL_PORT;
	gsTcpSKDesc.acceptf = UserSkTcpConnected;
	gsTcpSKDesc.errf = UserSkTcpErr;
	gsTcpSKDesc.recvf = UserSkTcpRecv;
	USER_PRINT("houxf debug tcp server listen port=%d\r\n", gsTcpSKDesc.port);
	tls_socket_create(&gsTcpSKDesc);
}
#ifndef JOHH
uint8_t *yuv_buf = NULL;
#endif

static uint8_t cameramode = 0;
void MMSetCameraMode(uint8_t mode)
{
	cameramode = mode;
}

void MMStartCamera(void)
{
	BaseType_t xReturn = pdTRUE;				   /* 定义一个创建信息返回值，默认为 pdPASS */
	xReturn = xSemaphoreGive(MuxSemCamera_Handle); //给出互斥量
}

static void CameraDataProcessCbCC(uint8_t *data, uint32_t len)
{
	BaseType_t xReturn = pdTRUE;				  /* 定义一个创建信息返回值，默认为 pdPASS */
	xReturn = xSemaphoreTake(MuxSemCamera_Handle, /* 互斥量句柄 */
							 portMAX_DELAY);	  /* 等待时间 */
	if (xReturn == pdTRUE)
	{
		uint32_t t1 = 0, t2 = 0, jpeg_length = 320 * 240 * 2;
		uint8_t header[8] = {0x5a, 0xa5, 0x5a, 0xa5, 0x00, 0x00, 0x00, 0x00};
#ifndef JOHHN
		yuv_buf = data;
#endif
		uint8_t *jpeg_buf = (uint8_t *)(PSRAM_ADDR_START) + 320 * 240 * 2 * 2;
		uint8_t *res_buf;
		res_buf = PSRAM_ADDR_START + 1000000;

		t1 = tls_os_get_time();
		jpeg_length = yuv422_to_jpeg(data, 320, 240, jpeg_buf, jpeg_length, 75);
		t2 = tls_os_get_time();

		// taskENTER_CRITICAL();
		xReturn = xSemaphoreTake(MuxSem_Handle,	 /* 互斥量句柄 */
								 portMAX_DELAY); /* 等待时间 */
		res_len = jpeg_length;
		printf("jpeg_len is %d\n", jpeg_length);
		for (uint32_t i = 0; i < res_len; i++)
		{
			res_buf[i] = jpeg_buf[i];
		}

		datajpeg_buf = res_buf;

		datajpeg_len = res_len;
		xReturn = xSemaphoreGive(MuxSem_Handle); //给出互斥量
	}
}

static void CameraDataProcessCbGZ(uint8_t *data, uint32_t len)
{
	uint32_t t1 = 0, t2 = 0, jpeg_length = 320 * 240 * 2;
	uint8_t header[8] = {0x5a, 0xa5, 0x5a, 0xa5, 0x00, 0x00, 0x00, 0x00};
#ifndef JOHHN
	yuv_buf = data;
#endif
	uint8_t *jpeg_buf = (uint8_t *)(PSRAM_ADDR_START) + 320 * 240 * 2 * 2;
	uint8_t *res_buf;
	BaseType_t xReturn = pdTRUE; /* 定义一个创建信息返回值，默认为 pdPASS */
	res_buf = PSRAM_ADDR_START + 1000000;

	t1 = tls_os_get_time();
	jpeg_length = yuv422_to_jpeg(data, 320, 240, jpeg_buf, jpeg_length, 75);
	t2 = tls_os_get_time();

	// taskENTER_CRITICAL();
	xReturn = xSemaphoreTake(MuxSem_Handle,	 /* 互斥量句柄 */
							 portMAX_DELAY); /* 等待时间 */
	res_len = jpeg_length;
	printf("jpeg_len is %d\n", jpeg_length);
	for (uint32_t i = 0; i < res_len; i++)
	{
		res_buf[i] = jpeg_buf[i];
	}

	datajpeg_buf = res_buf;

	datajpeg_len = res_len;
	xReturn = xSemaphoreGive(MuxSem_Handle); //给出互斥量
}

static void CameraDataProcessCb(uint8_t *data, uint32_t len)
{
	if (cameramode)
	{
		CameraDataProcessCbCC(data, len);
	}
	else
	{
		CameraDataProcessCbGZ(data, len);
	}
	// tls_os_time_delay(80);
	// taskEXIT_CRITICAL();
	// if (gsUserSkFd > 0)
	// {
	// 	memcpy(header + 4, (char *)&jpeg_length, 4);
	// 	tls_socket_send(gsUserSkFd, header, 8);
	// 	tls_socket_send(gsUserSkFd, jpeg_buf, jpeg_length);
	// }
	//	printf("%dms ", (t2 - t1)*2);
}


//kiwi add

void CamDataProcess(uint8_t *data, uint32_t len)
{
	BaseType_t xReturn = pdTRUE;				  /* 定义一个创建信息返回值，默认为 pdPASS */
	// xReturn = xSemaphoreTake(MuxSemCamera_Handle, /* 互斥量句柄 */
	// 						 portMAX_DELAY);	  /* 等待时间 */
	if (xReturn == pdTRUE)
	{
		uint32_t t1 = 0, t2 = 0, jpeg_length = 320 * 240 * 2;

		// uint8_t *jpeg_buf = (uint8_t *)(PSRAM_ADDR_START) + 320 * 240 * 2 * 2;

		xReturn = xSemaphoreTake(MuxSem_Handle,	 /* 互斥量句柄 */
								 portMAX_DELAY); /* 等待时间 */


		datajpeg_buf = data;

		datajpeg_len = len;
		xReturn = xSemaphoreGive(MuxSem_Handle); //给出互斥量
	}
}



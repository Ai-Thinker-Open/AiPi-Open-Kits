#include "usbd_core.h"
#include "usbd_hid.h"
#include "FreeRTOS.h"
#include "task.h"
#include "log.h"

#define DBG_TAG "USB HID"

#define USBD_VID           0xffff
#define USBD_PID           0xffff
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define HID_INT_EP          0x81
#define HID_INT_EP_SIZE     8
#define HID_INT_EP_INTERVAL 10

#define HID_INT_E2          0x82

#define USB_HID_CONFIG_DESC_SIZ       34
#define HID_KEYBOARD_REPORT_DESC_SIZE 89
// #define HID_KEYBOARD_REPORT_DESC_SIZE 28

static bool desktop_lock = false;

static const uint8_t hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0002, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_HID_CONFIG_DESC_SIZ, 0x01, 0x01,0xe0, USBD_MAX_POWER),

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x00,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x01,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x33,                          /* bCountryCode: Hardware target country */
    0x01,                          /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                          /* bDescriptorType */
    HID_KEYBOARD_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HID_INT_EP,                   /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
        HID_INT_EP_SIZE,              /* wMaxPacketSize: 4 Byte max */
        0x00,
        HID_INT_EP_INTERVAL, /* bInterval: Polling Interval */
        /* 34 */
        ///////////////////////////////////////
        /// string0 descriptor
        ///////////////////////////////////////
        USB_LANGID_INIT(USBD_LANGID_STRING),
        ///////////////////////////////////////
        /// string1 descriptor  字符串描述符
        ///////////////////////////////////////
        18+2,                       /* bLength */
        USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */ //cherryUSB
        'C', 0x00,                  /* wcChar0 */
        'h', 0x00,                  /* wcChar1 */
        'e', 0x00,                  /* wcChar2 */
        'r', 0x00,                  /* wcChar3 */
        'r', 0x00,                  /* wcChar4 */
        'y', 0x00,                  /* wcChar5 */
        'U', 0x00,                  /* wcChar6 */
        'S', 0x00,                  /* wcChar7 */
        'B', 0x00,                  /* wcChar8 */
        ///////////////////////////////////////
        /// string2 descriptor
        ///////////////////////////////////////
        26+2,                       /* bLength */
        USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
        'A', 0x00,                  /* wcChar0 */
        'i', 0x00,                  /* wcChar1 */
        'P', 0x00,                  /* wcChar2 */
        'i', 0x00,                  /* wcChar3 */
        '-', 0x00,                  /* wcChar4 */
        'K', 0x00,                  /* wcChar5 */
        'e', 0x00,                  /* wcChar6 */
        'y', 0x00,                  /* wcChar7 */
        'b', 0x00,                  /* wcChar8 */
        'o', 0x00,                  /* wcChar9 */
        'a', 0x00,                  /* wcChar10 */
        'r', 0x00,                  /* wcChar11 */
        'd', 0x00,                  /* wcChar12 */
        ///////////////////////////////////////
        /// string3 descriptor
        ///////////////////////////////////////
        0x16,                       /* bLength */
        USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
        '2', 0x00,                  /* wcChar0 */
        '0', 0x00,                  /* wcChar1 */
        '2', 0x00,                  /* wcChar2 */
        '2', 0x00,                  /* wcChar3 */
        '1', 0x00,                  /* wcChar4 */
        '2', 0x00,                  /* wcChar5 */
        '3', 0x00,                  /* wcChar6 */
        '4', 0x00,                  /* wcChar7 */
        '5', 0x00,                  /* wcChar8 */
        '6', 0x00,                  /* wcChar9 */
    #ifdef CONFIG_USB_HS
        ///////////////////////////////////////
        /// device qualifier descriptor
        ///////////////////////////////////////
        0x0a,
        USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02,
        0x00,
        0x00,
        0x00,
        0x40,
        0x01,
        0x00,
    #endif
        0x00
};

/* USB HID device Configuration Descriptor */
static uint8_t hid_desc[9] __ALIGN_END = {
    /* 18 */
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                          /* bCountryCode: Hardware target country */
    0x01,                          /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                          /* bDescriptorType */
    HID_KEYBOARD_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
};

static const uint8_t hid_keyboard_report_desc[HID_KEYBOARD_REPORT_DESC_SIZE] = {

    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x85, 0x01, //   REPORT_ID (1)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)
    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs)
    0x95, 0x05, // REPORT_COUNT (5)
    0x75, 0x01, // REPORT_SIZE (1)
    0x05, 0x08, // USAGE_PAGE (LEDs)
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    0x29, 0x05, // USAGE_MAXIMUM (Kana)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x03, // REPORT_SIZE (3)
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0xFF, // LOGICAL_MAXIMUM (255)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)
    0xc0,        // END_COLLECTION

    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x19, 0x00,                    //   USAGE_MINIMUM (Unassigned)
    0x29, 0x80,                    //   USAGE_MAXIMUM (Selection)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0x80,  0x00,              //   LOGICAL_MAXIMUM (128)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                           // END_COLLECTION
};

void usbd_configure_done_callback(void)
{
    /* no out ep, do nothing */
}

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t hid_state = HID_STATE_IDLE;

void usbd_hid_int_callback(uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
}

static struct usbd_endpoint hid_in_ep = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = HID_INT_EP
};

struct usbd_interface intf0;

void hid_keyboard_init(void)
{
    usbd_desc_register(hid_descriptor);
    usbd_add_interface(usbd_hid_init_intf(&intf0, hid_keyboard_report_desc, HID_KEYBOARD_REPORT_DESC_SIZE));
    usbd_add_endpoint(&hid_in_ep);
    usbd_initialize();
}
/**
 * @brief usb_hid_keyboard_send
 *
 * @param Keyboard_ctrl LeftControl~Keyboard Right GUI
 * @param keybord_key a ~ Application
*/
void usb_hid_keyboard_send(char Keyboard_ctrl, char keybord_key)
{
    uint8_t sendbuffer[] = { 0X01, Keyboard_ctrl,0x00,keybord_key,0x00,0x00,0x00,0x00,0x00 }; // CRTL+L
    bflb_l1c_dcache_clean_range(sendbuffer, 9);
    int ret = usbd_ep_start_write(HID_INT_EP, sendbuffer, 9);
    if (ret < 0) {
        return;
    }
    hid_state = HID_STATE_BUSY;
    while (hid_state == HID_STATE_BUSY) {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

//休眠和唤醒目前只能实现休眠，唤醒未实现
#if 1
/**
 * @brief usb_hid_keyboard_setSleep
 *
*/
void usb_hid_keyboard_stop_send(void);

void usb_hid_keyboard_setSleep(void)
{
    if (desktop_lock) return; //如果屏幕已经灭掉，就退出

    uint8_t sendbuffer[] = { 0x02,0x70 };

    for (size_t i = 0; i < 10; i++)
    {
        bflb_l1c_dcache_clean_range(sendbuffer, 2);
        int ret = usbd_ep_start_write(HID_INT_EP, sendbuffer, 2);
        if (ret < 0) {
            desktop_lock = true;
            usb_hid_keyboard_stop_send();
            return;
        }
        hid_state = HID_STATE_BUSY;
        for (size_t i = 0; i < 30; i++)
        {
            if (hid_state!=HID_STATE_BUSY)break;
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    usb_hid_keyboard_stop_send();

    desktop_lock = true;
}

/**
 * @brief usb_hid_keyboard_setWakeup
 *
*/

void usb_hid_keyboard_setWakeup(void)
{
    // uint8_t sendbuffer[] = { 0x01,0x00,0x00,HID_KBD_USAGE_ENTER,0x00,0x00,0x00,0x00,0x00 }; // CRTL+L
    if (!desktop_lock) return; //如果屏幕还亮着，就退出
    uint8_t sendbuffer[] = { 0x02, 0x6f };

    for (size_t i = 0; i < 10; i++)
    {
        bflb_l1c_dcache_clean_range(sendbuffer, 2);
        int ret = usbd_ep_start_write(HID_INT_EP, sendbuffer, 2);
        if (ret < 0) {
            return;
        }
        hid_state = HID_STATE_BUSY;
        for (size_t i = 0; i < 30; i++)
        {
            if (hid_state!=HID_STATE_BUSY)break;
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
    usb_hid_keyboard_stop_send();
    desktop_lock = false;
}

void usb_hid_keyboard_stop_send(void)
{
    uint8_t sendbuffer[] = { 0x02, 0x00 };
    bflb_l1c_dcache_clean_range(sendbuffer, 2);
    int ret = usbd_ep_start_write(HID_INT_EP, sendbuffer, 2);
    if (ret < 0) {
        return;
    }
    hid_state = HID_STATE_BUSY;
    for (size_t i = 0; i < 30; i++)
    {
        if (hid_state!=HID_STATE_BUSY)break;
        vTaskDelay(pdMS_TO_TICKS(1));
    }

}
#endif
/**
 * @brief  usb_hid_keyboard_inputpassword
 *
 * @param PIN 锁屏 PIN码
*/
void usb_hid_keyboard_inputpassword(char* PIN)
{
    if (!desktop_lock) return; //如果屏幕没有锁定，就退出
    if (PIN==NULL) return;

    uint8_t lock_pin_len = strlen(PIN);//计算密码数量
    char* lock_pin = PIN;
    //发送回车进入密码输出
    usb_hid_keyboard_send(0x00, HID_KBD_USAGE_SPACE);//空格键，进入密码输入 
    vTaskDelay(pdMS_TO_TICKS(100));
    usb_hid_keyboard_send(0x00, 0x00);
    vTaskDelay(pdMS_TO_TICKS(100));
    //如果密码为空，就退出
    for (size_t i = 0; i < lock_pin_len; i++)
    {
        //发送密码
        usb_hid_keyboard_send(0x00, (lock_pin[i]-'0')+29);//
        vTaskDelay(pdMS_TO_TICKS(100));
        usb_hid_keyboard_send(0x00, 0x00);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    desktop_lock = false;

}
/**
 * @brief 锁屏
 *
*/
void usb_hid_keyboard_lock(void)
{
    if (desktop_lock) return; //如果是锁屏状态，就不继续锁屏
    usb_hid_keyboard_send(0x08, 0X00);
    usb_hid_keyboard_send(0x08, 0X0f);
    vTaskDelay(pdMS_TO_TICKS(100));
    usb_hid_keyboard_send(0x00, 0X00);
    desktop_lock = true;
}
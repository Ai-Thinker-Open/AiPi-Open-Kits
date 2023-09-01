/**
 * @file kvm_dev.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-22
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef KVM_DEV_H
#define KVM_DEV_H

typedef enum {
    KVM_USB_CH_1 = 0,
    KVM_USB_CH_2,
    KVM_USB_CH_3,
}kvm_usb_ch_t;

typedef enum {
    KVM_HDMI_CH_1 = 0,
    KVM_HDMI_CH_2,
    KVM_HDMI_CH_3,
}kvm_hdmi_ch_t;
void aipi_kvm_dev_init(void);
void aipi_kvm_set_usb_channel(kvm_usb_ch_t kvm_usb_ch);
void aipi_kvm_set_HDIM_channel(kvm_hdmi_ch_t kvm_ch);

#endif
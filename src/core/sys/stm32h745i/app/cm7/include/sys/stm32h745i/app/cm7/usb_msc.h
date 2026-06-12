#pragma once
#include <stdbool.h>

extern void USB_Device_Init(void);

//--------------------------------------------------------------------+
// TinyUSB Device Callbacks
//--------------------------------------------------------------------+
// Invoked when device is mounted
extern void tud_mount_cb(void);

// Invoked when device is unmounted
extern void tud_umount_cb(void);

// Invoked when usb bus is suspended
extern void tud_suspend_cb(bool remote_wakeup_en);

// Invoked when usb bus is resumed
extern void tud_resume_cb(void);

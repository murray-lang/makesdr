# STM32H745I USB Host HID Control

This directory contains the STM32-specific implementation of USB Host HID control for the STM32H745I-DISCO board.

## Files

- **Stm32HidUsbControl.h/cpp** - Main USB HID control class implementation
- **usbh_conf.h** - USB Host stack configuration
- **usbh_platform.cpp** - Low-level USB Host HAL integration callbacks

## Integration Requirements

### 1. Hardware Configuration

In your STM32 project, you need to configure the USB OTG peripheral:

```c
// In your main application or initialization code
HCD_HandleTypeDef hhcd;

void USB_OTG_HS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&hhcd);
}

// Enable USB OTG HS clock and GPIO configuration
// Configure VBUS detection if available
```

### 2. Periodic Processing

The USB Host stack requires periodic processing. Call `process()` regularly:

```cpp
Stm32HidUsbControl usbControl(VENDOR_ID, PRODUCT_ID);
usbControl.initialise();

while (1) {
    usbControl.process();  // Call this regularly (e.g., in main loop or timer)

    if (usbControl.isReady()) {
        // Device is ready for communication
        unsigned char buffer[64];
        size_t bytesRead;
        usbControl.read(buffer, sizeof(buffer), &bytesRead);
    }
}
```

### 3. STM32CubeMX Configuration

Configure USB_OTG_HS in Host mode:
- Mode: Host_Only
- Speed: High Speed or Full Speed
- DMA: Enabled (recommended)
- VBUS sensing: Enabled if available

### 4. USB Host Stack Notes

- The implementation uses a circular buffer (256 bytes) for received data
- Data reception is handled via `USBH_HID_EventCallback()`
- Device connection/disconnection is handled via `USBH_UserProcess()`
- Only one USB HID device instance is supported (uses global pointer)

### 5. Memory Requirements

Ensure sufficient heap for USB Host stack:
- Stack uses dynamic allocation for descriptors and buffers
- Minimum recommended heap: 8KB

### 6. Known Limitations

- Single device support (one instance at a time)
- No RTOS support in current configuration (set `USBH_USE_OS` to 1 for RTOS)
- Assumes USB_OTG_HS peripheral (modify `usbh_platform.cpp` for FS)

## Error Handling

The implementation returns `ResultCode` values:
- `ERR_SETTING_CONTROL_USB_HID_INIT` - Initialization failed
- `ERR_SETTING_CONTROL_USB_HID_NOT_INIT` - Not initialized
- `ERR_SETTING_CONTROL_USB_HID_NOT_CONNECTED` - Device not connected
- `ERR_SETTING_CONTROL_USB_HID_WRONG_DEVICE` - VID/PID mismatch
- `ERR_SETTING_CONTROL_USB_HID_OPEN` - Failed to open device
- `ERR_SETTING_CONTROL_USB_HID_READ` - Read error
- `ERR_SETTING_CONTROL_USB_HID_WRITE` - Write error

## Example Usage

```cpp
#include "Stm32HidUsbControl.h"

#define MY_DEVICE_VID  0x1234
#define MY_DEVICE_PID  0x5678

Stm32HidUsbControl usbCtrl(MY_DEVICE_VID, MY_DEVICE_PID);

// Initialize
if (usbCtrl.initialise() != ResultCode::OK) {
    // Handle error
}

// Wait for device connection
while (!usbCtrl.discover()) {
    usbCtrl.process();
    HAL_Delay(100);
}

// Open device
if (usbCtrl.open() != ResultCode::OK) {
    // Handle error
}

// Communication loop
while (1) {
    usbCtrl.process();  // Process USB events

    // Read data
    unsigned char rxBuf[64];
    size_t bytesRead;
    if (usbCtrl.read(rxBuf, sizeof(rxBuf), &bytesRead) == ResultCode::OK) {
        if (bytesRead > 0) {
            // Process received data
        }
    }

    // Write data
    unsigned char txBuf[64] = {0x01, 0x02, 0x03};
    size_t bytesWritten;
    usbCtrl.write(txBuf, sizeof(txBuf), &bytesWritten);

    HAL_Delay(10);
}
```

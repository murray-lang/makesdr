#pragma once

#include "settings/control/usb/HidUsbControl.h"
#include "usbh_core.h"
#include "usbh_hid.h"
#include <cstddef>

/**
 * @brief STM32 HID USB Control implementation using STM32 USB Host Library
 *
 * This class provides USB HID device control for STM32H7 microcontrollers
 * using the STM32 USB Host middleware stack.
 */
class Stm32HidUsbControl : public HidUsbControl {
public:
    Stm32HidUsbControl(uint16_t vendorId, uint16_t productId);
    ~Stm32HidUsbControl() override;

    ResultCode initialise() override;
    bool discover() override;
    ResultCode open() override;
    void close() override;
    void exit() override;

    ResultCode read(unsigned char* data, size_t length, size_t* bytesRead) const override;
    ResultCode write(const unsigned char* data, size_t length, size_t* bytesWritten) const override;

    /**
     * @brief Process USB Host state machine (must be called periodically)
     */
    void process();

    /**
     * @brief Check if device is connected
     */
    bool isConnected() const;

    /**
     * @brief Check if device is ready for communication
     */
    bool isReady() const;

    /**
     * @brief Get pointer to USB Host handle
     */
    USBH_HandleTypeDef* getHostHandle() { return &m_hostHandle; }

private:
    USBH_HandleTypeDef m_hostHandle;
    HID_HandleTypeDef m_hidHandle;

    bool m_initialized;
    bool m_deviceConnected;
    bool m_deviceReady;

    // Circular buffer for received data
    static constexpr size_t RX_BUFFER_SIZE = 256;
    unsigned char m_rxBuffer[RX_BUFFER_SIZE];
    volatile size_t m_rxHead;
    volatile size_t m_rxTail;

    // Callback for HID data reception
    static void hidDataCallback(USBH_HandleTypeDef* phost);

    friend void USBH_HID_EventCallback(USBH_HandleTypeDef* phost);
};

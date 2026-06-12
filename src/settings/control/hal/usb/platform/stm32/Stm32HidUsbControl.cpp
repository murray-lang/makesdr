#include "Stm32HidUsbControl.h"
#include <ResultCode.h>
#include <cstring>

// External USB Host application state callback
extern "C" void USBH_UserProcess(USBH_HandleTypeDef* phost, uint8_t id);

// Global instance pointer for callbacks (single instance assumed)
static Stm32HidUsbControl* g_instance = nullptr;

Stm32HidUsbControl::Stm32HidUsbControl(uint16_t vendorId, uint16_t productId)
    : HidUsbControl(vendorId, productId)
    , m_initialized(false)
    , m_deviceConnected(false)
    , m_deviceReady(false)
    , m_rxHead(0)
    , m_rxTail(0)
{
    memset(&m_hostHandle, 0, sizeof(m_hostHandle));
    memset(&m_hidHandle, 0, sizeof(m_hidHandle));
    memset(m_rxBuffer, 0, RX_BUFFER_SIZE);
    g_instance = this;
}

Stm32HidUsbControl::~Stm32HidUsbControl()
{
    exit();
    g_instance = nullptr;
}

ResultCode Stm32HidUsbControl::initialise()
{
    if (m_initialized) {
        return ResultCode::OK;
    }

    // Initialize USB Host Library
    USBH_StatusTypeDef status = USBH_Init(&m_hostHandle, USBH_UserProcess, 0);
    if (status != USBH_OK) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_INIT;
    }

    // Register HID Class
    status = USBH_RegisterClass(&m_hostHandle, USBH_HID_CLASS);
    if (status != USBH_OK) {
        USBH_DeInit(&m_hostHandle);
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_INIT;
    }

    // Start USB Host
    status = USBH_Start(&m_hostHandle);
    if (status != USBH_OK) {
        USBH_DeInit(&m_hostHandle);
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_INIT;
    }

    m_initialized = true;
    return ResultCode::OK;
}

bool Stm32HidUsbControl::discover()
{
    if (!m_initialized) {
        return false;
    }

    // Check if a device is connected
    return m_deviceConnected;
}

ResultCode Stm32HidUsbControl::open()
{
    if (!m_initialized) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_NOT_INIT;
    }

    if (!m_deviceConnected) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_NOT_CONNECTED;
    }

    // Check device VID/PID match
    if (m_hostHandle.device.DevDesc.idVendor != m_vendorId ||
        m_hostHandle.device.DevDesc.idProduct != m_productId) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_WRONG_DEVICE;
    }

    // Wait for device to be ready
    uint32_t timeout = 5000; // 5 seconds
    uint32_t start = HAL_GetTick();
    while (!m_deviceReady && (HAL_GetTick() - start) < timeout) {
        process();
        HAL_Delay(10);
    }

    if (!m_deviceReady) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_OPEN;
    }

    return ResultCode::OK;
}

void Stm32HidUsbControl::close()
{
    m_deviceReady = false;
    m_rxHead = 0;
    m_rxTail = 0;
}

void Stm32HidUsbControl::exit()
{
    if (m_initialized) {
        USBH_Stop(&m_hostHandle);
        USBH_DeInit(&m_hostHandle);
        m_initialized = false;
        m_deviceConnected = false;
        m_deviceReady = false;
    }
}

ResultCode Stm32HidUsbControl::read(unsigned char* data, size_t length, size_t* bytesRead) const
{
    if (!m_initialized || !m_deviceReady) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_NOT_OPEN;
    }

    *bytesRead = 0;

    // Read from circular buffer
    while (*bytesRead < length && m_rxTail != m_rxHead) {
        data[*bytesRead] = m_rxBuffer[m_rxTail];
        m_rxTail = (m_rxTail + 1) % RX_BUFFER_SIZE;
        (*bytesRead)++;
    }

    return ResultCode::OK;
}

ResultCode Stm32HidUsbControl::write(const unsigned char* data, size_t length, size_t* bytesWritten) const
{
    if (!m_initialized || !m_deviceReady) {
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_NOT_OPEN;
    }

    // Send HID report
    USBH_StatusTypeDef status = USBH_HID_SetReport(&m_hostHandle,
                                                     0x02,  // Report Type: Output
                                                     0x00,  // Report ID
                                                     const_cast<uint8_t*>(data),
                                                     length);

    if (status != USBH_OK) {
        *bytesWritten = 0;
        return ResultCode::ERR_SETTING_CONTROL_USB_HID_WRITE;
    }

    *bytesWritten = length;
    return ResultCode::OK;
}

void Stm32HidUsbControl::process()
{
    if (m_initialized) {
        USBH_Process(&m_hostHandle);
    }
}

bool Stm32HidUsbControl::isConnected() const
{
    return m_deviceConnected;
}

bool Stm32HidUsbControl::isReady() const
{
    return m_deviceReady;
}

// USB Host User Callback
extern "C" void USBH_UserProcess(USBH_HandleTypeDef* phost, uint8_t id)
{
    if (!g_instance) {
        return;
    }

    switch (id) {
    case HOST_USER_SELECT_CONFIGURATION:
        // Configuration selected
        break;

    case HOST_USER_CLASS_ACTIVE:
        // Class active, device is ready
        g_instance->m_deviceReady = true;
        break;

    case HOST_USER_CLASS_SELECTED:
        // Class driver selected
        break;

    case HOST_USER_CONNECTION:
        // Device connected
        g_instance->m_deviceConnected = true;
        break;

    case HOST_USER_DISCONNECTION:
        // Device disconnected
        g_instance->m_deviceConnected = false;
        g_instance->m_deviceReady = false;
        g_instance->m_rxHead = 0;
        g_instance->m_rxTail = 0;
        break;

    case HOST_USER_UNRECOVERED_ERROR:
        // Unrecoverable error
        g_instance->m_deviceReady = false;
        break;

    default:
        break;
    }
}

// HID Event Callback for receiving data
extern "C" void USBH_HID_EventCallback(USBH_HandleTypeDef* phost)
{
    if (!g_instance || !g_instance->isReady()) {
        return;
    }

    HID_HandleTypeDef* HID_Handle = (HID_HandleTypeDef*)phost->pActiveClass->pData;

    if (HID_Handle->length > 0) {
        // Copy received data to circular buffer
        for (uint16_t i = 0; i < HID_Handle->length; i++) {
            size_t nextHead = (g_instance->m_rxHead + 1) % g_instance->RX_BUFFER_SIZE;

            // Check for buffer overflow
            if (nextHead != g_instance->m_rxTail) {
                g_instance->m_rxBuffer[g_instance->m_rxHead] = HID_Handle->pData[i];
                g_instance->m_rxHead = nextHead;
            }
            // else: buffer full, data dropped
        }
    }
}

#include <CrossPlatformTypes.h>
#include "usb/host/UsbHost.h"

#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>
#include <tinyusb/host/tusb_config.h>
#include <tinyusb/host/tusb.h>
#include <tinyusb/host/usbh.h>

#ifndef USB_HOST_THREAD_PRIORITY
#define USB_HOST_THREAD_PRIORITY tskIDLE_PRIORITY
#endif

// Static instance for callbacks
UsbHost* UsbHost::s_instance = nullptr;
UsbHostThread UsbHost::s_thread;

UsbHost::UsbHost(uint16_t vendorId, uint16_t productId)
  : UsbBase(vendorId, productId)
    , m_devAddr(0)
    , m_mounted(false)
{
}

UsbHost::UsbHost(UsbHost&& rhs) noexcept
  : UsbBase(rhs.m_vendorId, rhs.m_productId)
    , m_devAddr(rhs.m_devAddr)
    , m_mounted(rhs.m_mounted)
{
  if (s_instance == &rhs) {
    s_instance = this;
  }
  rhs.m_mounted = false;
  rhs.m_devAddr = 0;
}

UsbHost& UsbHost::operator=(UsbHost&& rhs) noexcept
{
  if (this != &rhs) {
    UsbBase::operator=(move(rhs));
    m_devAddr = rhs.m_devAddr;
    m_mounted = rhs.m_mounted;
  }
  return *this;
}

UsbHost::~UsbHost()
{
  if (s_instance == this) {
    s_instance = nullptr;
  }
}

ResultCode UsbHost::initialise()
{
  // if (!s_thread.isRunning()) {
  //   return s_thread.start(configMINIMAL_STACK_SIZE*2, tskIDLE_PRIORITY, "USB Host");
  // }
  return ResultCode::OK;
}

bool UsbHost::discover()
{
  // return s_thread.isRunning();
  return true;
}

ResultCode UsbHost::open()
{
  s_instance = this; // Set instance for callbacks

  if (!s_thread.isRunning()) {
    ResultCode rc = s_thread.start(configMINIMAL_STACK_SIZE*4, USB_HOST_THREAD_PRIORITY, "USB Host");
    if (rc != ResultCode::OK) {
      printf("Failed to start USB host thread\r\n");
      return rc;
    }
  }

  if (!m_mounted) {
    printf("USB host is not mounted\r\n");
    return ResultCode::ERR_USB_NOT_MOUNTED;
  }
  printf("USB host open() OK\r\n");
  // Device is already opened when mounted
  return ResultCode::OK;
}

void UsbHost::close()
{
  // TinyUSB handles device closure automatically on unmount
  m_mounted = false;
  m_devAddr = 0;
  s_thread.stop();
}

void UsbHost::exit()
{
  // TinyUSB doesn't require explicit exit/cleanup
  close();
}

ResultCode
UsbHost::read(
  uint8_t request,
  uint16_t value,
  uint16_t index,
  unsigned char* data,
  size_t length,
  size_t* bytesRead
)
{
  if (!m_mounted) {
    return ResultCode::ERR_USB_NOT_MOUNTED;
  }

  if (data == nullptr || bytesRead == nullptr) {
    return ResultCode::ERR_USB_HOST_INVALID_PARAMETER;
  }

  // USB control transfer for vendor-specific IN request
  tusb_control_request_t setup = {
    .bmRequestType_bit = {
      .recipient = TUSB_REQ_RCPT_DEVICE,
      .type = TUSB_REQ_TYPE_VENDOR,
      .direction = TUSB_DIR_IN
    },
    .bRequest = request,
    .wValue = value,
    .wIndex = index,
    .wLength = static_cast<uint16_t>(length)
  };

  // Setup transfer structure for synchronous control transfer
  tuh_xfer_t xfer = {
    .daddr = m_devAddr,
    .ep_addr = 0, // Control endpoint
    .setup = &setup,
    .buffer = data,
    .complete_cb = nullptr // Synchronous (blocking)
  };

  bool result = tuh_control_xfer(&xfer);

  if (result) {
    *bytesRead = xfer.actual_len;
    return ResultCode::OK;
  }

  *bytesRead = 0;
  return ResultCode::ERR_USB_HOST_IO;
}

ResultCode
UsbHost::write(uint8_t request, uint16_t value, uint16_t index) const
{
  if (!m_mounted) {
    return ResultCode::ERR_USB_NOT_MOUNTED;
  }
  tusb_control_request_t setup = {
    .bmRequestType_bit = {
      .recipient = TUSB_REQ_RCPT_DEVICE,
      .type = TUSB_REQ_TYPE_VENDOR,
      .direction = TUSB_DIR_OUT
    },
    .bRequest = request,
    .wValue = value,
    .wIndex = index,
    .wLength = 0
  };

  // Setup transfer structure for synchronous control transfer
  // Note: const_cast needed as TinyUSB doesn't use const for buffer
  tuh_xfer_t xfer = {
    .daddr = m_devAddr,
    .ep_addr = 0, // Control endpoint
    .setup = &setup,
    .buffer = nullptr,
    .complete_cb = nullptr // Synchronous (blocking)
  };

  bool result = tuh_control_xfer(&xfer);

  if (!result) {
    return ResultCode::ERR_USB_HOST_IO;
  }

  return ResultCode::OK;
}

ResultCode
UsbHost::write(
  uint8_t request,
  uint16_t value,
  uint16_t index,
  const unsigned char* data,
  size_t length,
  size_t* bytesWritten
) const
{
  // BSP_LED_Toggle(LED_RED);
  if (!m_mounted) {
    return ResultCode::ERR_USB_NOT_MOUNTED;
  }

  if (data == nullptr || bytesWritten == nullptr) {
    return ResultCode::ERR_USB_HOST_INVALID_PARAMETER;
  }

  // USB control transfer for vendor-specific OUT request
  tusb_control_request_t setup = {
    .bmRequestType_bit = {
      .recipient = TUSB_REQ_RCPT_DEVICE,
      .type = TUSB_REQ_TYPE_VENDOR,
      .direction = TUSB_DIR_OUT
    },
    .bRequest = request,
    .wValue = value,
    .wIndex = index,
    .wLength = static_cast<uint16_t>(length)
  };

  // Setup transfer structure for synchronous control transfer
  // Note: const_cast needed as TinyUSB doesn't use const for buffer
  tuh_xfer_t xfer = {
    .daddr = m_devAddr,
    .ep_addr = 0, // Control endpoint
    .setup = &setup,
    .buffer = const_cast<unsigned char*>(data),
    .complete_cb = nullptr // Synchronous (blocking)
  };

  bool result = tuh_control_xfer(&xfer);

  if (result) {
    *bytesWritten = xfer.actual_len;
    // BSP_LED_Toggle(LED_RED);
    return ResultCode::OK;
  }

  *bytesWritten = 0;
  return ResultCode::ERR_USB_HOST_IO;
}

bool UsbHost::matchesDevice(uint16_t vid, uint16_t pid) const
{
  return (vid == m_vendorId) && (pid == m_productId);
}

void UsbHost::handleMount(uint8_t dev_addr)
{
  m_devAddr = dev_addr;
  m_mounted = true;
}

void UsbHost::handleUmount(uint8_t dev_addr)
{
  if (m_devAddr == dev_addr) {
    m_mounted = false;
  }
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks (static methods)
//--------------------------------------------------------------------+

void UsbHost::onMount(uint8_t dev_addr)
{
  tusb_desc_device_t dev_desc;

  // Crucial: Bypass standard structural caching and grab the raw local cache block
  if (tuh_descriptor_get_device_local(dev_addr, &dev_desc)) {
    uint16_t vid = dev_desc.idVendor;
    uint16_t pid = dev_desc.idProduct;

    if (s_instance && s_instance->matchesDevice(vid, pid)) {
      // BSP_LED_On(LED_GREEN);
      s_instance->handleMount(dev_addr);
    }
  } else {
    // printf("tuh_descriptor_get_device_local() returned false\r\n");
  }
}

void UsbHost::onUmount(uint8_t dev_addr)
{
  if (s_instance) {
    s_instance->handleUmount(dev_addr);
    // BSP_LED_Off(LED_GREEN);
  }
}

//--------------------------------------------------------------------+
// TinyUSB Host Callbacks (extern "C" required by TinyUSB)
//--------------------------------------------------------------------+

extern "C" {
void tuh_mount_cb(uint8_t dev_addr)
{
  UsbHost::onMount(dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr)
{
  UsbHost::onUmount(dev_addr);
}
} // extern "C"

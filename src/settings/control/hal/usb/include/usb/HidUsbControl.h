#pragma once

#include "UsbControl.h"
#include <cstddef>


struct hid_device_;
typedef hid_device_ hid_device;

class HidUsbControl : public UsbControl {
public:
    HidUsbControl(uint16_t vendorId, uint16_t productId);
    ~HidUsbControl() override = default;

    ResultCode initialise() override;
    bool discover() override;
    ResultCode open() override;
    void close() override;
    void exit() override;

    ResultCode read(unsigned char * data, size_t length, size_t* bytesRead) const;
    ResultCode write(const unsigned char * data, size_t length, size_t* bytesWritten) const;

protected:
#ifdef LINUX_HIDAPI
    hid_device* m_device;
#endif
};

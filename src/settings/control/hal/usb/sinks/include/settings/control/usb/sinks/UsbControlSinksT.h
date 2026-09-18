#pragma once
#include <ResultCode.h>
#include <config/struct/UsbControlSinksConfig.h>
#include <settings/control/sink/SettingsControlSinkT.h>
#include "settings/control/usb/sinks/UsbControlSinkTypesT.h"
#include "settings/control/usb/sinks/UsbControlSinkFactoryT.h"

template <typename RadioSettingsT>
class UsbControlSinksT : public SettingsControlSinkT<RadioSettingsT>, public FieldUpdateSink
{
public:
  UsbControlSinksT() = default;
  ~UsbControlSinksT() override = default;

  UsbControlSinksT(UsbControlSinksT&&)  noexcept = default;
  UsbControlSinksT& operator=(UsbControlSinksT&&)  noexcept = default;

  // ControlBase overrides;
  ResultCode configure(const Config::UsbControlSinks::Fields& config) { return createDevices(config); }
  bool discover() override { return true; }
  ResultCode open() override
  {
    for (auto& device : m_devices) {
      ResultCode rc = visit([](auto&& dev) -> ResultCode
      {
        ResultCode devRc = dev.open();
        if (devRc == ResultCode::ERR_USB_NOT_MOUNTED) {
          return ResultCode::OK;
        }
        return devRc;
      }, device);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  void close() override
  {
    for (auto& device : m_devices) {
      visit([](auto&& dev)
      {
        dev.close();
      }, device);
    }
  }
  void exit() override {}

  ResultCode applySettings(RadioSettingsT& settings) override
  {
    for (auto& device : m_devices) {
      const ResultCode rc = visit([&settings] (auto&& dev) -> ResultCode
      {
        return dev.applySettings(settings);
      }, device);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }
  ResultCode applyFieldUpdate(const FieldUpdate& settingDelta) override
  {
    for (auto& device : m_devices) {
      const ResultCode rc = visit([&settingDelta] (auto&& dev) -> ResultCode
      {
        return dev.applyFieldUpdate(settingDelta);
      }, device);
      if (rc != ResultCode::OK) {
        return rc;
      }
    }
    return ResultCode::OK;
  }

  ResultCode ptt(bool on) override
  {
    for (auto& device : m_devices)
    {
      visit([on](auto&& dev)-> void
      {
        dev.ptt(on);
      }, device) ;
    }
    return ResultCode::OK;
  }

protected:
  ResultCode createDevices(const Config::UsbControlSinks::Fields& config)
  {
    m_devices.clear();
    ResultCode rc = ResultCode::OK;
    for (const auto& deviceConfig : config.devices) {
      m_devices.emplace_back();
      rc = UsbControlSinkFactoryT<RadioSettingsT>::create(deviceConfig, m_devices.back());
      if (rc != ResultCode::OK) {
        m_devices.pop_back();
        return rc;
      }
    }
    return rc;
  }

  UsbControlSinkTypesT<RadioSettingsT>::VariantVector m_devices;
};


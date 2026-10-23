#pragma once
#include "IRadioSettingsMirror.h"
#include "RadioSettingsViewT.h"

template<typename MirrorT>
class RadioSettingsMirrorPolymorphicT : public IRadioSettingsMirror, public MirrorT
{
public:
  using MirrorT::MirrorT;
  ResultCode start() override { return MirrorT::start(); }
  void stop() override { MirrorT::stop(); }
  RadioSettingsUpdater* updater() override { return MirrorT::updater(); }
  ResultCode requestSettings() override { return MirrorT::requestSettings(); }
  [[nodiscard]] const IRadioSettingsView* view() const override { return &m_view; }

private:
  RadioSettingsViewT<typename MirrorT::RadioSettings> m_view{MirrorT::settings()};
};

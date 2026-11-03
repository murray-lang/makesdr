#pragma once
#include "IRadioSettingsView.h"
#include "ActiveBandSettingsViewT.h"

// Adapts a concrete RadioSettings flavour to IRadioSettingsView.
// Header-only and only instantiated by the front end, alongside RadioSettingsMirrorPolymorphicT.
template<typename RadioSettingsT>
class RadioSettingsViewT final : public IRadioSettingsView
{
public:
  using RadioSettings = RadioSettingsT;
  using ActiveBandSettings = typename RadioSettingsT::ActiveBandSettings;

  // pState: the accumulated settings that 'settings' are changes to, or nullptr if 'settings' is the state.
  explicit RadioSettingsViewT(const RadioSettingsT& settings, const IRadioSettingsView* pState = nullptr)
    : m_settings(settings)
    , m_pState(pState)
    , m_activeBands(
        *settings.activeBands(),
        pState != nullptr ? pState->activeBands() : nullptr,
        settings.traits().hasTx
      )
  {}
  RadioSettingsViewT(const RadioSettingsViewT&) = delete;
  RadioSettingsViewT& operator=(const RadioSettingsViewT&) = delete;

  [[nodiscard]] RadioSettingsTraits traits() const override { return m_settings.traits(); }

  [[nodiscard]] const IRadioSettingsView* state() const override { return m_pState != nullptr ? m_pState : this; }

  [[nodiscard]] bool hasActiveBands() const override { return m_settings.hasActiveBands(); }
  [[nodiscard]] const IActiveBandSettingsView* activeBands() const override { return &m_activeBands; }

  [[nodiscard]] bool hasPtt() const override { return m_settings.hasPtt(); }
  [[nodiscard]] bool ptt() const override { return m_settings.ptt(); }

  [[nodiscard]] const RadioSettingsT& settings() const { return m_settings; }

private:
  const RadioSettingsT& m_settings;
  const IRadioSettingsView* m_pState;
  ActiveBandSettingsViewT<ActiveBandSettings> m_activeBands;
};

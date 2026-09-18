#pragma once

#include <QWidget>
#include <memory>

#include <settings/model/radios/PipelineId.h>
#include <settings/model/radios/SplitBandId.h>
#include <settings/model/radios/IRadioSettingsUpdater.h>
#include <settings/model/radios/iq/RxTxDualIqBandSettings.h>

namespace Ui { class QtMiniVfoToolbar; }

class QtMiniVfoToolbar final : public QWidget
{
  Q_OBJECT
public:
  explicit QtMiniVfoToolbar(IRadioSettingsUpdater* radioControl, QWidget* parent = nullptr);
  ~QtMiniVfoToolbar() override;

  void setContext(SplitBandId bandId, VfoId vfoId);
  void applyBandSettings(const RxTxDualIqBandSettings* bandSettings, bool isFocusBand, bool isTxBand);

  void setMuted(bool muted);
  [[nodiscard]] bool isMuted() const noexcept { return m_muted; }

  void setAbActive(bool active);          // dual pipeline active? (presentation override)
  [[nodiscard]] bool isAbActive() const noexcept { return m_abActive; }

  void setTxActive(bool active);          // purely presentation (optional)
  void setModeText(const QString& text);  // e.g. "USB"

  signals:
    //   // Mode button still needs a chooser UI somewhere; emit this to open it.
    void modeSelectRequested(SplitBandId bandId);

private:

  [[nodiscard]] bool getIsMuted(const RxTxDualIqBandSettings* bandSettings) const noexcept;
  void setFixedProperties();
  void updateMutePresentation();
  void updateAbPresentation();
  void refreshFromSettings(bool isFocusBand, bool isTxBand);

  void onAbPressed();
  void onTxPressed();
  void onModePressed();
  void onMutePressed();

  [[nodiscard]] bool hasValidContext() const noexcept;
  void lockAbButtonWidthToIconState();

  std::unique_ptr<Ui::QtMiniVfoToolbar> ui;
  IRadioSettingsUpdater* m_radioControl;
  const RxTxDualIqBandSettings* m_bandSettings = nullptr;
  SplitBandId m_bandId = SplitBandId::None;
  VfoId m_vfoId = VfoId::A;

  bool m_muted = false;
  bool m_abActive = false;
};
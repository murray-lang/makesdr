#pragma once

#include <QWidget>
#include <string>

#include <ui/qt/widgets/QtNumberReadout.h>
#include "VfoActions.h"
#include <samples/SampleTypes.h>
#include <settings/model/radios/PipelineId.h>
#include <settings/model/radios/SplitBandId.h>
#include <settings/model/radios/iq/RxTxDualIqBandSettings.h>
#include <settings/model/radios/IRadioSettingsUpdater.h>

class QtMiniVfoToolbar;

class QtVfoReadout final : public QWidget
{
  Q_OBJECT
public:
  explicit QtVfoReadout(IRadioSettingsUpdater* radioControl, VfoId id, QWidget* parent = nullptr);
  ~QtVfoReadout() override = default;

  // Band context so the embedded toolbar can generate correct FieldUpdates.
  void setBandId(SplitBandId bandId);
  void applyBandSettings(const RxTxDualIqBandSettings* bandSettings, bool isLoneVfo, bool isFocusBand, bool isTxBand);
  void setModeText(const QString& text);

  // Forwarded API (public surface = “a VFO cell”, not “a number label”)
  void setValue(int64_t value);
  void setValue(sdrreal value);
  void setValueText(const std::string& value);


  void setMuted(bool muted);

  void setMultiVfoAction(MultiVfoAction mode);
  [[nodiscard]] MultiVfoAction multiVfoAction() const noexcept { return m_multiVfoAction; }

  void setVfoTxAction(VfoTxAction mode);
  [[nodiscard]] VfoTxAction vfoTxAction() const noexcept { return m_vfoTxAction; }

  void setWidgetProperties(bool repolish = true);
  void setPttProperty(bool ptt, bool repolish = true);

  signals:
    // void muteToggledRequested(VfoId id, bool muted);
    //
    // // Legacy UI-level signals (can be removed once the panel is fully FieldUpdate-driven)
    // void multiVfoActionRequested(MultiVfoAction mode);
    // void vfoTxActionRequested(VfoTxAction mode);

    // void clicked(VfoId id);

protected:
  void mousePressEvent(QMouseEvent* e) override;
  bool event(QEvent* e) override;
  bool eventFilter(QObject* watched, QEvent* e) override;

private:
  void buildUi();
  [[nodiscard]] bool isInteractiveChildAt(const QPoint& localPos) const;
  void updateFixedWidth();
  void scheduleWidthRecompute();
  void applyFocusAndTx(bool focus, bool tx);

  IRadioSettingsUpdater* m_radioControl;
  VfoId m_id;
  bool m_hasFocus;
  bool m_isTx;

  SplitBandId m_splitBandId = SplitBandId::None;

  QtNumberReadout* m_readout = nullptr;
  QtMiniVfoToolbar* m_toolbar = nullptr;

  MultiVfoAction m_multiVfoAction = MultiVfoAction::None;
  VfoTxAction m_vfoTxAction = VfoTxAction::None;
};
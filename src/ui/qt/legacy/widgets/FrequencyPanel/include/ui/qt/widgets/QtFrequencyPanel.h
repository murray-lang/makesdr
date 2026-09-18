//
// Created by murray on 28/1/26.
//

#pragma once
#include <QObject>
#include <QWidget>
#include <memory>

#include "QtBandReadout.h"
#include <ui/qt/widgets/QtVfoReadout.h>
// #include <settings/model/radios/iq/SplitBandDualIqActiveBandSettings.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

namespace Ui { class QtFrequencyPanel; }

class QtFrequencyPanel : public QWidget //, public FieldUpdateSource
{
  Q_OBJECT
public:
  using RadioSettings = SplitBandDualIqRxTxSettings;
  using ActiveBandSettings = RadioSettings::ActiveBandSettings;

  explicit QtFrequencyPanel(IRadioSettingsUpdater* updater, QWidget* parent = nullptr);
  ~QtFrequencyPanel() override;

  void initialise(const RadioSettings* pRadioSettings);
  void applyRadioSettings(const RadioSettings* pRadioSettings, bool onlyIfChanged = true);

// private slots:
//   void onSplitRequested(SplitBandId whichBand);
//   void onCloseRequested(SplitBandId whichBand);
//   void onMultiVfoActionRequested(SplitBandId whichBand,
//                             VfoId whichVfo,
//                             MultiVfoAction action);
//   void onVfoTxActionRequested(SplitBandId whichBand,
//                             VfoId whichVfo,
//                             VfoTxAction action);
//   void onBandClicked(SplitBandId whichBand);
//   void onVfoClicked(SplitBandId whichBand, VfoId whichVfo);
//   void onTxBandClicked(SplitBandId whichBand);

private:
  void initialiseLayout();
  void applyFrequencyChanges(const ActiveBandSettings* bandSelector, bool onlyIfChanged = true);
  void applyFrequencyAndPipelineChanges(const ActiveBandSettings* bandSelector, bool onlyIfChanged = true);
  void applyBandSelectorChange(const ActiveBandSettings* bandSelector);

  bool hasChangesOtherThanVfo(const ActiveBandSettings* bandSelector) const;

  void setPttProperty(bool ptt, bool repolish = true);
  void setIsSplitProperty(bool isSplit, bool repolish = true);

  void updateRowActionModes(bool hasBand1, bool hasBand2, bool isSplit);

private:
  std::unique_ptr<Ui::QtFrequencyPanel> ui;
  IRadioSettingsUpdater* m_radioUpdater;
  QtBandReadout* m_band1Readout;
  QtBandReadout* m_band2Readout;
};
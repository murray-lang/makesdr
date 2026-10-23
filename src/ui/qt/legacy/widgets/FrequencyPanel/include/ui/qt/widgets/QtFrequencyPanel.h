//
// Created by murray on 28/1/26.
//

#pragma once
#include <QObject>
#include <QWidget>
#include <memory>

#include "QtBandReadout.h"
#include <ui/qt/widgets/QtVfoReadout.h>
#include <settings/model/radio/IRadioSettingsView.h>

#include <settings/model/radio/RadioSettingsTraits.h>

namespace Ui { class QtFrequencyPanel; }

class QtFrequencyPanel : public QWidget //, public FieldUpdateSource
{
  Q_OBJECT
public:
  explicit QtFrequencyPanel(const RadioSettingsTraits& traits, IRadioSettingsUpdater* updater, QWidget* parent = nullptr);
  ~QtFrequencyPanel() override;

  void initialise(const IRadioSettingsView* pRadioSettings);
  void applyRadioSettings(const IRadioSettingsView* pRadioSettings, bool onlyIfChanged = true);

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
  void applyFrequencyChanges(const IActiveBandSettingsView* bandSelector, bool onlyIfChanged = true);
  void applyFrequencyAndPipelineChanges(const IActiveBandSettingsView* bandSelector, bool onlyIfChanged = true);
  void applyBandSelectorChange(const IActiveBandSettingsView* state);

  bool hasChangesOtherThanVfo(const IActiveBandSettingsView* bandSelector) const;

  void setPttProperty(bool ptt, bool repolish = true);
  void setIsSplitProperty(bool isSplit, bool repolish = true);

  void updateRowActionModes(bool hasBand1, bool hasBand2, bool isSplit);

private:
  std::unique_ptr<Ui::QtFrequencyPanel> ui;
  RadioSettingsTraits m_traits;
  IRadioSettingsUpdater* m_radioUpdater;
  QtBandReadout* m_band1Readout;
  QtBandReadout* m_band2Readout;
};
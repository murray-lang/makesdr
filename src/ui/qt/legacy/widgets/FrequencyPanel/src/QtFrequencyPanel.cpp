#include "ui/qt/widgets/QtFrequencyPanel.h"
#include "ui_QtFrequencyPanel.h"

#include "ui/qt/widgets/QtBandReadout.h"
#include <ui/qt/widgets/QtVfoReadout.h>

#include <QStyle>



QtFrequencyPanel::QtFrequencyPanel(IRadioSettingsUpdater* updater, QWidget* parent)
  : QWidget(parent)
  , ui(std::make_unique<Ui::QtFrequencyPanel>())
  , m_radioUpdater(updater)
  , m_band1Readout(nullptr)
  , m_band2Readout(nullptr)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_StyledBackground, true);
  initialiseLayout();
}

QtFrequencyPanel::~QtFrequencyPanel() = default;

void
QtFrequencyPanel::initialiseLayout()
{
  // Keep using the .ui gridLayout as a simple 2-row host.
  ui->gridLayout->setContentsMargins(0, 0, 0, 0);
  ui->gridLayout->setHorizontalSpacing(6);
  ui->gridLayout->setVerticalSpacing(2);
  ui->gridLayout->setColumnStretch(0, 1);
  ui->gridLayout->setColumnStretch(1, 1);

  m_band1Readout = new QtBandReadout(m_radioUpdater, SplitBandId::One, this);
  // m_band1Readout->setObjectName("band1Row");
  ui->gridLayout->addWidget(m_band1Readout, 0, 0, 1, 2, Qt::AlignLeft);

  m_band2Readout = new QtBandReadout(m_radioUpdater, SplitBandId::Two, this);
  // m_band2Readout->setObjectName("band2Row");
  ui->gridLayout->addWidget(m_band2Readout, 1, 0, 1, 2, Qt::AlignLeft);

  m_band1Readout->setWidgetProperties(false);
  m_band2Readout->setWidgetProperties(false);
  QWidgetPropertySetter::setWidgetProperty(this, "role", "frequencyReadout", true);
}

void
QtFrequencyPanel::initialise(const SplitBandDualIqRxTxSettings* pRadioSettings)
{
  applyRadioSettings(pRadioSettings, false);
}

void
QtFrequencyPanel::applyRadioSettings(const SplitBandDualIqRxTxSettings* pRadioSettings, bool onlyIfChanged)
{
  if (pRadioSettings == nullptr) return;

  if (pRadioSettings->hasActiveBands()) {
    const ActiveBandSettings* activeBandSettings = pRadioSettings->activeBands();
    if (hasChangesOtherThanVfo(activeBandSettings)) {
      applyBandSelectorChange(activeBandSettings);
      applyFrequencyAndPipelineChanges(activeBandSettings, onlyIfChanged);
    } else {
      applyFrequencyChanges(activeBandSettings, onlyIfChanged);
    }
    // BandSettings* focusBand = activeBandSettings.getFocusBandSettings();
    // if (onlyIfChanged && focusBand != nullptr && focusBand->hasFocusVfoChanged()) {
    //   // Do the bare minimum for VFO changes otherwise Qt stuff bogs everything down.
    //   applyFrequencyChanges(activeBandSettings, onlyIfChanged);
    // } else {
    //
    // }
    //
  } else if (pRadioSettings->hasPtt()) {
    setPttProperty(pRadioSettings->ptt());
  }
}

bool
QtFrequencyPanel::hasChangesOtherThanVfo(const ActiveBandSettings* activeBandSettings) const
{
  if (   activeBandSettings->hasFocusBandId()
      || activeBandSettings->hasTxBandId()
      || activeBandSettings->hasRxBandId()
      || activeBandSettings->hasIsSplit()
      ) {
    return true;
  }
  if (activeBandSettings->hasBand(SplitBandId::One)) {
    const ActiveBandSettings::BandSettings* band1 = activeBandSettings->band(SplitBandId::One);
    if (band1->hasFocusPipeline() || band1->hasIsMultiPipeline() || band1->hasTxPipeline()) {
      return true;
    }
  }
  if (activeBandSettings->hasBand(SplitBandId::Two)) {
    const ActiveBandSettings::BandSettings* band2 = activeBandSettings->band(SplitBandId::Two);
    if (band2->hasFocusPipeline() || band2->hasIsMultiPipeline() || band2->hasTxPipeline()) {
      return true;
    }
  }
  return false;
}

void
QtFrequencyPanel::applyFrequencyChanges(const ActiveBandSettings* activeBandSettings, bool onlyIfChanged)
{
  // Band 1
  if (activeBandSettings->hasBand(SplitBandId::One)) {
    const ActiveBandSettings::BandSettings* band1 = activeBandSettings->band(SplitBandId::One);
    if (band1 != nullptr) {
      m_band1Readout->applyFrequencyChanges(band1, onlyIfChanged);
    }
  }

  // Band 2 (may be absent when not split)
  if (activeBandSettings->hasBand(SplitBandId::Two)) {
    const ActiveBandSettings::BandSettings* band2 = activeBandSettings->band(SplitBandId::Two);
    if (band2 != nullptr) {
      m_band2Readout->applyFrequencyChanges(band2, onlyIfChanged);
    }
  }
}

void
QtFrequencyPanel::applyFrequencyAndPipelineChanges(const ActiveBandSettings* activeBandSettings, bool onlyIfChanged)
{
  // Band 1
  if (activeBandSettings->hasBand(SplitBandId::One)) {
    const ActiveBandSettings::BandSettings* band1 = activeBandSettings->band(SplitBandId::One);
    if (band1 != nullptr) {
      m_band1Readout->applyFrequencyChanges(band1, onlyIfChanged);
      m_band1Readout->applyPipelineChanges(band1, onlyIfChanged);
    }
  }

  // Band 2 (may be absent when not split)
  if (activeBandSettings->hasBand(SplitBandId::Two)) {
    const ActiveBandSettings::BandSettings* band2 = activeBandSettings->band(SplitBandId::Two);
    if (band2 != nullptr) {
      m_band2Readout->applyFrequencyChanges(band2, onlyIfChanged);
      m_band2Readout->applyPipelineChanges(band2, onlyIfChanged);
    }
  }
}

void
QtFrequencyPanel::applyBandSelectorChange(const ActiveBandSettings* activeBandSettings)
{
  
  const SplitBandId txBandId = activeBandSettings->txBandId();
  const SplitBandId rxBandId = activeBandSettings->rxBandId();
  const SplitBandId focusBandId = activeBandSettings->focusBandId();

  const bool isSplit = activeBandSettings->isSplit();
  bool showBand1 = (activeBandSettings->hasBand(SplitBandId::One) && focusBandId == SplitBandId::One) || isSplit;
  bool showBand2 = (activeBandSettings->hasBand(SplitBandId::Two) && focusBandId == SplitBandId::Two) || isSplit;

  // Visibility (Band 2 row disappears when not split)
  m_band1Readout->setRowVisible(showBand1);
  m_band2Readout->setRowVisible(showBand2);

  if (showBand1) {
    const ActiveBandSettings::BandSettings* band1 = activeBandSettings->band(SplitBandId::One);
    m_band1Readout->applyBandSettings(band1, SplitBandId::One, txBandId, rxBandId, focusBandId);
    // m_band1Readout->setTxButtonVisible(isSplit);
  }

  if (showBand2) {
    const ActiveBandSettings::BandSettings* band2 = activeBandSettings->band(SplitBandId::Two);
    m_band2Readout->applyBandSettings(band2, SplitBandId::Two, txBandId, rxBandId, focusBandId);
    // m_band2Readout->setTxButtonVisible(isSplit);
  }

  // Vertically center the single band by spanning both rows
  if (ui && ui->gridLayout && m_band1Readout && m_band2Readout) {
    ui->gridLayout->removeWidget(m_band1Readout);
    ui->gridLayout->removeWidget(m_band2Readout);

    if (showBand1 && !showBand2) {
      ui->gridLayout->addWidget(m_band1Readout, 0, 0, 2, 2, Qt::AlignVCenter | Qt::AlignLeft);
      ui->gridLayout->addWidget(m_band2Readout, 1, 0, 1, 2, Qt::AlignLeft); // hidden anyway
    } else if (!showBand1 && showBand2) {
      ui->gridLayout->addWidget(m_band1Readout, 1, 0, 1, 2, Qt::AlignLeft); // hidden anyway
      ui->gridLayout->addWidget(m_band2Readout, 0, 0, 2, 2, Qt::AlignVCenter | Qt::AlignLeft);
    } else {
      ui->gridLayout->addWidget(m_band1Readout, 0, 0, 1, 2, Qt::AlignLeft);
      ui->gridLayout->addWidget(m_band2Readout, 1, 0, 1, 2, Qt::AlignLeft);
    }
  }

  updateRowActionModes(showBand1, showBand2, isSplit);

  setIsSplitProperty(isSplit, true);

  ui->gridLayout->setRowStretch(0, showBand1 ? 1 : 0);
  ui->gridLayout->setRowStretch(1, showBand2 ? 1 : 0);
  ui->gridLayout->invalidate();
  updateGeometry();
}

void
QtFrequencyPanel::updateRowActionModes(bool hasBand1, bool hasBand2, bool isSplit)
{
  // With the row widget, the gutters are fixed and buttons are icon-only,
  // so there is no horizontal jostling; we just change the action mode.

  if (!m_band1Readout || !m_band2Readout) return;

  if (isSplit && hasBand1 && hasBand2) {
    m_band1Readout->setBandActionMode(QtBandReadout::BandAction::Close);
    m_band2Readout->setBandActionMode(QtBandReadout::BandAction::Close);
    return;
  }

  // Not split
  if (hasBand1 && !hasBand2) {
    m_band1Readout->setBandActionMode(QtBandReadout::BandAction::Split);
    m_band2Readout->setBandActionMode(QtBandReadout::BandAction::Disabled);
    return;
  }
  if (!hasBand1 && hasBand2) {
    m_band1Readout->setBandActionMode(QtBandReadout::BandAction::Disabled);
    m_band2Readout->setBandActionMode(QtBandReadout::BandAction::Split);
    return;
  }

  // Fallback: disable actions (should be rare)
  m_band1Readout->setBandActionMode(QtBandReadout::BandAction::Disabled);
  m_band2Readout->setBandActionMode(QtBandReadout::BandAction::Disabled);
}

// void
// QtFrequencyPanel::onSplitRequested(SplitBandId /*whichBand*/)
// {
//   m_radioControl->split();
// }
//
// void
// QtFrequencyPanel::onCloseRequested(SplitBandId whichBand)
// {
//   if (m_pSettingsSink == nullptr) return;
//
//   ActiveBandSettings::Features select =
//     whichBand == SplitBandId::One ? ActiveBandSettings::SELECT_1 : ActiveBandSettings::SELECT_2;
//
//   FieldUpdatePath bandPath({RadioSettings::BAND, static_cast<uint32_t>(select)});
//   FieldUpdate bandSetting(bandPath, BandNameString(""), FieldUpdateMeaning::VALUE); // Empty band name closes it
//   m_pSettingsSink->applyFieldUpdate(bandSetting);
// }
//
// void
// QtFrequencyPanel::onMultiVfoActionRequested(SplitBandId whichBand,
//                                         VfoId whichVfo,
//                                         MultiVfoAction action)
// {
//   uint32_t selectBand = whichBand == SplitBandId::One ? ActiveBandSettings::WITH_1 : ActiveBandSettings::WITH_2;
//
//   switch (action) {
//   case MultiVfoAction::Multi:
//     {
//       FieldUpdatePath bandPath({
//         RadioSettings::BAND,
//         selectBand,
//         BandSettings::MULTI_PIPELINE
//       });
//       FieldUpdate bandSetting(bandPath, true, FieldUpdateMeaning::VALUE);
//       m_pSettingsSink->applyFieldUpdate(bandSetting);
//       break;
//     }
//   case MultiVfoAction::Close:
//     {
//       FieldUpdatePath bandPath({
//         RadioSettings::BAND,
//         selectBand,
//         BandSettings::CLOSE_PIPELINE
//       });
//       FieldUpdate bandSetting(bandPath, whichVfo, FieldUpdateMeaning::VALUE);
//       m_pSettingsSink->applyFieldUpdate(bandSetting);
//       break;
//     }
//   }
// }
//
// void
// QtFrequencyPanel::onVfoTxActionRequested(SplitBandId whichBand,
//                             VfoId whichVfo,
//                             VfoTxAction action)
// {
//   uint32_t selectBand = whichBand == SplitBandId::One ? ActiveBandSettings::WITH_1 : ActiveBandSettings::WITH_2;
//   if (action == VfoTxAction::Tx) {
//     FieldUpdatePath bandPath({
//         RadioSettings::BAND,
//         selectBand,
//         BandSettings::TX_PIPELINE
//       });
//     FieldUpdate bandSetting(bandPath, whichVfo, FieldUpdateMeaning::VALUE);
//     m_pSettingsSink->applyFieldUpdate(bandSetting);
//   }
//
//   switch (action) {
//   case VfoTxAction::Tx:
//     {
//       FieldUpdatePath bandPath({
//         RadioSettings::BAND,
//         selectBand,
//         BandSettings::MULTI_PIPELINE
//       });
//       FieldUpdate bandSetting(bandPath, true, FieldUpdateMeaning::VALUE);
//       m_pSettingsSink->applyFieldUpdate(bandSetting);
//       break;
//     }
//
//   }
// }
//
void
QtFrequencyPanel::setPttProperty(bool ptt, bool repolish)
{
  m_band1Readout->setPttProperty(ptt, false);
  m_band2Readout->setPttProperty(ptt, false);
  QWidgetPropertySetter::setWidgetProperty(this, "ptt", ptt, repolish);
}

void
QtFrequencyPanel::setIsSplitProperty(bool isSplit, bool repolish)
{
  if (m_band1Readout != nullptr) m_band1Readout->setIsBandSplitProperty(isSplit, false);
  if (m_band2Readout != nullptr) m_band2Readout->setIsBandSplitProperty(isSplit, false);
  QWidgetPropertySetter::setWidgetProperty(this, "isSplit", isSplit, repolish);
}
//
// void
// QtFrequencyPanel::onBandClicked(SplitBandId whichBand)
// {
//   if (m_pSettingsSink == nullptr) return;
//
//   FieldUpdatePath path({RadioSettings::BAND, ActiveBandSettings::FOCUS});
//   FieldUpdate u(path, whichBand, FieldUpdateMeaning::VALUE);
//   m_pSettingsSink->applyFieldUpdate(u);
// }
//
// void
// QtFrequencyPanel::onTxBandClicked(SplitBandId whichBand)
// {
//   if (m_pSettingsSink == nullptr) return;
//
//   FieldUpdatePath path({RadioSettings::BAND, ActiveBandSettings::TX_BAND});
//   FieldUpdate update(path, whichBand, FieldUpdateMeaning::VALUE);
//   m_pSettingsSink->applyFieldUpdate(update);
// }
//
// void
// QtFrequencyPanel::onVfoClicked(SplitBandId whichBand, VfoId whichVfo)
// {
//   if (m_pSettingsSink == nullptr) return;
//
//   FieldUpdate update = FieldUpdateHelpers::makeSetFocusPipeline(whichBand, whichVfo);
//   m_pSettingsSink->applyFieldUpdate(update);
// }
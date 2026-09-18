#pragma once

#include <QWidget>
#include <QStackedLayout>

#include <settings/model/radios/PipelineId.h>
#include <settings/model/radios/SplitBandId.h>
#include <settings/model/radios/IRadioSettingsUpdater.h>
#include <settings/model/radios/iq/RxTxDualIqBandSettings.h>
#include <ui/qt/widgets/QtVfoReadout.h>
#include <ui/qt/widgets/VfoActions.h>

class QToolButton;

class QtBandReadout final : public QWidget
{
  Q_OBJECT
public:
  enum class BandAction { Split, Tx, Close, Disabled };

  explicit QtBandReadout(IRadioSettingsUpdater* radioControl, SplitBandId bandIndex, QWidget* parent = nullptr);
  ~QtBandReadout() override = default;

  void initialise();

  // Let the row forward FieldUpdates from its children to the sink,
  // without QtFrequencyPanel knowing about VFO internals.
  void connectFieldUpdateSink(FieldUpdateSink* pSink) { m_pSettingsSink = pSink; }

  [[nodiscard]] SplitBandId bandIndex() const noexcept { return m_splitBandId; }

  // VFO cells are now proper standalone widgets
  [[nodiscard]] QtVfoReadout* vfoA() const noexcept { return m_vfoA; }
  [[nodiscard]] QtVfoReadout* vfoB() const noexcept { return m_vfoB; }

  void setIsFocusBand(bool focus, bool repolish = true);
  void setIsTxBand(bool focus, bool repolish = true);
  void setRowVisible(bool visible);

  // Controls what the *left* button means for this band row.
  void setBandActionMode(BandAction mode);

  // Controls whether the right readout area shows VFO B or a placeholder
  void setVfoBVisible(bool visible);

  void applyBandSettings(
    const RxTxDualIqBandSettings* bandSettings,
    SplitBandId thisBandId,
    SplitBandId txBandId,
    SplitBandId rxBandId,
    SplitBandId focusBandId
    );

  void applyFrequencyChanges(const RxTxDualIqBandSettings* bandSettings, bool onlyIfChanged = true);
  void applyPipelineChanges(const RxTxDualIqBandSettings* bandSettings, bool onlyIfChanged = true);

  void setWidgetProperties(bool repolish = true);

  void setPttProperty(bool ptt, bool repolish = true);
  void setIsBandSplitProperty(bool isSplit, bool repolish = true);

signals:

  // Bubble up FieldUpdates from child VFO toolbars
  // void settingUpdateRequested(QVector<FieldUpdate>& updates);
  void bandClicked(SplitBandId whichBand);
  void vfoClicked(SplitBandId whichBand, VfoId whichVfo);

protected:
  void mousePressEvent(QMouseEvent* e) override;
  bool event(QEvent* e) override;

private slots:
  void onLeftActionClicked();

private:
  void buildUi();
  void updateLeftButtonPresentation();
  void updateActionGutterWidth();

  static void applyFrequencyChanges(QtVfoReadout* readout,
                                   const class RxPipelineSettings* rxPipelineSettings,
                                   bool onlyIfChanged);

  // static void applyFocus(QtVfoReadout* w, bool focusVfo, bool focusRx, bool focusTx);

  static constexpr int kActionSlotWidthPx = 28;

  IRadioSettingsUpdater* m_radioControl;

  SplitBandId m_splitBandId;

  // Layout pieces
  QToolButton* m_addRemoveBandButton = nullptr;

  QtVfoReadout* m_vfoA = nullptr;
  QtVfoReadout* m_vfoB = nullptr;

  QWidget* m_vfoPlaceholder = nullptr;
  QStackedLayout* m_rightStack = nullptr;

  QHBoxLayout* m_rowLayout = nullptr;
  QWidget* m_leftCell = nullptr;
  QWidget* m_rightCell = nullptr;

  QWidget* m_rightStackHost = nullptr;
  QWidget* m_actionGutter = nullptr;

  FieldUpdateSink* m_pSettingsSink = nullptr;

  BandAction m_addRemoveBandAction = BandAction::Disabled;
  BandAction m_selectTxBandAction = BandAction::Tx;
};
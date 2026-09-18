#include "ui/qt/widgets/QtMiniVfoToolbar.h"

#include <QIcon>
#include <QStyle>
#include <QToolButton>
#include <array>

#include "ui_QtMiniVfoToolbar.h"

#include <ui/qt/widgets/QWidgetPropertySetter.h>

const auto closeIconName = QStringLiteral(":/ui/icons/solid/circle-xmark.svg");
const auto mutedIconName = QStringLiteral(":/ui/icons/solid/volume-xmark-solid-full.svg");
const auto unmutedIconName = QStringLiteral(":/ui/icons/solid/volume-solid-full.svg");
const auto openIconName = QStringLiteral(":/ui/icons/solid/arrow-right.svg");
const auto abButtonText = QStringLiteral("A/B");

QtMiniVfoToolbar::QtMiniVfoToolbar(IRadioSettingsUpdater* radioControl, QWidget* parent)
  : QWidget(parent)
  , m_radioControl(radioControl)
  , ui(std::make_unique<Ui::QtMiniVfoToolbar>())
{
  setAttribute(Qt::WA_StyledBackground, true);
  setAttribute(Qt::WA_AcceptTouchEvents, true);

  ui->setupUi(this);

  ui->muteButton->setFocusPolicy(Qt::NoFocus);
  ui->abButton->setFocusPolicy(Qt::NoFocus);
  ui->txButton->setFocusPolicy(Qt::NoFocus);
  ui->modeButton->setFocusPolicy(Qt::NoFocus);

  connect(ui->muteButton, &QToolButton::clicked, this, [this] { onMutePressed(); });
  connect(ui->abButton, &QToolButton::clicked, this, [this] { onAbPressed(); });
  connect(ui->txButton, &QToolButton::clicked, this, [this] { onTxPressed(); });
  connect(ui->modeButton, &QToolButton::clicked, this, [this] { onModePressed(); });

  // lockAbButtonWidthToIconState();
  setFixedProperties();
  updateMutePresentation();
  updateAbPresentation();
  setTxActive(false);
}

QtMiniVfoToolbar::~QtMiniVfoToolbar() = default;

void
QtMiniVfoToolbar::setContext(SplitBandId bandId, VfoId vfoId)
{
  m_bandId = bandId;
  m_vfoId = vfoId;
  // refreshFromSettings();
}

void
QtMiniVfoToolbar::setFixedProperties()
{
  QWidgetPropertySetter::setWidgetProperty(ui->muteButton, "role", "mute", false);
  QWidgetPropertySetter::setWidgetProperty(ui->abButton, "role", "ab", false);
  QWidgetPropertySetter::setWidgetProperty(ui->txButton, "role", "tx", false);
  QWidgetPropertySetter::setWidgetProperty(ui->modeButton, "role", "mode", false);
}

void
QtMiniVfoToolbar::applyBandSettings(const RxTxDualIqBandSettings* bandSettings, bool isFocusBand, bool isTxBand)
{
  m_bandSettings = bandSettings;
  refreshFromSettings(isFocusBand, isTxBand);
}

void
QtMiniVfoToolbar::setMuted(bool muted)
{
  ui->muteButton->setProperty("active", muted);
  m_muted = muted;
  updateMutePresentation();
}

void
QtMiniVfoToolbar::setAbActive(bool active)
{
  if (m_abActive == active) return;
  m_abActive = active;
  updateAbPresentation();
}

void
QtMiniVfoToolbar::setTxActive(bool active)
{
  ui->txButton->setProperty("active", active);
  ui->txButton->style()->unpolish(ui->txButton);
  ui->txButton->style()->polish(ui->txButton);
  ui->txButton->update();
}

void QtMiniVfoToolbar::setModeText(const QString& text)
{
  ui->modeButton->setText(text.isEmpty() ? QStringLiteral("Mode") : text);
}

void QtMiniVfoToolbar::updateMutePresentation()
{
  ui->muteButton->setProperty("active", m_muted);
  // ui->muteButton->setText(QStringLiteral("Mute"));
  ui->muteButton->setIcon(m_muted ? QIcon(mutedIconName) : QIcon(unmutedIconName));
  ui->muteButton->style()->unpolish(ui->muteButton);
  ui->muteButton->style()->polish(ui->muteButton);
  ui->muteButton->update();
}

void QtMiniVfoToolbar::updateAbPresentation()
{
  ui->abButton->setText(abButtonText);
  ui->abButton->setProperty("active", m_abActive);

  ui->abButton->setIcon(m_abActive
    ? QIcon(closeIconName)
    : QIcon(openIconName));

  ui->abButton->style()->unpolish(ui->abButton);
  ui->abButton->style()->polish(ui->abButton);
  ui->abButton->update();
}

bool QtMiniVfoToolbar::hasValidContext() const noexcept
{
  return m_bandId != SplitBandId::None;
}

void QtMiniVfoToolbar::refreshFromSettings(bool isFocusBand, bool isTxBand)
{
  if (m_bandSettings == nullptr) {
    setAbActive(false);
    setTxActive(false);
    return;
  }
  setMuted(getIsMuted(m_bandSettings));
  setAbActive(m_bandSettings->isMultiPipeline());
  setTxActive(m_bandSettings->txPipelineId() == m_vfoId && isTxBand);
}

bool
QtMiniVfoToolbar::getIsMuted(const RxTxDualIqBandSettings* bandSettings) const noexcept
{
  const RxPipelineSettings* pipeline = bandSettings->pipeline(m_vfoId);
  return pipeline->mute();
}

void QtMiniVfoToolbar::onAbPressed()
{
  if (!hasValidContext() || m_bandSettings == nullptr) return;

  FieldUpdateVector updates;
  if (!m_bandSettings->isMultiPipeline()) {

    m_radioControl->setMultiPipeline(m_bandId, true);
    return;
  }
  m_radioControl->closePipeline(m_bandId, m_vfoId);
}

void QtMiniVfoToolbar::onTxPressed()
{
  if (!hasValidContext()) return;

  m_radioControl->setTxBand(m_bandId);
  m_radioControl->setTxPipeline(m_bandId, m_vfoId);
}

void QtMiniVfoToolbar::onModePressed()
{
  if (!hasValidContext()) return;

  // The selection UI (menu/popup) likely lives above this widget.
  // When a mode is chosen, that code should call:
  //   settingUpdateRequested(RadioSettings::makeBandSetModeOnFocusPipelineUpdate(m_bandId, modeType));
  emit modeSelectRequested(m_bandId);
}

void QtMiniVfoToolbar::onMutePressed()
{
  if (!hasValidContext()) return;

  m_radioControl->mutePipeline(m_bandId, m_vfoId, !m_muted);
}

void QtMiniVfoToolbar::lockAbButtonWidthToIconState()
{
  if (!ui || !ui->abButton) return;

  const QIcon closeIcon(closeIconName);

  const QIcon oldIcon = ui->abButton->icon();
  const QString oldText = ui->abButton->text();

  ui->abButton->setText(oldText.isEmpty() ? abButtonText : oldText);
  ui->abButton->setIcon(closeIcon);

  // Ensure any QSS-driven metrics are applied before measuring.
  ui->abButton->ensurePolished();

  const int w = ui->abButton->sizeHint().width();
  if (w > 0) {
    ui->abButton->setMinimumWidth(w);
    ui->abButton->setMaximumWidth(w);
  }

  ui->abButton->setIcon(oldIcon);
  ui->abButton->setText(oldText);
}
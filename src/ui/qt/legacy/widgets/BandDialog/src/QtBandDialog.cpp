#include "ui/qt/widgets/QtBandDialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStyle>
#include <QMouseEvent>

#include "ui_QtBandDialog.h"
#include <settings/model/data/band/BandList.h>



QtBandDialog::QtBandDialog(
  const BandCategoryList* bandCategories,
  const char * selectedBandName,
  BandUpdateCallback& bandUpdateCallback,
  QWidget *parent
  )
  : QWidget(parent) // Set flags here
  , ui(new Ui::QtBandDialog)
  , m_bandCategories(bandCategories)
  , m_bandUpdateCallback(bandUpdateCallback)
  // m_pRadio(pRadio)
{
  ui->setupUi(this);
  // setFocusPolicy(Qt::StrongFocus);
  addCategoryTabs(selectedBandName);
  ui->tabWidget->style()->polish(ui->tabWidget);
}

QtBandDialog::~QtBandDialog() {
  delete ui;
}

void
QtBandDialog::setSelectedBand(const char * bandName)
{
  updateTabs(bandName);
  updateBandButtons(bandName);
}

void
QtBandDialog::updateTabs(const char * bandName)
{
  if (bandName != nullptr) {
    int categoryIndex = m_bandCategories->findCategoryIndexOfBand(bandName);
    if (categoryIndex >= 0) {
      ui->tabWidget->setCurrentIndex(categoryIndex);
    }
  }
}

void
QtBandDialog::updateBandButtons(const char * bandName)
{
  const Band::Proto* bandProto = m_bandCategories->findBand(bandName);

  if (bandProto != nullptr) {
    Band band(*bandProto);
    updateBandButtons(&band);
  } else {
    updateBandButtons(static_cast<Band*>(nullptr));
  }

}
void
QtBandDialog::updateBandButtons(const Band* band)
{
  for (QPushButton* btn : m_bandButtons) {
    bool isSelected = band != nullptr && (btn->text().toStdString() == band->label().c_str());
    btn->setProperty("selected", isSelected);
    btn->style()->unpolish(btn);
    btn->style()->polish(btn);
  }
}


void
QtBandDialog::addCategoryTabs(const char * selectedBandName)
{
  const BandCategory* selectedCategory = m_bandCategories->findCategoryOfBand(selectedBandName);

  int selectedTabIndex = 0;
  int currentTabIndex = 0;

  const BandCategorySpan& categories = m_bandCategories->categories();
  ui->tabWidget->clear();
  for (const auto& category : categories) {
    std::string nextCategoryName = category.name;
    bool isSelected = selectedCategory != nullptr && nextCategoryName == selectedCategory->name;
    if (isSelected) {
      selectedTabIndex = currentTabIndex;
    }
    addCategoryTab(category, isSelected, selectedBandName);
    currentTabIndex++;
  }
  ui->tabWidget->setCurrentIndex(selectedTabIndex);
  ui->tabWidget->style()->unpolish(ui->tabWidget);
  ui->tabWidget->style()->polish(ui->tabWidget);
}
void
QtBandDialog::addCategoryTab(const BandCategory& category, bool isSelected, const char * selectedBandName)
{
  QWidget* tab = new QWidget();
  tab->setProperty("class", "toolbarDialogTab");

  QGridLayout* layout = new QGridLayout(tab);
  layout->setContentsMargins(9, 9, 9, 9);
  layout->setSpacing(1);
  layout->setVerticalSpacing(10);

  const int columns = 5;
  int row = 0;
  int col = 0;

  BandList bandList(category.bands);
  for (const auto& bandProto : bandList.bands()) {
    Band band(bandProto);
    auto* bandBtn = new QPushButton(QString::fromStdString(band.label().c_str()));
    m_bandButtons.append(bandBtn);
    bandBtn->setMaximumHeight(40);
    bandBtn->setMaximumWidth(70);
    bandBtn->setProperty("class", "toolbarDialogButton");

    bool isSelectedBand = band.name() == selectedBandName;
    bandBtn->setProperty("selected", isSelectedBand);
    bandBtn->style()->unpolish(bandBtn);
    bandBtn->style()->polish(bandBtn);

    connect(bandBtn, &QPushButton::clicked, this, [this, band, bandBtn]() {
      for (QPushButton* btn : m_bandButtons) {
        if (btn->property("selected").toBool()) {
          btn->setProperty("selected", false);
          btn->style()->unpolish(btn);
          btn->style()->polish(btn);
        }
      }
      bandBtn->setProperty("selected", true);
      bandBtn->style()->unpolish(bandBtn);
      bandBtn->style()->polish(bandBtn);

      m_bandUpdateCallback(band.name().c_str());
      // this->close();
    });

    layout->addWidget(bandBtn, row, col);

    col++;
    if (col >= columns) {
      col = 0;
      row++;
    }
  }

  // Add a spacer to push buttons to the top
  layout->setRowStretch(row + 1, 1);

  ui->tabWidget->addTab(tab, QString::fromStdString(category.label));
}
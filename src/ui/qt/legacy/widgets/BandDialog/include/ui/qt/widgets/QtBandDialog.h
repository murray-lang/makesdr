#pragma once
#include <QDialog>

#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/data/band/BandCategoryList.h>
#include <settings/model/radios/Band.h>

using BandCategory = makesdr_BandCategoryPb;

class Bands;

namespace Ui
{
  class QtBandDialog;
}

using BandUpdateCallback = std::function<void(const char* bandName)>;

template<typename T, void (T::*MemberFn)(const char* bandName)>
inline BandUpdateCallback makeBandUpdateCallback(T* self)
{
  return [self](const char* bandName) {
    (self->*MemberFn)(bandName);
  };
}

class QtBandDialog : public QWidget
{
  Q_OBJECT
public:
  explicit QtBandDialog(
    const BandCategoryList* bandCategories,
    const char * selectedBandName,
    BandUpdateCallback& bandUpdateCallback,
    QWidget *parent = nullptr);

  ~QtBandDialog() override;

  void setSelectedBand(const char * bandName);

protected:
  void addCategoryTabs(const char * selectedBandName);
  void addCategoryTab(const BandCategory& category, bool isSelected, const char * selectedBandName);
  void updateTabs(const char * bandName);
  void updateBandButtons(const char * bandName);
  void updateBandButtons(const Band* band);

private:
  Ui::QtBandDialog *ui;
  const BandCategoryList* m_bandCategories;
  BandUpdateCallback& m_bandUpdateCallback;
  QList<QPushButton*> m_bandButtons;
};
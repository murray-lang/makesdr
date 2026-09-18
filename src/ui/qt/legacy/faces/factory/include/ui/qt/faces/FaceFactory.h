#pragma once

#include <ui/qt/faces/FaceBase.h>
#include <config/struct/UiConfig.h>

#ifdef USE_ETL
#include "etl/unordered_map.h"

using FaceCreator = etl::function<unique_ptr<FaceBase>(QWidget* parent)>;
using FaceMap = etl::unordered_map<Config::Ui::FaceString, FaceCreator, 3>;
#else
using FaceCreator = function<unique_ptr<FaceBase>(QWidget* parent)>;
using FaceMap = std::unordered_map<Config::Ui::FaceString, FaceCreator>;
#endif

class FaceFactory {
public:
  static constexpr const char* defaultName = "standard";

  // using Creator = std::function<std::unique_ptr<FaceBase>(QWidget* parent)>;

  FaceFactory();
  static FaceFactory& instance();

  void registerFace(const Config::Ui::FaceString& name, const FaceCreator& creator);
  unique_ptr<FaceBase> create(const Config::Ui::FaceString& name, QWidget* parents) const;


private:
  FaceMap m_creators;
};

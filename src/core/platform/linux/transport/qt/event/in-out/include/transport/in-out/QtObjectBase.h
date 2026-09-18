#pragma once
#include <QObject>


class QtObjectBase : public QObject
{
  Q_OBJECT
public:
  QtObjectBase()
      : QObject()
  {}
};

#ifndef QT_RX_IQ_EVENT_H
#define QT_RX_IQ_EVENT_H
#include <QEvent>
#include <event/sample/RxIqEventT.h>
#include "../QtEventRegistrar.h"

using RxIqEvent = RxIqEventT<QEvent, QEvent::Type>;

#endif // QT_RX_IQ_EVENT_H
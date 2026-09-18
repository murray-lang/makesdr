#ifndef QT_TX_IQ_EVENT_H
#define QT_TX_IQ_EVENT_H
#include <QEvent>
#include <event/sample/TxIqEventT.h>
#include "../QtEventRegistrar.h"

using TxIqEvent = TxIqEventT<QEvent, QEvent::Type>;

#endif // QT_TX_IQ_EVENT_H
#ifndef QT_IQ_RX_METER_EVENT_H
#define QT_IQ_RX_METER_EVENT_H
#include <QEvent>
#include "../QtEventRegistrar.h"
#include <event/metering/RxIqMeterEventT.h>

using RxIqMeterEvent = RxIqMeterEventT<QEvent, QEvent::Type>;

#endif // QT_IQ_RX_METER_EVENT_H
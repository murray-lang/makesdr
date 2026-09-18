#ifndef QT_RX_METER_EVENT_H
#define QT_RX_METER_EVENT_H
#include <QEvent>
#include <event/metering/RxMeterEventT.h>
#include "../QtEventRegistrar.h"

using RxMeterEvent = RxMeterEventT<QEvent, QEvent::Type>;

#endif // IQ_RX_METER_EVENT_H
#ifndef QT_RXAUDIO_EVENT_H
#define QT_RXAUDIO_EVENT_H

#include <QEvent>
#include <event/sample/RxAudioEventT.h>
#include "../QtEventRegistrar.h"

using RxAudioEvent = RxAudioEventT<QEvent, QEvent::Type>;

#endif // QT_RXAUDIO_EVENT_H



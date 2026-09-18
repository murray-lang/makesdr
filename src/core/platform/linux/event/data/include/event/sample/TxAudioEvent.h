#ifndef QT_TXAUDIO_EVENT_H
#define QT_TXAUDIO_EVENT_H

#include <QEvent>
#include <event/sample/TxAudioEventT.h>
#include "../QtEventRegistrar.h"

using TxAudioEvent = TxAudioEventT<QEvent, QEvent::Type>;

#endif // QT_TXAUDIO_EVENT_H

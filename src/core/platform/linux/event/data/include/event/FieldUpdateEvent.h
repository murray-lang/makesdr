#pragma once
#include <settings/model/message/FieldUpdateEventT.h>
#include <QEvent>
#include "QtEventRegistrar.h"

using FieldUpdateEvent = FieldUpdateEventT<QEvent, QEvent::Type>;

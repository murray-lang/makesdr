#pragma once

#include <transport/RxMeteringClientSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtRxMeteringMessageRings.h"

// The client's meta side on Qt, notified on the thread its recipient lives in.
using QtRxMeteringClientSideTransport = RecipientSideT<RxMeteringClientSideRingsT<QtRxMeteringMessageRings>, QtRecipientT>;

#pragma once

#include <transport/IqClientSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtIqMessageRings.h"

// The client's IQ side on Qt, notified on the thread its recipient lives in.
using QtIqClientSideTransport = RecipientSideT<IqClientSideRingsT<QtIqMessageRings>, QtRecipientT>;

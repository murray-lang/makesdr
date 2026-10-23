#pragma once

#include <transport/MetaClientSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtMetaMessageRings.h"

// The client's meta side on Qt, notified on the thread its recipient lives in.
using QtMetaClientSideTransport = RecipientSideT<MetaClientSideRingsT<QtMetaMessageRings>, QtRecipientT>;

#pragma once

#include <transport/UpdateClientSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtUpdateMessageRings.h"

// The client's settings side on Qt, notified on the thread its recipient lives in.
using QtUpdateClientSideTransport = UpdateClientSideRingsT<QtUpdateMessageRings>;

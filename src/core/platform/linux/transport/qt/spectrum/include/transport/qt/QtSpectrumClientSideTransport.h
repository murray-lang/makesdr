#pragma once

#include <transport/SpectrumClientSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtSpectrumMessageRings.h"

// The client's IQ side on Qt, notified on the thread its recipient lives in.
using QtSpectrumClientSideTransport = RecipientSideT<SpectrumClientSideRingsT<QtSpectrumMessageRings>, QtRecipientT>;

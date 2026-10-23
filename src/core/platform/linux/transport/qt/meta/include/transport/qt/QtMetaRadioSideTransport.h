#pragma once

#include <transport/MetaRadioSideRingsT.h>

#include "QtMetaMessageRings.h"

// The radio's meta side on Qt. It only sends, so needs no wake.
using QtMetaRadioSideTransport = MetaRadioSideRingsT<QtMetaMessageRings>;

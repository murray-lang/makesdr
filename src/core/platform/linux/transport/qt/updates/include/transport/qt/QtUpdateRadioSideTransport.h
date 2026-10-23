#pragma once

#include <transport/UpdateRadioSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtUpdateMessageRings.h"

using QtUpdateRadioSideTransport =
  RecipientSideT<UpdateRadioSideRingsT<QtUpdateMessageRings>, QtRecipientT>;

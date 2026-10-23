#pragma once

#include <transport/SettingsClientSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtSettingsMessageRingsT.h"

// The client's settings side on Qt, notified on the thread its recipient lives in.
template<typename RadioSettingsT>
using QtSettingsClientSideTransportT =
  RecipientSideT<SettingsClientSideRingsT<QtSettingsMessageRingsT<RadioSettingsT>>, QtRecipientT>;

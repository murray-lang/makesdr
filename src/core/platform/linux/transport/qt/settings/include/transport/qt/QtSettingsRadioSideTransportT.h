#pragma once

#include <transport/SettingsRadioSideRingsT.h>
#include <transport/RecipientSideT.h>
#include <transport/qt/QtRecipientT.h>

#include "QtSettingsMessageRingsT.h"

// The radio's settings side on Qt, notified on the thread its recipient lives in.
template<typename RadioSettingsT>
using QtSettingsRadioSideTransportT =
  RecipientSideT<SettingsRadioSideRingsT<QtSettingsMessageRingsT<RadioSettingsT>>, QtRecipientT>;

#pragma once

#include "MutableActiveBandSettings.h"
#include <model/basic/ReceiverSettings.h>
#include "model/basic/RadioSettings.h"
// #include "../TransmitterSettings.h"
#include "model/core/MessageVisitor.h"
// #include "../../util/exampleRadioSettings.h"
#include "BandSettingsCache.h"
#include "AutoComplete.h"
// #include "base/mutable/ResolveIndirection.h"
#include "RadioCategories.h"
#include "model/core/ProtobufIo.h"

class MutableRadioSettings :
  public RadioSettings<MutableActiveBandSettings>,
  public AutoComplete
{
public:
  MutableRadioSettings(const RadioCategories& categories)
    : m_visitor(&m_rawSettings, &RadioSettings_RadioSettingsPb_msg)
    , m_categories(categories)
  {
    m_activeBandSettings.setCategories(&m_categories);
  }

  MutableRadioSettings(RadioSettings_RadioSettingsPb& raw, const RadioCategories& categories)
  : RadioSettings(raw)
  , m_categories(categories)
  , m_visitor(&m_rawSettings, &RadioSettings_RadioSettingsPb_msg)
  {
    m_activeBandSettings.setCategories(&m_categories);
  }

  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length) {
    return ProtobufIo::readProtobuf<RadioSettings_RadioSettingsPb>(
      buffer,
      msg_length,
      RadioSettings_RadioSettingsPb_fields,
      RadioSettings_RadioSettingsPb_init_zero,
      m_rawSettings
    );
  }

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingFieldPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  );

  ResultCode applyUpdate(const SettingFieldUpdate& update);
  ResultCode autoComplete(const SettingFieldPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger) override;
  ResultCode autoComplete() override;
protected:
  MessageVisitor m_visitor;
  RadioCategories m_categories;
  BandSettingsCache m_bandSettingsCache;
};

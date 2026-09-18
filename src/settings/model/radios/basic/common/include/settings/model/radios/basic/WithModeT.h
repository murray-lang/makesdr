#pragma once
#include <settings/model/message/FieldDescriptor.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/radios/Mode.h>
#include <settings/model/radios/IApplyBandDefaults.h>

template <typename protoT, int requestTag, int modeTag>
class WithModeT
{
public:
  virtual ~WithModeT() = default;

  explicit WithModeT(protoT& rawWithMode)
    : m_rawModeSettings(rawWithMode)
    , m_modeRequest(static_cast<Mode::Type>(rawWithMode.mode_or_request.mode_request))
    , m_mode(rawWithMode.mode_or_request.mode)
  {
  }

  [[nodiscard]] bool isModeRequest() const { return m_rawModeSettings.which_mode_or_request == requestTag; }
  [[nodiscard]] bool isMode() const { return m_rawModeSettings.which_mode_or_request == modeTag; }
  [[nodiscard]] Mode::Type modeRequest() const { return m_modeRequest; }
  Mode& mode() { return m_mode; }
  const Mode& mode() const { return m_mode; }
  protoT& rawMode() { return m_rawModeSettings; }

  void setModeRequest(Mode::Type modeType)
  {
    m_rawModeSettings.which_mode_or_request = requestTag;
    m_rawModeSettings.mode_or_request.mode_request = static_cast<makesdr_ModeType>(modeType);
  }

  ResultCode autoCompleteMode(const ModeList* modes)
  {
    if (m_rawModeSettings.which_mode_or_request == requestTag) {
      if (modes == nullptr) {
        return ResultCode::ERR_SETTING_AUTOCOMPLETE_NO_MODE_INFO;
      }
      const makesdr_ModePb* pMode = modes->findModeByType(m_rawModeSettings.mode_or_request.mode_request);
      if (pMode == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_MODE_NOT_FOUND;

      m_rawModeSettings.mode_or_request.mode = *pMode;
      m_rawModeSettings.which_mode_or_request = modeTag;
    }
    return ResultCode::OK;
  }

  ResultCode autoCompleteMode(const FieldDescriptor& setting, uint32_t startIndex, const ModeList* modes)
  {
    const FieldPath& path = setting.getPath();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startIndex] != requestTag) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
    }
    return autoCompleteMode(modes);
  }

protected:
  protoT& m_rawModeSettings;
  Mode::Type m_modeRequest;
  Mode m_mode;
};
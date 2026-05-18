#pragma once
#include "RadioSettings.pb.h"
#include "SettingFieldPath.h"
#include <etl/string.h>
#include <ResultCode.h>
#include "SettingFieldUpdate.h"

class SettingsManipulator
{
public:
  SettingsManipulator(void* pMessage, const pb_msgdesc_t *pDescriptor)
  : m_pMessage(pMessage)
  , m_pDescriptor(pDescriptor) {}

  ResultCode updateField(const SettingFieldUpdate &settingUpdate)
  {
    return updateField(settingUpdate.path(), settingUpdate.value());
  }

  ResultCode updateField(const SettingFieldPath &path, const SettingFieldVariant &value);

  ResultCode getField(const SettingFieldPath &path, SettingFieldVariant &value);

  ResultCode mergePresentFields(const void* pRhsMessage);

  static ResultCode resolveDottedPath(const char *dottedPath, SettingFieldPath &path);

protected:
  static ResultCode updateField(pb_field_iter_t* iter, const SettingFieldVariant& value);
  static ResultCode updateSteppable(pb_field_iter_t* iter, const SettingFieldVariant& value);
  ResultCode getField(pb_field_iter_t* iter, SettingFieldVariant& value);
  static ResultCode markFieldPresent(const pb_field_iter_t* iter);

  static ResultCode mergePresentFields(
    void* pLhsMessage,
    const void* pRhsMessage,
    const pb_msgdesc_t* pDescriptor
  );
  static bool fieldHasMergeablePresence(const pb_field_iter_t* iter);
  static bool shouldVisitField(const pb_field_iter_t* iter);
  static ResultCode copyPresentField(pb_field_iter_t* pLhsIter, const pb_field_iter_t* pRhsIter);

  static void pb_calc_steppable_float(
    float oldValue,
    float delta,
    float coarseStep,
    float fineStep,
    bool useFine,
    float* newValue
  );

  static void pb_calc_steppable_int64(
    int64_t oldValue,
    int64_t delta,
    int64_t coarseStep,
    int64_t fineStep,
    int64_t useFine,
    int64_t* newValue
  );
private:
  struct StringValue
  {
    const char* data = nullptr;
    pb_size_t size = 0;
  };

  struct StringValueVisitor
  {
    StringValue operator()(const NameString& value) const;
    StringValue operator()(const LabelString& value) const;

    template <typename T>
    StringValue operator()(const T&) const
    {
      return {};
    }
  };

  static StringValue getStringValue(const SettingFieldVariant& value);
  void* m_pMessage;
  const pb_msgdesc_t *m_pDescriptor;
};

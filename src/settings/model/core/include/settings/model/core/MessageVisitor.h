#pragma once
#include "settings/model/proto/RadioSettings.pb.h"
#include "SettingPath.h"
#include <etl/string.h>
#include <ResultCode.h>
#include "SettingUpdate.h"
#include "SettingUpdateVariant.h"

class MessageVisitor
{
public:
  MessageVisitor(void* pMessage, const pb_msgdesc_t *pDescriptor)
  : m_pMessage(pMessage)
  , m_pDescriptor(pDescriptor) {}

  static ResultCode updateField(
    void* pMessage,
    const pb_msgdesc_t *pDescriptor,
    const SettingUpdate &settingUpdate,
    uint32_t startingAtIndex = 0
    )
  {
    return updateField(pMessage, pDescriptor, settingUpdate.path(), settingUpdate.value(), startingAtIndex);
  }

  static ResultCode updateField(
    void* pMessage,
    const pb_msgdesc_t *pDescriptor,
    const SettingPath &path,
    const SettingUpdateVariant &value,
    uint32_t startingAtIndex = 0
    );

  static ResultCode getField(
    void* pMessage,
    const pb_msgdesc_t *pDescriptor,
    const SettingPath &path,
    SettingUpdateVariant &value
  );

  static ResultCode getField(
    void* pMessage,
    const pb_msgdesc_t *pDescriptor,
    const SettingPath &path,
    SettingUpdateVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  );

  static ResultCode setFieldPresence(
    void* pMessage,
    const pb_msgdesc_t *pDescriptor,
    const SettingPath &path, bool present
  );

  static ResultCode setAllFieldsPresence(
    void* pMessage,
    const pb_msgdesc_t *pDescriptor,
    bool present
  );

  ResultCode updateField(const SettingUpdate &settingUpdate)
  {
    return updateField(m_pMessage, m_pDescriptor, settingUpdate.path(), settingUpdate.value());
  }

  ResultCode updateField(const SettingPath &path, const SettingUpdateVariant &value)
  {
    return updateField(m_pMessage, m_pDescriptor, path, value);
  }

  ResultCode getField(const SettingPath &path, SettingUpdateVariant &value) const
  {
    return getField(m_pMessage, m_pDescriptor, path, value);
  }

  ResultCode getField(
    const SettingPath &path,
    SettingUpdateVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  ) const
  {
    return getField(m_pMessage, m_pDescriptor, path, value, mustHave, parentsMustHave, retrieved);
  }

  ResultCode setFieldPresence(const SettingPath &path, bool present)
  {
    return setFieldPresence(m_pMessage, m_pDescriptor, path, present);
  }

  ResultCode setAllFieldsPresence(bool present)
  {
    return setAllFieldsPresence(m_pMessage, m_pDescriptor, present);
  }

  ResultCode mergePresentFields(const void* pRhsMessage);

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  );

protected:
  static ResultCode updateField(pb_field_iter_t* iter, const SettingUpdateVariant& value);
  static ResultCode updateSteppable(pb_field_iter_t* iter, const SettingUpdateVariant& value);
  static ResultCode getField(pb_field_iter_t* iter, SettingUpdateVariant& value) ;
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
#ifdef USE_ETL
    StringValue operator()(const NameString& value) const;
    StringValue operator()(const LabelString& value) const;
#else
    StringValue operator()(const std::string& value) const;
#endif
    template <typename T>
    StringValue operator()(const T&) const
    {
      return {};
    }
  };

  template<typename TargetType, ResultCode ErrorCode>
  struct PromotingIntUpdateVisitor
  {
    void* target_ptr;

    template<typename T>
    ResultCode operator()(T value) const {
      if constexpr (std::is_integral_v<T>) {
        if constexpr (std::is_same_v<std::common_type_t<T, TargetType>, TargetType>) {
          *static_cast<TargetType*>(target_ptr) = static_cast<TargetType>(value);
          return ResultCode::OK;
        }
      }
      return ErrorCode;
    }
  };

  static StringValue getStringValue(const SettingUpdateVariant& value);
  void* m_pMessage;
  const pb_msgdesc_t *m_pDescriptor;
};

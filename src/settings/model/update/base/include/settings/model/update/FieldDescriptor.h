#pragma once

#include <ResultCode.h>
#include "FieldPath.h"
#include <config/struct/FieldDescriptorConfig.h>

class FieldDescriptor
{
public:
  FieldDescriptor();
  FieldDescriptor(FieldPath path);
  FieldDescriptor(FieldPath path, bool needsAutoComplete);
  FieldDescriptor(FieldPath path, bool needsAutoComplete, bool isIndirect);
  FieldDescriptor(const uint32_t* path, uint16_t pathCount, bool needsAutoComplete, bool isIndirect);

  ResultCode configure(const Config::FieldDescriptor::Fields& fields);

  [[nodiscard]] FieldPath& getPath() { return m_path; }
  [[nodiscard]] const FieldPath& getPath() const { return m_path; }

  [[nodiscard]] bool needsAutoComplete() const { return m_needsAutoComplete; }
  void setNeedsAutoComplete(bool needsAutoComplete)
  {
    m_needsAutoComplete = needsAutoComplete;
  }

  [[nodiscard]] bool isIndirect() const { return m_isIndirect; }
  void setIsIndirect(bool isIndirect) { m_isIndirect = isIndirect; }

  [[nodiscard]] bool isRequest() const
  {
    return !m_path.empty() && m_path.at(0) == 0;
  }

private:
  FieldPath m_path;
  bool m_needsAutoComplete;
  bool m_isIndirect;
};

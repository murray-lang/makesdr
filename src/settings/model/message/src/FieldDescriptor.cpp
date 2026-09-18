#include <CrossPlatformTypes.h>
#include "settings/model/message/FieldDescriptor.h"

FieldDescriptor::FieldDescriptor()
  : m_needsAutoComplete(false)
  , m_isIndirect(false)
{
}

FieldDescriptor::FieldDescriptor(FieldPath  path)
  : m_path(::move(path))
  , m_needsAutoComplete(false)
  , m_isIndirect(false)
{
}

FieldDescriptor::FieldDescriptor(FieldPath  path, bool needsAutoComplete)
  : m_path(::move(path))
  , m_needsAutoComplete(needsAutoComplete)
  , m_isIndirect(false)
{
}

FieldDescriptor::FieldDescriptor(
  FieldPath  path,
  bool needsAutoComplete,
  bool isIndirect
  )
  : m_path(::move(path))
  , m_needsAutoComplete(needsAutoComplete)
  , m_isIndirect(isIndirect)
{
}

FieldDescriptor::FieldDescriptor(
  const uint32_t* path,
  uint16_t pathCount,
  bool needsAutoComplete,
  bool isIndirect
  )
  : m_path(path, path + pathCount)
  , m_needsAutoComplete(needsAutoComplete)
  , m_isIndirect(isIndirect)
{
}


ResultCode
FieldDescriptor::configure(const Config::FieldDescriptor::Fields& fields)
{
  m_path = fields.tags;
  m_isIndirect = fields.isIndirect;
  m_needsAutoComplete = fields.needsAutoComplete;
  return ResultCode::OK;
}

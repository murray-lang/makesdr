#include "settings/model/extra/RadioSettingsEx.h"
// #include "../../util/ResolvedFieldPaths.h"


ResultCode
RadioSettingsEx::resolveDottedPath(
  const char *dottedPath,
  SettingFieldPath &path,
  bool* isIndirectOut,
  AutoCompleteTrigger* triggerOut
)
{
  return MessageVisitor::resolveDottedPath(dottedPath, path, isIndirectOut, triggerOut);
}

void printPath(const SettingFieldPath& v)
{
  for (uint32_t i : v) {
    printf("%d ", i);
  }
  printf("\n");
}

ResultCode
RadioSettingsEx::autoComplete(
  const SettingFieldPath& path,
  uint32_t startingAtIndex,
  AutoCompleteTrigger trigger
)
{
  if (startingAtIndex >= path.size()) {
    return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
  }
  switch (path[startingAtIndex]) {
  case RadioSettings_RadioSettingsPb_active_bands_tag:
    return m_activeBandSettings.autoComplete(path, startingAtIndex + 1, trigger);
  }
  return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
}

ResultCode
RadioSettingsEx::autoComplete()
{
  return m_activeBandSettings.autoComplete();
}








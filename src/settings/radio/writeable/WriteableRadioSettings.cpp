#include "WriteableRadioSettings.h"
#include "../../util/ResolvedFieldPaths.h"
#include "../../mode/ModeList.h"
#include "../../band/writeable/AvailableBands.h"
#include "../../band/BandCategoryList.h"


ResultCode WriteableRadioSettings::resolveDottedPath(
  const char *dottedPath,
  SettingFieldPath &path,
  bool* isIndirectOut,
  AutoCompleteTrigger* triggerOut
)
{
  return SettingsManipulator::resolveDottedPath(dottedPath, path, isIndirectOut, triggerOut);
}

void printPath(const SettingFieldPath& v)
{
  for (uint32_t i : v) {
    printf("%d ", i);
  }
  printf("\n");
}

ResultCode
WriteableRadioSettings::applyUpdate(const SettingFieldUpdate& update)
{
  const SettingFieldPath& path = update.path();
  ResultCode rc = ResultCode::OK;
  if (update.isIndirect()) {
    SettingFieldPath resolvedPath;
    rc = resolveIndirection(path, 0, resolvedPath);
    if (rc != ResultCode::OK) return rc;
    rc = m_manipulator.updateField(resolvedPath, update.value());
    if (rc == ResultCode::OK && update.trigger() != AutoCompleteTrigger::NONE) {
      return autoComplete(resolvedPath, 0, update.trigger());
    }
  } else {
    rc = m_manipulator.updateField(update);
    if (rc == ResultCode::OK && update.trigger() != AutoCompleteTrigger::NONE) {
      return autoComplete(update.path(), 0, update.trigger());
    }
  }

  return rc;
}

ResultCode
WriteableRadioSettings::resolveIndirection(
  const SettingFieldPath& indirectPath,
  uint32_t startingAtIndex,
  SettingFieldPath& resolvedPath
  )
{
  if (startingAtIndex >= indirectPath.size()) {
    return ResultCode::ERR_SETTING_RESOLVE_INDIRECTION_PATH_INVALID;
  }
  switch (indirectPath[startingAtIndex]) {
  case RadioSettings_RadioSettingsPb_active_bands_tag:
    resolvedPath.emplace_back(RadioSettings_RadioSettingsPb_active_bands_tag);
    return m_activeBandSettings.resolveIndirection(indirectPath, startingAtIndex + 1, resolvedPath);
  }
  for (uint32_t i = startingAtIndex; i < indirectPath.size(); i++) {
    resolvedPath.emplace_back(indirectPath[i]);
  }
  return ResultCode::OK;

}

ResultCode
WriteableRadioSettings::autoComplete(
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








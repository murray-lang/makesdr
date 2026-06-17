#include "settings/model/core/BandSettingsCache.h"
#include <cstring>

BandSettingsCache::Entry::Entry(makesdr_BandSettingsCachePb_BandSettingsEntry& raw)
      : m_rawSettings(raw)
      , m_bandName(raw.key, raw.key, sizeof(raw.key))
{
  if (raw.has_value) {
    m_bandSettings.emplace(raw.value);
  }
}

BandSettingsCache::Entry::Entry(Entry&& rhs) noexcept
  : m_rawSettings(rhs.m_rawSettings)
  , m_bandName(rhs.raw().key, rhs.raw().key, sizeof(rhs.raw().key))
{
  m_bandSettings.emplace(etl::move(rhs.m_bandSettings));
}

BandSettingsCache::Entry&
BandSettingsCache::Entry:: operator=(Entry&& rhs) noexcept
{
  m_rawSettings = rhs.m_rawSettings;
  m_bandName = rhs.m_bandName;
  m_bandSettings = rhs.m_bandSettings;
  return *this;
}

makesdr_BandSettingsPb*
BandSettingsCache::Entry::bandSettings()
{
  if (m_bandSettings) return &m_bandSettings.value();
  return nullptr;
}

BandSettingsCache::BandSettingsCache()
  : m_rawSettings(makesdr_BandSettingsCachePb_init_zero)
{
  for (pb_size_t i = 0; i < m_rawSettings.band_settings_count; i++) {
    m_entries.emplace_back(m_rawSettings.band_settings[i]);
  }
}

ResultCode
BandSettingsCache::get(makesdr_BandSettingsPb* bandSettings)
{
  if (bandSettings->band_or_request.band.name[0] == '\0') {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND_NAME;
  }

  int32_t index = findEntryIndex(bandSettings->band_or_request.band.name);
  if (index == -1) {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_NOT_AVAILABLE;
  }
  *bandSettings = m_rawSettings.band_settings[index].value;
  return ResultCode::OK;
 }

ResultCode
BandSettingsCache::set(const makesdr_BandSettingsPb* bandSettings)
{
  if (bandSettings->which_band_or_request != makesdr_BandSettingsPb_band_tag) {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND;
  }
  if (bandSettings->band_or_request.band.name[0] == '\0') {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND_NAME;
  }

  int32_t index = findEntryIndex(bandSettings->band_or_request.band.name);
  if (index != -1) {
    updateRawEntry(index, *bandSettings);
  } else {
    pb_size_t count = rawCount();
    if (count >= MAX_CACHE_ENTRIES) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_CACHE_FULL;
    }
    pb_size_t newIndex = incrementRawCount() - 1;
    setRawEntry(newIndex, bandSettings->band_or_request.band.name, *bandSettings);

    // Now point a new wrapper entry to it
    m_entries.emplace_back(m_rawSettings.band_settings[newIndex]);
  }

  return ResultCode::OK;
}

void
BandSettingsCache::updateRawEntry(pb_size_t index, const makesdr_BandSettingsPb& rawBandSettings)
{
  m_rawSettings.band_settings[index].value = rawBandSettings;
}

void
BandSettingsCache::setRawEntry(
  pb_size_t index,
  const char* bandName,
  const makesdr_BandSettingsPb& rawBandSettings
  )
{
  std::memcpy(m_rawSettings.band_settings[index].key, bandName, MAX_NAME_LENGTH+1);
  m_rawSettings.band_settings[index].value = rawBandSettings;

}

int32_t
BandSettingsCache::findEntryIndex(const char * bandName) const
{
  int numEntries = static_cast<int>(m_entries.size());
  for (int i = 0; i < numEntries; i++) {
    if (m_entries[i].bandName() == bandName) {
      return i;
    }
  }
  return -1;
}

BandSettingsCache::Entry*
BandSettingsCache::getEntry(int32_t index)
{
  int numEntries = static_cast<int>(m_entries.size());
  return index >= 0 && index < numEntries ? &m_entries[index] : nullptr;
}

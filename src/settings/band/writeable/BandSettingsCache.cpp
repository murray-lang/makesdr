#include "BandSettingsCache.h"

BandSettingsCache::Entry::Entry(RadioSettings_BandSettingsCachePb_BandSettingsEntry& raw)
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

BandSettings*
BandSettingsCache::Entry::bandSettings()
{
  if (m_bandSettings) return &m_bandSettings.value();
  return nullptr;
}

BandSettingsCache::BandSettingsCache(
  const BandCategoryList& bandInfo,
  const ModeList& modeInfo
)
  : m_rawSettings(RadioSettings_BandSettingsCachePb_init_zero)
  , m_bandInfo(bandInfo)
  , m_modeInfo(modeInfo)
{
  for (pb_size_t i = 0; i < m_rawSettings.band_settings_count; i++) {
    m_entries.emplace_back(m_rawSettings.band_settings[i]);
  }
}

ResultCode
BandSettingsCache::get(BandSettings* settings)
{
  const StringRef* bandName = settings->bandName();
  if (bandName == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND_NAME;

  int32_t index = findEntryIndex(bandName->c_str());
  if (index != -1) {
    *settings = m_rawSettings.band_settings[index].value;
    return ResultCode::OK;
  }
  // Not found in the cache. See if we have room for a new one
  pb_size_t count = rawCount();
  if (count >= MAX_CACHE_ENTRIES) {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_NOT_AVAILABLE;
  }
  // We have room for another, but do we know about this band?
  const Band* bandInfo = m_bandInfo.findBand(bandName->c_str());
  if (bandInfo == nullptr) {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_NOT_AVAILABLE;
  }
  //Add the entry, set the defaults for the given band, then point the output to the new entry
  //Create a wrapper for the new entry to give us initialisation tools.
  pb_size_t newIndex = incrementRawCount() - 1;
  initRawEntry(newIndex, *bandName, *bandInfo);
  // BandSettings newSettingWrapper(m_rawSettings.band_settings[newIndex].value);
  m_entries.emplace_back(m_rawSettings.band_settings[newIndex]);
  *settings = m_rawSettings.band_settings[newIndex].value;

  return ResultCode::OK;
}

ResultCode
BandSettingsCache::set(const BandSettings* bandSettings)
{
  if (!bandSettings->hasBand()) {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND;
  }
  const StringRef* bandName = bandSettings->bandName();
  if (bandName == nullptr) return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND_NAME;

  int32_t index = findEntryIndex(bandName->c_str());
  if (index != -1) {
    updateRawEntry(index, bandSettings->raw());
  } else {
    pb_size_t count = rawCount();
    if (count >= MAX_CACHE_ENTRIES) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_CACHE_FULL;
    }
    pb_size_t newIndex = incrementRawCount() - 1;
    setRawEntry(newIndex, *bandName, bandSettings->raw());

    // Now point a new wrapper entry to it
    m_entries.emplace_back(m_rawSettings.band_settings[newIndex]);
  }

  return ResultCode::OK;
}

void
BandSettingsCache::initRawEntry(pb_size_t index, const StringRef& bandName, const Band& band)
{
  std::copy(bandName.c_str(), bandName.c_str() + bandName.length() + 1, m_rawSettings.band_settings[index].key);
  m_rawSettings.band_settings[index].value.which_band_or_request = RadioSettings_BandSettingsPb_band_tag;
  m_rawSettings.band_settings[index].value.band_or_request.band = band.raw();
  m_rawSettings.band_settings[index].has_value = true;

}

void
BandSettingsCache::updateRawEntry(pb_size_t index, const RadioSettings_BandSettingsPb& rawBandSettings)
{
  m_rawSettings.band_settings[index].value = rawBandSettings;
}

void
BandSettingsCache::setRawEntry(
  pb_size_t index,
  const StringRef& bandName,
  const RadioSettings_BandSettingsPb& rawBandSettings
  )
{
  std::copy(bandName.c_str(), bandName.c_str() + bandName.length() + 1, m_rawSettings.band_settings[index].key);
  m_rawSettings.band_settings[index].value = rawBandSettings;

}

int32_t
BandSettingsCache::findEntryIndex(const char * bandName) const
{
  for (int i = 0; i < m_entries.size(); i++) {
    if (m_entries[i].bandName() == bandName) {
      return i;
    }
  }
  return -1;
}

BandSettingsCache::Entry*
BandSettingsCache::getEntry(int32_t index)
{
  return index >= 0 && index < m_entries.size() ? &m_entries[index] : nullptr;
}

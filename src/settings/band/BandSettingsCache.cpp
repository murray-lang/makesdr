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

BandSettingsCache::BandSettingsCache(RadioSettings_BandSettingsCachePb& raw)
    : m_rawSettings(raw)
{
  for (pb_size_t i = 0; i < m_rawSettings.band_settings_count; i++) {
    m_entries.emplace_back(m_rawSettings.band_settings[i]);
  }
}

ResultCode
BandSettingsCache::set(const char * bandName, const BandSettings& settings)
{
  if (!settings.hasBand()) {
    return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND;
  }
  pb_size_t index = findEntryIndex(bandName);
  if (index == -1) {
    if (count() >= MAX_CACHE_ENTRIES) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_CACHE_FULL;
    }

    pb_size_t currCount = m_rawSettings.band_settings_count;
    const Band* pUpdatedBand = settings.band();
    if (pUpdatedBand == nullptr) {
      return ResultCode::ERR_SETTING_BAND_SETTINGS_HAS_NO_BAND;
    }
    RadioSettings_BandSettingsCachePb_BandSettingsEntry& rawEntry =
      m_rawSettings.band_settings[currCount];
    rawEntry.has_value = true;
    rawEntry.value.which_band_or_request = RadioSettings_BandSettingsPb_band_tag;
    rawEntry.value.band_or_request.band = pUpdatedBand->raw();

    Entry newEntry(rawEntry);
    newEntry.bandName() = pUpdatedBand->name();
    m_entries.emplace_back(newEntry);

    m_rawSettings.band_settings[currCount].key = pUpdatedBand->name();
    m_rawSettings.band_settings_count++;

  } else {

  }
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

#pragma once
#include "BandSettings.h"
#include "base/SettingsBase.h"
#include "etl/vector.h"

#define MAX_CACHE_ENTRIES 10

class BandSettingsCache : SettingsBase
{
public:
  //BandSettingsCache::Entry -------------------------------------------------------------------------
  class Entry : SettingsBase
  {
  public:
    Entry(RadioSettings_BandSettingsCachePb_BandSettingsEntry& raw);
    Entry(Entry&& rhs) noexcept;
    Entry& operator=(Entry&& rhs) noexcept;

    [[nodiscard]] const StringRef& bandName() const { return m_bandName; }
    StringRef& bandName() { return m_bandName; }

    [[nodiscard]] BandSettings* bandSettings();
    RadioSettings_BandSettingsCachePb_BandSettingsEntry& raw() { return m_rawSettings; }
  protected:
    RadioSettings_BandSettingsCachePb_BandSettingsEntry& m_rawSettings;
    StringRef m_bandName;
    etl::optional<BandSettings> m_bandSettings;
  };
  //BandSettingsCache::Entry ---------------------------------------------------------------------------

  using EntryList = etl::vector<Entry, MAX_CACHE_ENTRIES>;

  BandSettingsCache(RadioSettings_BandSettingsCachePb& raw);

  uint32_t count() const { return m_entries.size(); }

  BandSettings* getBandSettings(const char * bandName);

  ResultCode set(const char * bandName, const BandSettings& settings);

protected:
  int32_t findEntryIndex(const char * bandName) const;
  Entry* getEntry(int32_t index);

  RadioSettings_BandSettingsCachePb& m_rawSettings;
  EntryList m_entries;
};

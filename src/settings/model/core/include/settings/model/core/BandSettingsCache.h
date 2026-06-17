#pragma once
#include <ResultCode.h>
#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/core/StringRef.h"
#include <etl/vector.h>
#include "pb.h"


#define MAX_CACHE_ENTRIES 10

class BandSettingsCache
{
public:
  //BandSettingsCache::Entry -------------------------------------------------------------------------
  class Entry
  {
  public:
    Entry(makesdr_BandSettingsCachePb_BandSettingsEntry& raw);
    Entry(Entry&& rhs) noexcept;
    Entry& operator=(Entry&& rhs) noexcept;

    [[nodiscard]] const StringRef& bandName() const { return m_bandName; }
    StringRef& bandName() { return m_bandName; }

    [[nodiscard]] makesdr_BandSettingsPb* bandSettings();
    makesdr_BandSettingsCachePb_BandSettingsEntry& raw() { return m_rawSettings; }
  protected:
    makesdr_BandSettingsCachePb_BandSettingsEntry& m_rawSettings;
    StringRef m_bandName;
    etl::optional<makesdr_BandSettingsPb> m_bandSettings;
  };
  //BandSettingsCache::Entry ---------------------------------------------------------------------------

  using EntryList = etl::vector<Entry, MAX_CACHE_ENTRIES>;

  BandSettingsCache();

  [[nodiscard]] uint32_t count() const { return m_entries.size(); }

  bool hasBand(const char * bandName) const  { return findEntryIndex(bandName) != -1; }

  ResultCode get(makesdr_BandSettingsPb* settings);
  ResultCode set(const makesdr_BandSettingsPb* settings);

  makesdr_BandSettingsCachePb& raw() { return m_rawSettings; }
  [[nodiscard]] const makesdr_BandSettingsCachePb& raw() const { return m_rawSettings; }

protected:
  [[nodiscard]] pb_size_t rawCount() const { return m_rawSettings.band_settings_count; }
  pb_size_t incrementRawCount() { return ++m_rawSettings.band_settings_count; }
  void updateRawEntry(pb_size_t index, const makesdr_BandSettingsPb& rawBandSettings);
  void setRawEntry(pb_size_t index, const char* bandName, const makesdr_BandSettingsPb& rawBandSettings);

  int32_t findEntryIndex(const char * bandName) const;
  Entry* getEntry(int32_t index);

  makesdr_BandSettingsCachePb m_rawSettings;
  EntryList m_entries;
};

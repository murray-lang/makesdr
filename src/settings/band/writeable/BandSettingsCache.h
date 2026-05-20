#pragma once
#include "../BandCategoryList.h"
#include "../BandSettings.h"
#include "base/SettingsBase.h"
#include "etl/vector.h"
#include "mode/ModeList.h"

#define MAX_CACHE_ENTRIES 10

class BandSettingsCache : public SettingsBase
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

  BandSettingsCache(const BandCategoryList& bandInfo,  const ModeList& modeInfo);

  [[nodiscard]] uint32_t count() const { return m_entries.size(); }

  ResultCode get(BandSettings* settings);
  ResultCode set(const BandSettings* settings);

  [[nodiscard]] const ModeList& modeInfo() const { return m_modeInfo; }
  [[nodiscard]] const BandCategoryList& bandInfo() const { return m_bandInfo; }

  RadioSettings_BandSettingsCachePb& raw() { return m_rawSettings; }
  [[nodiscard]] const RadioSettings_BandSettingsCachePb& raw() const { return m_rawSettings; }

protected:
  [[nodiscard]] pb_size_t rawCount() const { return m_rawSettings.band_settings_count; }
  pb_size_t incrementRawCount() { return ++m_rawSettings.band_settings_count; }
  void updateRawEntry(pb_size_t index, const RadioSettings_BandSettingsPb& rawBandSettings);
  void setRawEntry(pb_size_t index, const StringRef& bandName, const RadioSettings_BandSettingsPb& rawBandSettings);
  void initRawEntry(pb_size_t index, const StringRef& bandName, const Band& band);

  int32_t findEntryIndex(const char * bandName) const;
  Entry* getEntry(int32_t index);

  RadioSettings_BandSettingsCachePb m_rawSettings;
  const BandCategoryList& m_bandInfo;
  const ModeList& m_modeInfo;
  EntryList m_entries;
};

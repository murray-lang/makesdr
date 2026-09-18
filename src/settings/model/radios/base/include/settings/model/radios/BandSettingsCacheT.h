#pragma once
#include <CrossPlatformTypes.h>
#include <ResultCode.h>
#include <settings/model/message/MessageT.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/message/StringRef.h>
#include <etl/vector.h>
#include <pb.h>


#define MAX_CACHE_ENTRIES 10

//template <typename BandSettingsType, typename BandSettingsCacheType, typename BandSettingsEntryType>
template<
  typename CacheType,
  const pb_msgdesc_t* descriptor,
  typename PayloadPbType,
  makesdr_RadioPayloadType payloadTypeEnum,
  int payloadSize,
  typename BandSettingsType,
  typename BandSettingsEntryType
>
class BandSettingsCacheT : public MessageT<CacheType, descriptor, PayloadPbType, payloadTypeEnum, payloadSize>
{
public:
  //BandSettingsCache::Entry -------------------------------------------------------------------------
  class Entry
  {
  public:
    Entry(BandSettingsEntryType& raw)
      : m_rawSettings(raw)
      , m_bandName(raw.key, raw.key, sizeof(raw.key))
    {
      if (raw.has_value) {
        m_bandSettings.emplace(raw.value);
      }
    }
    Entry(Entry&& rhs) noexcept
      : m_rawSettings(rhs.m_rawSettings)
      , m_bandName(rhs.raw().key, rhs.raw().key, sizeof(rhs.raw().key))
    {
      m_bandSettings.emplace(etl::move(rhs.m_bandSettings));
    }
    Entry& operator=(Entry&& rhs) noexcept
    {
      m_rawSettings = rhs.m_rawSettings;
      m_bandName = rhs.m_bandName;
      m_bandSettings = rhs.m_bandSettings;
      return *this;
    }

    [[nodiscard]] const StringRef& bandName() const { return m_bandName; }
    StringRef& bandName() { return m_bandName; }

    [[nodiscard]] BandSettingsType* bandSettings()
    {
      if (m_bandSettings) return &m_bandSettings.value();
      return nullptr;
    }
    BandSettingsEntryType& raw() { return m_rawSettings; }
  protected:
    BandSettingsEntryType& m_rawSettings;
    StringRef m_bandName;
    etl::optional<BandSettingsType> m_bandSettings;
  };
  //BandSettingsCache::Entry ---------------------------------------------------------------------------

  using EntryList = etl::vector<Entry, MAX_CACHE_ENTRIES>;

  BandSettingsCacheT()
    : m_rawSettings{}
  {
    for (pb_size_t i = 0; i < m_rawSettings.band_settings_count; i++) {
      m_entries.emplace_back(m_rawSettings.band_settings[i]);
    }
  }

  BandSettingsCacheT(BandSettingsCacheT&& other) noexcept
    : m_rawSettings(::move(other.m_rawSettings))
    , m_entries(::move(other.m_entries))
  {
  }

  [[nodiscard]] uint32_t count() const { return m_entries.size(); }

  bool hasBand(const char * bandName) const  { return findEntryIndex(bandName) != -1; }

  ResultCode get(BandSettingsType* bandSettings)
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
  ResultCode set(const BandSettingsType* bandSettings)
  {
    if (bandSettings->which_band_or_request != makesdr_BasicBandSettingsPb_band_tag) {
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

  CacheType& raw() { return m_rawSettings; }
  [[nodiscard]] const CacheType& raw() const { return m_rawSettings; }

protected:
  [[nodiscard]] pb_size_t rawCount() const { return m_rawSettings.band_settings_count; }
  pb_size_t incrementRawCount() { return ++m_rawSettings.band_settings_count; }
  void updateRawEntry(pb_size_t index, const BandSettingsType& rawBandSettings)
  {
    m_rawSettings.band_settings[index].value = rawBandSettings;
  }
  void setRawEntry(pb_size_t index, const char* bandName, const BandSettingsType& rawBandSettings)
  {
    memcpy(m_rawSettings.band_settings[index].key, bandName, MAX_NAME_LENGTH+1);
    m_rawSettings.band_settings[index].value = rawBandSettings;

  }

  int32_t findEntryIndex(const char * bandName) const
  {
    int numEntries = static_cast<int>(m_entries.size());
    for (int i = 0; i < numEntries; i++) {
      if (m_entries[i].bandName() == bandName) {
        return i;
      }
    }
    return -1;
  }

  Entry* getEntry(int32_t index)
  {
    int numEntries = static_cast<int>(m_entries.size());
    return index >= 0 && index < numEntries ? &m_entries[index] : nullptr;
  }

  CacheType m_rawSettings;
  EntryList m_entries;
};

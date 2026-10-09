#pragma once
#include <settings/model/meta/band/BandCategoryList.h>
#include <settings/model/radio/Band.h>
#include <settings/model/update/FieldUpdate.h>
#include <settings/model/radio/IApplyBandDefaults.h>
#include <settings/model/update/MessageTraverser.h>


template <
  typename protoT,
  const pb_msgdesc_t* descriptor,
  int requestTag, int bandTag,
  typename CacheClass
>
class WithBandT
{
public:
  explicit WithBandT(protoT& rawWithBand)
    : m_rawBandSettings(rawWithBand)
    , m_bandRequest(
      rawWithBand.band_or_request.band_request,
      rawWithBand.band_or_request.band_request,
      sizeof(rawWithBand.band_or_request.band_request))
    , m_band(rawWithBand.band_or_request.band)
  {
    // setBandOrRequestVariant(rawWithBand);
  }

  [[nodiscard]] bool isBandRequest() const { return m_rawBandSettings.which_band_or_request == requestTag; }
  [[nodiscard]] bool isBand() const { return m_rawBandSettings.which_band_or_request == bandTag; }
  [[nodiscard]] const StringRef& bandRequest() const { return m_bandRequest; }
  Band& band() { return m_band; }
  protoT& rawBand() { return m_rawBandSettings; }

  ResultCode autoCompleteBand(
    const NameString* bandName,
    const BandCategoryList* bands,
    const ModeList* modes,
    CacheClass* cache,
    IApplyBandDefaults* parent
    )
  {
    if (bandName == nullptr && m_rawBandSettings.which_band_or_request != requestTag) {
      return ResultCode::OK; // No external request and no request placed within the settings
    }
    // If the band is set then save it before updating.
    if (m_rawBandSettings.which_band_or_request == bandTag) {
      save(m_rawBandSettings.band_or_request.band.name, m_rawBandSettings, cache);
    }
    const char* newBandName = bandName != nullptr ? bandName->c_str() : m_rawBandSettings.band_or_request.band_request;
    if (newBandName != nullptr) {
      if (bands == nullptr) {
        return ResultCode::ERR_SETTING_AUTOCOMPLETE_NO_BAND_INFO;
      }

      // Try the cache first for the requested band.
      if (cache != nullptr) {
        ResultCode rc = cache->get(newBandName, &m_rawBandSettings);
        if (rc == ResultCode::OK) {
          m_rawBandSettings.which_band_or_request = bandTag;
          MessageTraverser::setAllFieldsPresence(&m_rawBandSettings, descriptor, true);
          return rc;
        }
      }
      // Not found in the cache. Get band info.
      const makesdr_BandPb* pBand = bands->findBand(newBandName);
      if (pBand == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_BAND_NOT_FOUND;

      m_rawBandSettings.band_or_request.band = *pBand;
      m_rawBandSettings.which_band_or_request = bandTag;

      if (parent != nullptr) {
        Band band(*pBand);
        parent->applyBandDefaults(band, bands, modes);
      }
      // save(newBandName, m_rawBandSettings, cache);
    }
    return ResultCode::OK;
  }

  ResultCode autoCompleteBand(
    const FieldUpdate& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    CacheClass* cache,
    IApplyBandDefaults* parent)
  {
    const FieldPath& path = setting.path();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startIndex] != requestTag) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
    }
    const NameString* bandName = get_if<NameString>(&setting.value());
    if (bandName == nullptr) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_BAND_NOT_PROVIDED;
    }
    return autoCompleteBand(bandName, bands, modes, cache, parent);
  }

  ResultCode save(const char* bandName, protoT& proto, CacheClass* cache)
  {
    if (cache != nullptr) {
      // int saveWhich = proto.which_band_or_request;
      // proto.which_band_or_request = makesdr_BasicBandSettingsPb_band_tag;
      ResultCode rc = cache->set(bandName, &proto);
      // proto.which_band_or_request = saveWhich;
      return rc;
    }
    return ResultCode::ERR_SETTING_NO_CACHE;
  }

protected:
  protoT& m_rawBandSettings;
  StringRef m_bandRequest;
  Band m_band;
  // BandOrRequestVariant m_bandOrRequest;
};

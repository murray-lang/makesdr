#pragma once
#include <settings/model/data/band/BandCategoryList.h>
#include <settings/model/radios/Band.h>
#include <settings/model/message/FieldDescriptor.h>
#include <settings/model/radios/IApplyBandDefaults.h>


template <typename protoT, int requestTag, int bandTag, typename CacheClass>
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

  ResultCode autoCompleteBand(const BandCategoryList* bands, const ModeList* modes, CacheClass* cache, IApplyBandDefaults* parent)
  {
    if (m_rawBandSettings.which_band_or_request == requestTag) {
      if (bands == nullptr) {
        return ResultCode::ERR_SETTING_AUTOCOMPLETE_NO_BAND_INFO;
      }
      // Try the cache first
      if (cache != nullptr) {
        ResultCode rc = cache->get(&m_rawBandSettings);
        if (rc == ResultCode::OK) {
          // TODO: Set has_* = true on the band's fields
          m_rawBandSettings.which_band_or_request = bandTag;
          return rc;
        }
      }
      // Not found in the cache. Get band info.
      const makesdr_BandPb* pBand = bands->findBand(m_rawBandSettings.band_or_request.band_request);
      if (pBand == nullptr) return ResultCode::ERR_SETTING_AUTOCOMPLETE_BAND_NOT_FOUND;

      m_rawBandSettings.band_or_request.band = *pBand;
      m_rawBandSettings.which_band_or_request = bandTag;

      if (parent != nullptr) {
        Band band(*pBand);
        parent->applyBandDefaults(band, bands, modes);
      }
      if (cache != nullptr) {
        cache->set(&m_rawBandSettings); // This might fail due to being full, but ignore that.
      }
    }
    return ResultCode::OK;
  }

  ResultCode autoCompleteBand(
    const FieldDescriptor& setting,
    uint32_t startIndex,
    const BandCategoryList* bands,
    const ModeList* modes,
    CacheClass* cache,
    IApplyBandDefaults* parent)
  {
    const FieldPath& path = setting.getPath();
    if (startIndex >= path.size()) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_PATH_INVALID;
    }
    if (path[startIndex] != requestTag) {
      return ResultCode::ERR_SETTING_AUTOCOMPLETE_NOT_IMPLEMENTED;
    }
    return autoCompleteBand(bands, modes, cache, parent);
  }

protected:
  protoT& m_rawBandSettings;
  StringRef m_bandRequest;
  Band m_band;
  // BandOrRequestVariant m_bandOrRequest;
};

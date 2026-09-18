#pragma once

#include <ResultCode.h>
#include <settings/model/proto/RadioSettings.pb.h>
#include <settings/model/message/MessageT.h>

#include "BandList.h"
#include "BandTypes.h"


#ifdef USE_ETL
#include <etl/span.h>

using BandCategorySpan = etl::span<const makesdr_BandCategoryPb, MAX_BAND_CATEGORIES>;
#else
#include <span>
using BandCategorySpan = std::span<const makesdr_BandCategoryPb>;
#endif

using BandCategoryListMessage = MessageT<
  makesdr_BandCategoryListPb,
  &makesdr_BandCategoryListPb_msg,
  makesdr_BandsPayloadPb,
  makesdr_RadioPayloadType_PAYLOAD_BANDS,
  makesdr_BandsPayloadPb_size
>;

class BandCategoryList : public BandCategoryListMessage
{
public:
  BandCategoryList()
    : BandCategoryListMessage()
    , m_categories(m_payload.body.categories, m_payload.body.categories_count)
  {
  }
  BandCategoryList(const PayloadProto& payload)
    : BandCategoryListMessage(payload)
    , m_categories(m_payload.body.categories, payload.body.categories_count)
  {}
  BandCategoryList(const BandCategoryList& other) : BandCategoryList(other.m_payload) {}

  BandCategoryList(const Proto& bands) //TODO: remove this and RadioLookup
    : BandCategoryListMessage()
    , m_categories(m_payload.body.categories, bands.categories_count)
  {
    m_payload.header.payloadType = makesdr_RadioPayloadType_PAYLOAD_BANDS;
    m_payload.header.purpose = makesdr_RadioPayloadPurpose_PURPOSE_NONE;
    m_payload.body = bands;
    m_payload.has_header = true;
    m_payload.has_body = true;
  }

  BandCategoryList(BandCategoryList&& other)  noexcept
    : BandCategoryListMessage(other.m_payload)
    , m_categories(m_payload.body.categories, other.m_payload.body.categories_count)
  {
  }

  BandCategoryList& operator=(const BandCategoryList& other)
  {
    if (this != &other) {
      m_payload = other.m_payload;
      m_categories = BandCategorySpan(m_payload.body.categories, m_payload.body.categories_count);
    }
    return *this;
  }

  [[nodiscard]] uint32_t count() const { return m_payload.body.categories_count; }

  [[nodiscard]] const BandCategorySpan& categories() const { return m_categories; }

  [[nodiscard]] const makesdr_BandCategoryPb* findCategory(const BandCategoryName& name) const
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      if (name == m_categories[i].name) {
        return &m_categories[i];
      }
    }
    return nullptr;
  }

  [[nodiscard]] int findCategoryIndex(const BandCategoryName& name) const
  {
    uint32_t max = count();
    for (int i = 0; i < max; i++) {
      if (name == m_categories[i].name) {
        return i;
      }
    }
    return -1;
  }

  [[nodiscard]] const makesdr_BandPb* findBand(const BandName& name) const
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      BandList bands(m_categories[i].bands);
      const makesdr_BandPb* band = bands.findBand(name);
      if (band != nullptr) {
        return band;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const makesdr_BandCategoryPb* findCategoryOfBand(const BandName& name) const
  {
    uint32_t max = count();
    for (uint32_t i = 0; i < max; i++) {
      BandList bands(m_categories[i].bands);
      const makesdr_BandPb* band = bands.findBand(name);
      if (band != nullptr) {
        return &m_categories[i];
      }
    }
    return nullptr;
  }

  [[nodiscard]] int findCategoryIndexOfBand(const BandName& name) const
  {
    uint32_t max = count();
    for (int i = 0; i < max; i++) {
      BandList bands(m_categories[i].bands);
      const makesdr_BandPb* band = bands.findBand(name);
      if (band != nullptr) {
        return i;
      }
    }
    return -1;
  }


protected:
  BandCategorySpan m_categories;
};
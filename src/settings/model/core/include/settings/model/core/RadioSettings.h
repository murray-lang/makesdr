#pragma once

#include "BandSettingsCache.h"
#include "SettingUpdateVariant.h"
#include "MessageVisitor.h"
#include "SplitBandId.h"
#include "PipelineId.h"
#include "SettingUpdateSink.h"
#include "settings/model/meta/RadioMeta.h"
#include "settings/model/proto/RadioPayloads.pb.h"

class RadioSettings : public SettingUpdateSink
{
public:
  RadioSettings(
    const makesdr_RadioMetaPb& meta,
    BandSettingsCache& cache
  );

  RadioSettings(
    makesdr_RadioSettingsPb& raw,
    const makesdr_RadioMetaPb& meta,
    BandSettingsCache& cache
  );

  makesdr_RadioSettingsPb& body() { return m_payload.body; }
  [[nodiscard]] const makesdr_RadioSettingsPb& body() const { return m_payload.body; }

  [[nodiscard]] makesdr_RadioPayloadPurpose purpose() const { return m_payload.purpose; }

  ResultCode writeProtobuf(
    makesdr_RadioPayloadPurpose purpose,
    uint8_t *buffer,
    size_t buffer_size,
    size_t* bytes_written
    );
  ResultCode readProtobuf(const uint8_t *buffer, size_t msg_length);


  void replace(makesdr_RadioSettingsPb& update, bool assumeComplete = false);
  ResultCode merge(const makesdr_RadioSettingsPb& update);

  ResultCode autoComplete();

  ResultCode setAllFieldsPresence(bool present);

  void assumeComplete(bool assumeComplete) { m_assumeComplete = assumeComplete; }
  [[nodiscard]] bool assumeComplete() const { return m_assumeComplete; }

  void copyTo(makesdr_RadioSettingsPb& out) const;

  ResultCode applySettingUpdate(const SettingUpdate &settingUpdate) override;

  ResultCode updateIndirectField(const SettingUpdate &settingUpdate, uint32_t startingAtIndex);
  ResultCode updateField(const SettingPath &path, const SettingUpdateVariant &value);
  ResultCode getField(const SettingPath &path, SettingUpdateVariant &value) const;
  ResultCode getField(
    const SettingPath &path,
    SettingUpdateVariant &value,
    bool mustHave,
    bool parentsMustHave,
    bool& retrieved
  );

  ResultCode setFieldPresence(const SettingPath &path, bool present);
  ResultCode mergePresentFields(const void* pRhsMessage);

  static ResultCode resolveDottedPath(
    const char *dottedPath,
    SettingPath &path,
    bool* isIndirectOut,
    AutoCompleteTrigger* triggerOut
  );

  [[nodiscard]] const makesdr_BandSettingsPb* getBandSettings(SplitBandId bandId) const;

  // [[nodiscard]] uint32_t getFocusBandTag() const
  // {
  //   switch (m_rawSettings.active_bands.focus_band_id)
  //   {
  //   case makesdr_SplitBandId_SPLIT_BAND_ONE: return makesdr_ActiveBandSettingsPb_band_1_tag;
  //   case makesdr_SplitBandId_SPLIT_BAND_TWO: return makesdr_ActiveBandSettingsPb_band_2_tag;
  //   default: return 0;
  //   }
  // }
  //
  // [[nodiscard]] uint32_t getFocusPipelineTag(uint32_t bandTag) const
  // {
  //   switch(bandTag) {
  //     case makesdr_ActiveBandSettingsPb_band_1_tag:
  //     return getPipelineTag(static_cast<PipelineId>(m_rawSettings.active_bands.band_1.focus_pipeline_id));
  //     case makesdr_ActiveBandSettingsPb_band_2_tag:
  //     return getPipelineTag(static_cast<PipelineId>(m_rawSettings.active_bands.band_2.focus_pipeline_id));
  //     default: return 0;
  //   }
  // }
  //
  // [[nodiscard]] uint32_t getPipelineTag(PipelineId pipelineId) const
  // {
  //   switch (pipelineId) {
  //   case PipelineId::A: return makesdr_BandSettingsPb_pipeline_a_tag;
  //   case PipelineId::B: return makesdr_BandSettingsPb_pipeline_b_tag;
  //   default: return 0;
  //   }
  // }

  void ptt(bool on);
  bool hasActiveBands() const { return m_payload.body.has_active_bands; }

  [[nodiscard]] SplitBandId getFocusBandId() const { return static_cast<SplitBandId>(m_payload.body.active_bands.focus_band_id); }
  [[nodiscard]] SplitBandId getRxBandId() const { return static_cast<SplitBandId>(m_payload.body.active_bands.rx_band_id); }
  [[nodiscard]] SplitBandId getTxBandId() const { return static_cast<SplitBandId>(m_payload.body.active_bands.tx_band_id); }

  [[nodiscard]] PipelineId getFocusPipelineId(SplitBandId bandId) const;
  [[nodiscard]] PipelineId getFocusBandFocusPipelineId() const;
  [[nodiscard]] const makesdr_BandSettingsPb* getFocusBandSettings() const;
  [[nodiscard]] const makesdr_BandSettingsPb* getTxBandSettings() const;
  [[nodiscard]] const makesdr_RxPipelineSettingsPb* getFocusRxPipelineSettings(SplitBandId bandId) const;
  [[nodiscard]] const makesdr_RxPipelineSettingsPb* getTxPipelineSettings() const;
  [[nodiscard]] const makesdr_RxPipelineSettingsPb* getFocusBandFocusRxPipelineSettings() const;


protected:
  void InitBandAndPipelineIdsWithDefaults();

  ResultCode updateIndirectActiveBandsField(
    makesdr_ActiveBandSettingsPb& rawBandSettings,
    const SettingUpdate &settingUpdate,
    uint32_t startingAtIndex
    );

  ResultCode updateIndirectBandField(
    makesdr_BandSettingsPb& rawBandSettings,
    const SettingUpdate &settingUpdate,
    uint32_t startingAtIndex
  );

  ResultCode autoComplete(const SettingPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger);
  ResultCode autoCompleteActiveBands(const SettingPath& path, uint32_t startingAtIndex, AutoCompleteTrigger trigger);
  ResultCode autoCompleteBand(
    makesdr_BandSettingsPb& rawBandSettings,
    const SettingPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger
  );
  ResultCode autoCompleteSplit(makesdr_ActiveBandSettingsPb& rawActiveBandSettings);
  ResultCode autoCompleteBandRequest(makesdr_BandSettingsPb& rawBandSettings);
  ResultCode autoCompleteMultiPipeline(makesdr_BandSettingsPb& rawBandSettings);
  ResultCode autoCompletePipeline(
    makesdr_PipelineSettingsPb& rawPipelineSettings,
    const SettingPath& path,
    uint32_t startingAtIndex,
    AutoCompleteTrigger trigger
  );
  ResultCode autoCompleteMode(makesdr_PipelineSettingsPb& rawPipelineSettings);

  ResultCode autoComplete(makesdr_ActiveBandSettingsPb& rawActiveBandSettings);
  ResultCode autoComplete(makesdr_BandSettingsPb& rawBandSettings);
  ResultCode autoComplete(makesdr_PipelineSettingsPb& rawPipelineSettings);

  ResultCode applyBandDefaults(makesdr_BandSettingsPb& rawBandSettings);
  ResultCode applyBandDefaults(const makesdr_BandPb& rawBand, makesdr_PipelineSettingsPb& rawPipeline);
  ResultCode applyBandDefaults(const makesdr_BandPb& rawBand, makesdr_RfSettingsPb& rawRf);


protected:
  bool m_assumeComplete;
  makesdr_RadioSettingsPayloadPb m_payload;

  MessageVisitor m_visitor;

  RadioMeta m_meta;
  BandSettingsCache& m_cache;
};

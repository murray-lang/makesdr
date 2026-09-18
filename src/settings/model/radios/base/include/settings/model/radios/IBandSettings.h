#pragma once
#include <ResultCode.h>

#include <settings/model/data/radio/RadioLookup.h>
#include "BandRfSettings.h"
#include "IfSettings.h"
#include "PipelineId.h"
#include "IApplyBandDefaults.h"

class RxPipelineSettings;
class TxPipelineSettings;

class IBandSettings : public IApplyBandDefaults
{
public:
  virtual ~IBandSettings() = default;

  [[nodiscard]] virtual const Band* getBand() const = 0;
  [[nodiscard]] virtual const Mode* getFocusMode() const = 0;
  [[nodiscard]] virtual bool hasRfSettings() const = 0;
  virtual BandRfSettings* rfSettings() = 0;
  [[nodiscard]] virtual const BandRfSettings* rfSettings() const = 0;

  [[nodiscard]] virtual bool hasIfSettings() const = 0;
  virtual IfSettings* ifSettings() = 0;
  [[nodiscard]] virtual const IfSettings* ifSettings() const = 0;

  [[nodiscard]] virtual bool hasFocusPipelineId() const  { return false; }
  [[nodiscard]] virtual PipelineId focusPipelineId() const { return PipelineId::NONE; }
  [[nodiscard]] virtual bool hasFocusPipeline() const  { return false; }
  virtual RxPipelineSettings* focusPipeline() { return nullptr; }
  [[nodiscard]] virtual const RxPipelineSettings* focusPipeline() const { return nullptr; }

  [[nodiscard]] virtual bool hasPipeline(PipelineId pipelineId) const { return false; }
  virtual RxPipelineSettings* pipeline(PipelineId pipelineId) { return nullptr; }
  [[nodiscard]] virtual const RxPipelineSettings* pipeline(PipelineId pipelineId) const { return nullptr; }

  [[nodiscard]] virtual bool hasTxPipeline() const { return false; }
  virtual TxPipelineSettings* txPipeline() { return nullptr; }
  [[nodiscard]] virtual const TxPipelineSettings* txPipeline() const { return nullptr; }

  [[nodiscard]] virtual bool hasIsMultiPipeline() const { return false; }
  [[nodiscard]] virtual bool isMultiPipeline() const { return false; }

  virtual ResultCode updateIndirectField(const FieldUpdate &settingUpdate, uint32_t startingAtIndex) = 0;

  // virtual ResultCode autoComplete(const RadioLookup& lookup) = 0;
  // virtual ResultCode autoComplete(FieldDescriptor& setting, uint32_t startIndex, const RadioLookup& lookup) = 0;
};

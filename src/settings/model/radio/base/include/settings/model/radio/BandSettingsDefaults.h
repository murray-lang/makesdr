#pragma once

#include "PipelineId.h"

class RxPipelineSettings;
class TxPipelineSettings;

// Non-virtual fallbacks for band settings classes that lack some pipeline
// features. A band settings class that supports a feature hides the fallback
// with its own member of the same name. This is not an interface: band
// settings are only ever accessed through their concrete type.
class BandSettingsDefaults
{
public:
  [[nodiscard]] bool hasFocusPipelineId() const  { return false; }
  [[nodiscard]] PipelineId focusPipelineId() const { return PipelineId::NONE; }
  [[nodiscard]] bool hasFocusPipeline() const  { return false; }
  RxPipelineSettings* focusPipeline() { return nullptr; }
  [[nodiscard]] const RxPipelineSettings* focusPipeline() const { return nullptr; }

  [[nodiscard]] bool hasPipeline(PipelineId pipelineId) const { return false; }
  RxPipelineSettings* pipeline(PipelineId pipelineId) { return nullptr; }
  [[nodiscard]] const RxPipelineSettings* pipeline(PipelineId pipelineId) const { return nullptr; }

  [[nodiscard]] bool hasTxPipeline() const { return false; }
  TxPipelineSettings* txPipeline() { return nullptr; }
  [[nodiscard]] const TxPipelineSettings* txPipeline() const { return nullptr; }

  [[nodiscard]] bool hasIsMultiPipeline() const { return false; }
  [[nodiscard]] bool isMultiPipeline() const { return false; }
};

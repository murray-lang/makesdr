#pragma once
#include "PipelineId.h"

class Band;
class Mode;
class BandRfSettings;
class RxPipelineSettings;

// Read-only, type-neutral view of one band's settings, for the front end only.
// Mirrors the shape that BandSettingsDefaults gives every band settings class:
// a single-pipeline band has only pipeline A, is never multi-pipeline and has no TX pipeline.
// has*() reports presence, so on a view of changes it says what changed.
class IBandSettingsView
{
public:
  virtual ~IBandSettingsView() = default;

  [[nodiscard]] virtual const Band* getBand() const = 0;
  [[nodiscard]] virtual const Mode* getFocusMode() const = 0;

  [[nodiscard]] virtual bool hasRfSettings() const = 0;
  [[nodiscard]] virtual const BandRfSettings* rfSettings() const = 0;

  [[nodiscard]] virtual bool hasFocusPipelineId() const = 0;
  [[nodiscard]] virtual PipelineId focusPipelineId() const = 0;
  [[nodiscard]] virtual bool hasFocusPipeline() const = 0;
  [[nodiscard]] virtual const RxPipelineSettings* focusPipeline() const = 0;

  // nullptr if the band doesn't have that pipeline
  [[nodiscard]] virtual const RxPipelineSettings* pipeline(PipelineId pipelineId) const = 0;

  [[nodiscard]] virtual bool hasIsMultiPipeline() const = 0;
  [[nodiscard]] virtual bool isMultiPipeline() const = 0;

  [[nodiscard]] virtual bool hasTxPipeline() const = 0;
  [[nodiscard]] virtual bool hasTxPipelineId() const = 0;
  // PipelineId::NONE if the band has no TX pipeline
  [[nodiscard]] virtual PipelineId txPipelineId() const = 0;
};

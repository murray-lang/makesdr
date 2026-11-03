#pragma once

#include <cstddef>
#include <cstdint>
#include <new>

#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>

#include "Stm32H745MessageRings.h"

//*****************************************************************************
// The one set of rings shared by the CM4 (radio) and the CM7 (UI), reached
// through global accessors as the Qt build's rings are (globalQtMetaRings()
// etc.), so neither side needs to know about the other.
//
// Unlike Qt's, these live in SRAM4, in the .shared_rings block that the CM7
// and CM4 linker scripts both reserve at its start, and they must exist
// before the CM4 runs. So the CM7 creates them explicitly, once, before
// waking the CM4 (and after MPU_Config() has made SRAM4 non-cacheable):
//     createStm32H745Rings<SingleIqRxSettings>();
// (or the radio's own wrapper, e.g. createStm32H745SingleIqRxRings()).
//
// The two images are built separately, and there may be several CM4 images,
// so the first accessor used on each core checks a header written by the
// CM7: a magic number, the layout version and the size of what follows; the
// settings accessor also checks the settings type. A mismatch halts with
// configASSERT rather than letting the cores corrupt each other's data.
// Bump LayoutVersion whenever the set or order of rings changes.
//
// Settings rings come last because their type depends on the radio; the
// families every radio shares come first, so their accessors need no
// settings type.
//*****************************************************************************

Stm32H745MetaMessageRings&       globalStm32H745MetaRings();
Stm32H745RxMeteringMessageRings& globalStm32H745RxMeteringRings();
Stm32H745UpdateMessageRings&     globalStm32H745UpdateRings();
Stm32H745SpectrumMessageRings&   globalStm32H745SpectrumRings();

namespace Stm32H745Rings
{
  inline constexpr uint32_t Magic = 0x5244534D;   // "MSDR"
  inline constexpr uint32_t LayoutVersion = 1;

  struct Header
  {
    uint32_t magic;
    uint32_t layoutVersion;
    uint32_t commonSize;
    uint32_t settingsPayloadType;
    uint32_t settingsSize;
    uint32_t ready;
  };

  // The families every radio shares.
  struct Common
  {
    Stm32H745MetaMessageRings       meta;
    Stm32H745RxMeteringMessageRings rxMetering;
    Stm32H745UpdateMessageRings     updates;
    Stm32H745SpectrumMessageRings   spectrum;
  };

  constexpr size_t alignUp(size_t n) { return (n + 31) & ~size_t{31}; }
  inline constexpr size_t CommonOffset = alignUp(sizeof(Header));
  inline constexpr size_t SettingsOffset = CommonOffset + alignUp(sizeof(Common));

  // Defined in globalStm32H745Rings.cpp.
  Header* header();
  uint8_t* base();
  size_t   capacity();
  void     checkCommon();     // configASSERTs the header, once per core
  void     markCreated();     // configASSERTs that creation happens only once
}

// CM7 only, once, before waking the CM4.
template<typename RadioSettingsT>
void createStm32H745Rings()
{
  using namespace Stm32H745Rings;
  using SettingsRings = Stm32H745SettingsMessageRingsT<RadioSettingsT>;
  static_assert(alignof(Common) <= 32 && alignof(SettingsRings) <= 32);

  markCreated();
  configASSERT(SettingsOffset + sizeof(SettingsRings) <= capacity());  // enlarge .shared_rings

  Header* h = header();
  h->ready = 0;
  __DSB();

  new (base() + CommonOffset) Common();
  new (base() + SettingsOffset) SettingsRings();

  h->magic = Magic;
  h->layoutVersion = LayoutVersion;
  h->commonSize = sizeof(Common);
  h->settingsPayloadType = RadioSettingsT::payloadType;
  h->settingsSize = sizeof(SettingsRings);
  __DSB();
  h->ready = Magic;     // last: everything above is in place
  __DSB();
}

template<typename RadioSettingsT>
Stm32H745SettingsMessageRingsT<RadioSettingsT>& globalStm32H745SettingsRings()
{
  using namespace Stm32H745Rings;
  using SettingsRings = Stm32H745SettingsMessageRingsT<RadioSettingsT>;

  checkCommon();
  const Header* h = header();
  configASSERT(h->settingsPayloadType == static_cast<uint32_t>(RadioSettingsT::payloadType));
  configASSERT(h->settingsSize == sizeof(SettingsRings));
  return *std::launder(reinterpret_cast<SettingsRings*>(base() + SettingsOffset));
}

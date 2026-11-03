#include "transport/stm32h745i/globalStm32H745Rings.h"

// Reserved by the linker scripts at the start of SRAM4 (.shared_rings).
extern "C" uint8_t _sshared_rings[];
extern "C" uint8_t _eshared_rings[];

namespace Stm32H745Rings
{
  Header*  header()   { return reinterpret_cast<Header*>(_sshared_rings); }
  uint8_t* base()     { return _sshared_rings; }
  size_t   capacity() { return static_cast<size_t>(_eshared_rings - _sshared_rings); }

  void checkCommon()
  {
    static bool checked = false;
    if (checked) {
      return;
    }
    const Header* h = header();
    configASSERT(h->ready == Magic);            // CM7 has not created the rings
    configASSERT(h->magic == Magic);
    configASSERT(h->layoutVersion == LayoutVersion);
    configASSERT(h->commonSize == sizeof(Common));
    checked = true;
  }

  void markCreated()
  {
    static bool created = false;
    configASSERT(!created);
    created = true;
  }
}

namespace
{
  Stm32H745Rings::Common& common()
  {
    Stm32H745Rings::checkCommon();
    return *std::launder(reinterpret_cast<Stm32H745Rings::Common*>(
      Stm32H745Rings::base() + Stm32H745Rings::CommonOffset));
  }
}

Stm32H745MetaMessageRings&       globalStm32H745MetaRings()       { return common().meta; }
Stm32H745RxMeteringMessageRings& globalStm32H745RxMeteringRings() { return common().rxMetering; }
Stm32H745UpdateMessageRings&     globalStm32H745UpdateRings()     { return common().updates; }
Stm32H745SpectrumMessageRings&   globalStm32H745SpectrumRings()   { return common().spectrum; }

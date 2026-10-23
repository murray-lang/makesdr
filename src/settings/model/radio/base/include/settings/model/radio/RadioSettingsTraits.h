#pragma once

// Runtime copy of a settings class's compile-time Traits, for code that only
// needs them as a value rather than a type.
struct RadioSettingsTraits
{
  bool hasRx;
  bool hasTx;
  bool isDualIq;
  bool isSplitBand;
};

template<typename T>
constexpr RadioSettingsTraits toRuntimeTraits()
{
  return { T::hasRx, T::hasTx, T::isDualIq, T::isSplitBand };
}

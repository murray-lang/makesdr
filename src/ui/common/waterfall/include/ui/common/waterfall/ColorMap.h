#pragma once

#include "PixelFormat.h"

#include <array>
#include <cstddef>
#include <cstdint>

// A gradient stop. Position is 0..1 along the map; stops must be in
// ascending position order.
struct ColorStop
{
  float position;
  Rgb color;
};

namespace ColorMaps
{
  // Black -> blue -> cyan -> green -> yellow -> red -> white, the usual SDR look
  inline constexpr ColorStop Classic[] = {
    { 0.00f, {   0,   0,   0 } },
    { 0.15f, {   0,   0, 140 } },
    { 0.35f, {   0, 160, 255 } },
    { 0.50f, {   0, 220,  60 } },
    { 0.65f, { 255, 230,   0 } },
    { 0.85f, { 255,  40,   0 } },
    { 1.00f, { 255, 255, 255 } },
  };

  inline constexpr ColorStop Grayscale[] = {
    { 0.0f, {   0,   0,   0 } },
    { 1.0f, { 255, 255, 255 } },
  };
}

// 256-entry lookup table from intensity level to packed pixel, built once from
// gradient stops so the per-pixel cost of coloring is a single array index.
template<typename Format>
class ColorMap
{
public:
  using Pixel = typename Format::Pixel;
  static constexpr size_t LEVELS = 256;

  ColorMap() { build(ColorMaps::Classic); }

  template<size_t N>
  void build(const ColorStop (&stops)[N]) { build(stops, N); }

  void build(const ColorStop* stops, size_t count)
  {
    for (size_t level = 0; level < LEVELS; level++) {
      float t = static_cast<float>(level) / static_cast<float>(LEVELS - 1);
      m_lut[level] = Format::pack(interpolate(stops, count, t));
    }
  }

  Pixel operator[](uint8_t level) const { return m_lut[level]; }

private:
  static Rgb interpolate(const ColorStop* stops, size_t count, float t)
  {
    if (count == 0) {
      return { 0, 0, 0 };
    }
    if (t <= stops[0].position) {
      return stops[0].color;
    }
    for (size_t i = 1; i < count; i++) {
      if (t <= stops[i].position) {
        const ColorStop& a = stops[i - 1];
        const ColorStop& b = stops[i];
        float span = b.position - a.position;
        float f = span > 0.0f ? (t - a.position) / span : 1.0f;
        return { lerp(a.color.r, b.color.r, f), lerp(a.color.g, b.color.g, f), lerp(a.color.b, b.color.b, f) };
      }
    }
    return stops[count - 1].color;
  }

  static uint8_t lerp(uint8_t a, uint8_t b, float f)
  {
    return static_cast<uint8_t>(static_cast<float>(a) + (static_cast<float>(b) - static_cast<float>(a)) * f + 0.5f);
  }

  std::array<Pixel, LEVELS> m_lut{};
};

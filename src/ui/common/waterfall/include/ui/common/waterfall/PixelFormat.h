#pragma once

#include <cstdint>

// Platform-neutral color and pixel packing for the waterfall. A PixelFormat
// is a traits type providing `Pixel` and `static Pixel pack(Rgb)`, so the
// waterfall can write directly into whatever buffer the UI toolkit renders.

struct Rgb
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// 0xAARRGGBB in a native uint32_t.
// Qt:   QImage::Format_RGB32 / Format_ARGB32 (QRgb)
// LVGL: LV_COLOR_FORMAT_XRGB8888 / ARGB8888 (little-endian B,G,R,A in memory)
struct Argb8888
{
  using Pixel = uint32_t;

  static constexpr Pixel pack(Rgb c)
  {
    return 0xFF000000u
      | (static_cast<uint32_t>(c.r) << 16)
      | (static_cast<uint32_t>(c.g) << 8)
      | static_cast<uint32_t>(c.b);
  }
};

// RRRRRGGGGGGBBBBB in a native uint16_t.
// Qt:   QImage::Format_RGB16
// LVGL: LV_COLOR_FORMAT_RGB565 (LV_COLOR_16_SWAP off)
struct Rgb565
{
  using Pixel = uint16_t;

  static constexpr Pixel pack(Rgb c)
  {
    return static_cast<uint16_t>(
      ((c.r & 0xF8u) << 8) | ((c.g & 0xFCu) << 3) | (c.b >> 3));
  }
};

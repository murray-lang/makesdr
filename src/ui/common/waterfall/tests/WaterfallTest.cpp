#include <gtest/gtest.h>

#include <ui/common/waterfall/Waterfall.h>

#include <vector>

namespace
{
  constexpr uint16_t WIDTH = 8;
  constexpr uint16_t HEIGHT = 4;

  // Grayscale over 0..255 dB makes level == dB, so pixels are easy to predict
  struct Fixture
  {
    explicit Fixture(Waterfall<Argb8888>::ScrollMode mode = Waterfall<Argb8888>::ScrollMode::Ring)
      : pixels(WIDTH * HEIGHT, 0xDEADBEEF)
    {
      waterfall.colorMap().build(ColorMaps::Grayscale);
      waterfall.setDbRange(0.0f, 255.0f);
      waterfall.attach(pixels.data(), WIDTH, HEIGHT, WIDTH * sizeof(uint32_t), mode);
    }

    void add(const std::vector<float>& bins, int64_t centre = 1000, uint32_t span = 800, bool shuffled = false)
    {
      waterfall.addSpectrum<float>({ bins.data(), bins.size(), centre, span, shuffled });
    }

    // Grey level of display row `row` (0 = newest), column `x`
    uint8_t at(uint16_t row, uint16_t x)
    {
      Waterfall<Argb8888>::Segment segs[2];
      size_t n = waterfall.segments(segs);
      for (size_t i = 0; i < n; i++) {
        if (row >= segs[i].displayRow && row < segs[i].displayRow + segs[i].rows) {
          uint16_t bufferRow = segs[i].sourceRow + (row - segs[i].displayRow);
          return static_cast<uint8_t>(waterfall.bufferRow(bufferRow)[x] & 0xFF);
        }
      }
      ADD_FAILURE() << "row not covered by segments";
      return 0;
    }

    std::vector<uint32_t> pixels;
    Waterfall<Argb8888> waterfall;
  };

  std::vector<float> ramp(float start)
  {
    std::vector<float> v(WIDTH);
    for (uint16_t i = 0; i < WIDTH; i++) {
      v[i] = start + static_cast<float>(i);
    }
    return v;
  }
}

TEST(ColorMapTest, InterpolatesBetweenStops)
{
  ColorMap<Argb8888> map;
  map.build(ColorMaps::Grayscale);
  EXPECT_EQ(map[0], 0xFF000000u);
  EXPECT_EQ(map[255], 0xFFFFFFFFu);
  EXPECT_EQ(map[128] & 0xFF, 128u);
}

TEST(PixelFormatTest, Rgb565Packs)
{
  EXPECT_EQ(Rgb565::pack({ 255, 255, 255 }), 0xFFFF);
  EXPECT_EQ(Rgb565::pack({ 255, 0, 0 }), 0xF800);
  EXPECT_EQ(Rgb565::pack({ 0, 255, 0 }), 0x07E0);
  EXPECT_EQ(Rgb565::pack({ 0, 0, 255 }), 0x001F);
}

TEST(WaterfallTest, AttachClearsToBackground)
{
  Fixture f;
  for (uint32_t p : f.pixels) {
    EXPECT_EQ(p, 0xFF000000u);
  }
}

TEST(WaterfallTest, NewestRowIsAtTopInRingMode)
{
  Fixture f;
  f.add(ramp(10));
  f.add(ramp(20));
  f.add(ramp(30));
  EXPECT_EQ(f.at(0, 0), 30);
  EXPECT_EQ(f.at(1, 0), 20);
  EXPECT_EQ(f.at(2, 7), 17);
  EXPECT_EQ(f.at(3, 0), 0);

  // Wrap the ring
  f.add(ramp(40));
  f.add(ramp(50));
  EXPECT_EQ(f.at(0, 0), 50);
  EXPECT_EQ(f.at(3, 0), 20);
}

TEST(WaterfallTest, ShiftModeKeepsDisplayOrder)
{
  Fixture f(Waterfall<Argb8888>::ScrollMode::Shift);
  f.add(ramp(10));
  f.add(ramp(20));
  Waterfall<Argb8888>::Segment segs[2];
  ASSERT_EQ(f.waterfall.segments(segs), 1u);
  EXPECT_EQ(f.pixels[0] & 0xFF, 20u);
  EXPECT_EQ(f.pixels[WIDTH] & 0xFF, 10u);
}

TEST(WaterfallTest, ShuffledBinsPutDcInTheMiddle)
{
  Fixture f;
  // Raw FFT order: DC at 0, negative frequencies in the upper half
  f.add({ 100, 101, 102, 103, 200, 201, 202, 203 }, 1000, 800, true);
  EXPECT_EQ(f.at(0, 0), 200);
  EXPECT_EQ(f.at(0, 4), 100);
}

TEST(WaterfallTest, DecimationKeepsPeak)
{
  Fixture f;
  std::vector<float> bins(WIDTH * 4, 5.0f);
  bins[13] = 99.0f;     // pixel 3 covers bins 12..15
  f.add(bins);
  EXPECT_EQ(f.at(0, 3), 99);
  EXPECT_EQ(f.at(0, 2), 5);
}

TEST(WaterfallTest, ClampsOutOfRangeAndNonFinite)
{
  Fixture f;
  f.add({ -50, 300, -INFINITY, NAN, 0, 0, 0, 0 });
  EXPECT_EQ(f.at(0, 0), 0);
  EXPECT_EQ(f.at(0, 1), 255);
  EXPECT_EQ(f.at(0, 2), 0);
  EXPECT_EQ(f.at(0, 3), 0);
}

TEST(WaterfallTest, RetuneShiftsHistory)
{
  Fixture f;          // 800 Hz over 8 px = 100 Hz/px
  f.add(ramp(10), 1000);
  f.add(ramp(20), 1200);   // tuned up two pixels
  EXPECT_EQ(f.at(0, 0), 20);
  EXPECT_EQ(f.at(1, 0), 12);   // old content moved left
  EXPECT_EQ(f.at(1, 5), 17);
  EXPECT_EQ(f.at(1, 6), 0);    // vacated columns are background
}

TEST(WaterfallTest, SpanChangeClearsHistory)
{
  Fixture f;
  f.add(ramp(10), 1000, 800);
  f.add(ramp(20), 1000, 1600);
  EXPECT_EQ(f.at(0, 0), 20);
  EXPECT_EQ(f.at(1, 0), 0);
}

TEST(WaterfallTest, FrequencyAtMapsColumns)
{
  Fixture f;
  f.add(ramp(10), 1000, 800);
  EXPECT_DOUBLE_EQ(f.waterfall.frequencyAt(0), 600.0);
  EXPECT_DOUBLE_EQ(f.waterfall.frequencyAt(4), 1000.0);
}

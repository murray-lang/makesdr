#include <gtest/gtest.h>
#include <transport/PayloadTypeBits.h>

#include <vector>

TEST(PayloadTypeBitsTest, BitIsThePayloadTypeValue)
{
  EXPECT_EQ(PayloadTypeBits::maskOf(PAYLOAD_TYPE_CAPABILITIES), uint32_t{1} << PAYLOAD_TYPE_CAPABILITIES);
  EXPECT_EQ(PayloadTypeBits::maskOf(PAYLOAD_TYPE_SPECTRUM), uint32_t{1} << PAYLOAD_TYPE_SPECTRUM);
}

TEST(PayloadTypeBitsTest, ForEachVisitsSetBitsLowestFirst)
{
  const uint32_t mask = PayloadTypeBits::maskOf(PAYLOAD_TYPE_SPECTRUM)
                      | PayloadTypeBits::maskOf(PAYLOAD_TYPE_CAPABILITIES)
                      | PayloadTypeBits::maskOf(PAYLOAD_TYPE_MODES);
  std::vector<PayloadType> visited;
  PayloadTypeBits::forEach(mask, [&](PayloadType payloadType) { visited.push_back(payloadType); });

  EXPECT_EQ(visited, (std::vector<PayloadType>{
    PAYLOAD_TYPE_CAPABILITIES, PAYLOAD_TYPE_MODES, PAYLOAD_TYPE_SPECTRUM
  }));
}

TEST(PayloadTypeBitsTest, ForEachOnEmptyMaskVisitsNothing)
{
  int visited = 0;
  PayloadTypeBits::forEach(0, [&](PayloadType) { ++visited; });
  EXPECT_EQ(visited, 0);
}

#ifndef LINUX_GPIO_LINE_DESCRIPTOR_H
#define LINUX_GPIO_LINE_DESCRIPTOR_H

#include <tuple>
#include <vector>
#include <gpiod.h>

using GpioLineDescriptor = std::tuple<gpiod_line_request*, uint32_t>;
using GpioLineDescriptorVector = std::vector<GpioLineDescriptor>;

inline gpiod_line_request* get0(GpioLineDescriptor& descriptor) { return etl::get<0>(request); }
inline uint32_t get1(GpioLineDescriptor& descriptor) { return etl::get<1>(request); }

#endif // LINUX_GPIO_LINE_DESCRIPTOR_H
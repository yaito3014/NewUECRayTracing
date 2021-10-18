#pragma once

#ifndef YK_RAYTRACING_CONFIG_HPP
#define YK_RAYTRACING_CONFIG_HPP

#ifndef YK_CONFIG_IMG_WIDTH
#define YK_CONFIG_IMG_WIDTH 400
#endif  // !YK_CONFIG_IMG_WIDTH

#ifndef YK_CONFIG_SPP
#define YK_CONFIG_SPP 100
#endif  // !YK_CONFIG_SPP

namespace yk {

namespace constants {

constexpr auto aspect_ratio = 16.0 / 9.0;
constexpr auto focal_length = 1.0;

constexpr std::size_t image_width = YK_CONFIG_IMG_WIDTH;
constexpr std::size_t image_height = std::size_t(image_width / aspect_ratio);
constexpr auto samples_per_pixel = YK_CONFIG_SPP;
constexpr auto max_depth = 50u;

}  // namespace constants

}  // namespace yk

#endif  // !YK_RAYTRACING_CONFIG_HPP

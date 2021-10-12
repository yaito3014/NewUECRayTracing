#pragma once

#ifndef YK_RAYTRACING_CONFIG_HPP
#define YK_RAYTRACING_CONFIG_HPP

namespace yk {

namespace constants {

constexpr auto aspect_ratio = 16.0 / 9.0;
constexpr auto viewport_height = 2.0;
constexpr auto viewport_width = aspect_ratio * viewport_height;
constexpr auto focal_length = 1.0;

constexpr std::size_t image_width = 400;
constexpr std::size_t image_height = image_width / aspect_ratio;
constexpr auto samples_per_pixel = 50u;
constexpr auto max_depth = 50u;

}  // namespace constants

}  // namespace yk

#endif  // !YK_RAYTRACING_CONFIG_HPP

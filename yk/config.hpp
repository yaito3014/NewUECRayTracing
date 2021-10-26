#pragma once

#ifndef YK_RAYTRACING_CONFIG_HPP
#define YK_RAYTRACING_CONFIG_HPP

#ifndef YK_CONFIG_IMG_WIDTH
#define YK_CONFIG_IMG_WIDTH 100
#endif  // !YK_CONFIG_IMG_WIDTH

#ifndef YK_CONFIG_SPP
#define YK_CONFIG_SPP 100
#endif  // !YK_CONFIG_SPP

namespace yk {

namespace constants {

inline constexpr auto focal_length = 1.0;
inline constexpr auto max_depth = 50u;

}  // namespace constants

inline auto aspect_ratio = 16.0 / 9.0;
inline std::size_t image_width = YK_CONFIG_IMG_WIDTH;
inline auto samples_per_pixel = YK_CONFIG_SPP;
inline std::size_t image_height = std::size_t(image_width / aspect_ratio);

}  // namespace yk

#endif  // !YK_RAYTRACING_CONFIG_HPP

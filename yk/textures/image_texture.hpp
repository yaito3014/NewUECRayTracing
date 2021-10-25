#pragma once

#ifndef YK_RAYTACING_IMAGE_TEXTURE_HPP
#define YK_RAYTACING_IMAGE_TEXTURE_HPP

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>

#include "../color.hpp"

namespace yk {

template <class T>
struct image_texture {
  static constexpr int bytes_per_pixel = 3;
  std::shared_ptr<std::byte> data;
  int width, height, bytes_per_scanline;

  image_texture(const char* filename) {
    auto components_per_pixel = bytes_per_pixel;
    data.reset(reinterpret_cast<std::byte*>(stbi_load(filename, &width, &height,
                                                      &components_per_pixel,
                                                      components_per_pixel)));
    if (!data) {
      std::cerr << "ERROR: Could not load texture image file '" << filename
                << "'.\n";
      width = height = 0;
    }
    bytes_per_scanline = bytes_per_pixel * width;
  }
  color<T> value(T u, T v, const pos3<T>&) const noexcept {
    if (!data) return {0, 1, 1};
    u = std::clamp<T>(u, 0, 1);
    v = 1 - std::clamp<T>(v, 0, 1);
    auto i = std::min<int>(u * width, width - 1);
    auto j = std::min<int>(v * height, height - 1);

    constexpr auto color_scale = T{1} / 255;
    auto pixel = data.get() + j * bytes_per_scanline + i * bytes_per_pixel;

    return {
        color_scale * static_cast<std::underlying_type_t<std::byte>>(pixel[0]),
        color_scale * static_cast<std::underlying_type_t<std::byte>>(pixel[1]),
        color_scale * static_cast<std::underlying_type_t<std::byte>>(pixel[2]),
    };
  }
};

}  // namespace yk

#endif  // !YK_RAYTACING_IMAGE_TEXTURE_HPP

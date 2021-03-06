#pragma once

#ifndef YK_RAYTRACING_SOLID_TEXTURE_HPP
#define YK_RAYTRACING_SOLID_TEXTURE_HPP

#include "../color.hpp"
#include "../vec3.hpp"

namespace yk {

template <class T>
struct solid_texture {
  color<T> color_value;

  constexpr color<T> value(T u, T v, const pos3<T>& p) const noexcept {
    return color_value;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_SOLID_TEXTURE_HPP

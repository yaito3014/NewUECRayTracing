#pragma once

#ifndef YK_RAYTRACING_DIFFUSE_LIGHT_HPP
#define YK_RAYTRACING_DIFFUSE_LIGHT_HPP

#include "../color.hpp"
#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../pos3.hpp"
#include "../ray.hpp"
#include "../texture.hpp"

namespace yk {

template <class T>
struct diffuse_light {
  texture<T> emit;

  template <class Gen>
  constexpr bool scatter(const ray<T>&, const hit_record<T>&, color<T>&,
                         ray<T>&, Gen&) const noexcept {
    return false;
  }

  constexpr color<T> emitted(T u, T v, const pos3<T>& p) const noexcept {
    return custom::value(emit, u, v, p);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_DIFFUSE_LIGHT_HPP

#pragma once

#ifndef YK_RAYTRACING_SCATTER_HPP
#define YK_RAYTRACING_SCATTER_HPP

#include "color.hpp"
#include "hit_record.hpp"
#include "material.hpp"
#include "ray.hpp"

namespace yk {

template <class T, class Gen>
constexpr bool scatter(const material<T>& mat, const ray<T>& r,
                       const hit_record<T>& rec, color<T>& attenuation,
                       ray<T>& scattered, Gen& gen) noexcept {
  return std::visit(
      [&](const auto& m) { return m.scatter(r, rec, attenuation, scattered, gen); },
      mat);
}

}  // namespace yk

#endif  // !YK_RAYTRACING_SCATTER_HPP

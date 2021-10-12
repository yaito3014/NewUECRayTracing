#pragma once

#ifndef YK_RAYTRACING_HIT_HPP
#define YK_RAYTRACING_HIT_HPP

#include "hit_record.hpp"
#include "hittable.hpp"
#include "ray.hpp"

namespace yk {

template <class T>
constexpr bool hit(const hittable<T>& h, const ray<T>& r, T t_min, T t_max,
                   hit_record<T>& rec) noexcept {
  return std::visit(
      [&](const auto& ho) { return ho.hit(r, t_min, t_max, rec); }, h);
}

}  // namespace yk

#endif  // !YK_RAYTRACING_HIT_HPP

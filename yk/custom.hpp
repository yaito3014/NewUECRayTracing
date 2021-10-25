#pragma once

#ifndef YK_RAYTRACING_CUSTOM_HPP
#define YK_RAYTRACING_CUSTOM_HPP

#include <variant>

#include "aabb.hpp"
#include "color.hpp"
#include "hit_record.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "texture.hpp"

namespace yk {

namespace custom {

template <class T>
constexpr bool hit(const hittable<T>& h, const ray<T>& r, T t_min, T t_max,
                   hit_record<T>& rec) noexcept {
  return std::visit(
      [&](const auto& ho) { return ho.hit(r, t_min, t_max, rec); }, h);
}

template <class T, class Gen>
constexpr bool scatter(const material<T>& mat, const ray<T>& r,
                       const hit_record<T>& rec, color<T>& attenuation,
                       ray<T>& scattered, Gen& gen) noexcept {
  return std::visit(
      [&](const auto& m) {
        return m.scatter(r, rec, attenuation, scattered, gen);
      },
      mat);
}

template <class T>
constexpr bool bounding_box(const hittable<T>& h, T time0, T time1,
                            aabb<T>& output_box) noexcept {
  return std::visit(
      [&](const auto& ho) { return ho.bounding_box(time0, time1, output_box); },
      h);
}

template <class T>
constexpr color<T> value(const texture<T>& tex, T u, T v,
                         const pos3<T>& p) noexcept {
  return std::visit([&](const auto& t) { return t.value(u, v, p); }, tex);
}

}  // namespace custom

}  // namespace yk

#endif  // !YK_RAYTRACING_CUSTOM_HPP

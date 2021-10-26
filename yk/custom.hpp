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

namespace detail {

template <class T, class Mat, class = void>
struct has_emitted : std::false_type {};

template <class T, class Mat>
struct has_emitted<
    T, Mat,
    std::void_t<decltype(std::declval<Mat>().emitted(
        std::declval<T>(), std::declval<T>(), std::declval<pos3<T>>()))>>
    : std::true_type {};

}  // namespace detail

template <class T>
constexpr color<T> emitted(const material<T>& mat, T u, T v,
                           const pos3<T>& p) noexcept {
  return std::visit(
      [&](const auto& m) -> color<T> {
        using M = std::remove_cv_t<std::remove_reference_t<decltype(m)>>;
        if constexpr (detail::has_emitted<T, M>::value)
          return m.emitted(u, v, p);
        else
          return {0, 0, 0};
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

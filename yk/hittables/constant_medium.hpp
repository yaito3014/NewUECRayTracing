#pragma once

#ifndef YK_RAYTRACING_CONSTANT_MEDIUM_HPP
#define YK_RAYTRACING_CONSTANT_MEDIUM_HPP

#include <algorithm>
#include <memory>

#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../hittable.hpp"
#include "../material.hpp"
#include "../math.hpp"
#include "../random.hpp"
#include "../ray.hpp"
#include "../texture.hpp"
#include "aabb.hpp"

namespace yk {

template <class T>
struct constant_medium {
  std::unique_ptr<hittable<T>> boundary;
  material<T> phase_function;
  T neg_inv_density;

  constexpr constant_medium() = default;

  template <class H>
  constexpr constant_medium(H&& h, T d, texture<T> tex) noexcept
      : constant_medium(std::make_unique<hittable<T>>(std::forward<H>(h)), d,
                        std::move(tex)) {}

  constexpr constant_medium(std::unique_ptr<hittable<T>>&& b, T d,
                            texture<T> tex) noexcept
      : boundary(std::move(b)),
        neg_inv_density(-1 / d),
        phase_function{isotropic<T>{tex}} {}

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max, hit_record<T>& rec,
                     Gen& gen) const noexcept {
    hit_record<T> rec1, rec2;

    constexpr auto inf = std::numeric_limits<T>::infinity();

    if (!custom::hit(*boundary, r, -inf, inf, rec1, gen)) return false;

    if (!custom::hit(*boundary, r, rec1.t + 0.0001, inf, rec2, gen))
      return false;

    rec1.t = std::max(rec1.t, t_min);
    rec2.t = std::min(rec2.t, t_max);

    if (rec1.t >= rec2.t) return false;

    rec1.t = std::max<T>(rec1.t, 0);

    const auto ray_length = r.direction.length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance =
        neg_inv_density * math::log(uniform_real_distribution<T>(0, 1)(gen));

    if (hit_distance > distance_inside_boundary) return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.pos = r.at(rec.t);

    rec.normal = {1, 0, 0};  // arbitrary
    rec.front_face = true;   // also arbitrary
    rec.mat = phase_function;

    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    return custom::bounding_box(*boundary, time0, time1, output_box);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_CONSTANT_MEDIUM_HPP

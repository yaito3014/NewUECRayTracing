#pragma once

#ifndef YK_RAYTRACING_ROTATE_HPP
#define YK_RAYTRACING_ROTATE_HPP

#include <algorithm>
#include <limits>
#include <memory>

#include "aabb.hpp"
#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../hittable.hpp"
#include "../math.hpp"
#include "../pos3.hpp"
#include "../ray.hpp"

namespace yk {

template <class T>
struct rotate_y {
  std::unique_ptr<hittable<T>> base;
  T sin_theta;
  T cos_theta;
  bool has_box;
  aabb<T> bbox;

  template <class H>
  constexpr rotate_y(H&& h, T angle)
      : rotate_y(std::make_unique<hittable<T>>(std::forward<H>(h)), angle) {}

  constexpr rotate_y(std::unique_ptr<hittable<T>> p, T angle) noexcept
      : base(std::move(p)) {
    auto radians = angle * math::numbers::pi / 180;
    sin_theta = math::sin(radians);
    cos_theta = math::cos(radians);
    has_box = custom::bounding_box<T>(*base, 0, 1, bbox);
    constexpr T inf = std::numeric_limits<T>::infinity();
    pos3<T> min{inf, inf, inf};
    pos3<T> max{-inf, -inf, -inf};
    for (const auto& x : {bbox.minimum.x, bbox.maximum.x}) {
      for (const auto& y : {bbox.minimum.y, bbox.maximum.y}) {
        for (const auto& z : {bbox.minimum.z, bbox.maximum.z}) {
          auto newx = cos_theta * x + sin_theta * z;
          auto newz = -sin_theta * x + cos_theta * x;
          pos3<T> tester{newx, y, newz};
          for (const auto& a : {&pos3<T>::x, &pos3<T>::y, &pos3<T>::z}) {
            min.*a = std::min(min.*a, tester.*a);
            max.*a = std::max(max.*a, tester.*a);
          }
        }
      }
    }
    bbox = {min, max};
  }

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec,Gen& gen) const noexcept {
    auto origin = r.origin;
    auto direction = r.direction;

    origin.x = cos_theta * r.origin.x - sin_theta * r.origin.z;
    origin.z = sin_theta * r.origin.x + cos_theta * r.origin.z;

    direction.x = cos_theta * r.direction.x - sin_theta * r.direction.z;
    direction.z = sin_theta * r.direction.x + cos_theta * r.direction.z;

    ray<T> rotated_ray{origin, direction, r.time};
    if (!custom::hit(*base, rotated_ray, t_min, t_max, rec, gen)) return false;

    auto pos = rec.pos;
    auto normal = rec.normal;

    pos.x = cos_theta * rec.pos.x + sin_theta * rec.pos.z;
    pos.z = -sin_theta * rec.pos.x + cos_theta * rec.pos.z;

    normal.x = cos_theta * rec.normal.x + sin_theta * rec.normal.z;
    normal.z = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;

    rec.pos = pos;
    rec.set_face_normal(rotated_ray, normal);

    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = bbox;
    return has_box;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_ROTATE_HPP

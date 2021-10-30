#pragma once

#ifndef YK_RAYTRACING_TRANSLATE_HPP
#define YK_RAYTRACING_TRANSLATE_HPP

#include <memory>
#include <utility>

#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../hittable.hpp"
#include "../ray.hpp"
#include "../vec3.hpp"
#include "aabb.hpp"

namespace yk {

template <class T>
struct translate {
  std::unique_ptr<hittable<T>> base;
  vec3<T> offset;

  template <class H>
  constexpr translate(H&& h, const vec3<T>& offset)
      : translate(std::make_unique<hittable<T>>(std::forward<H>(h)), offset) {}

  constexpr translate(std::unique_ptr<hittable<T>>&& p, const vec3<T>& offset)
      : base(std::move(p)), offset(offset) {}

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max, hit_record<T>& rec,
                     Gen& gen) const noexcept {
    ray<T> moved_ray{r.origin - offset, r.direction, r.time};
    if (!custom::hit(*base, moved_ray, t_min, t_max, rec, gen)) return false;
    rec.pos += offset;
    rec.set_face_normal(moved_ray, rec.normal);
    return true;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    if (!custom::bounding_box(*base, time0, time1, output_box)) return false;
    output_box = {
        output_box.minimum + offset,
        output_box.maximum + offset,
    };
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_TRANSLATE_HPP

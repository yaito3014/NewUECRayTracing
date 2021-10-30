#pragma once

#ifndef YK_RAYTRACING_BOX_HPP
#define YK_RAYTRACING_BOX_HPP

#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../material.hpp"
#include "../pos3.hpp"
#include "../ray.hpp"
#include "aarect.hpp"
#include "hittable_list.hpp"

namespace yk {

template <class T>
struct box {
  pos3<T> box_min;
  pos3<T> box_max;
  hittable_list<T> sides;

  // constexpr box() noexcept = default;
  constexpr box(pos3<T> min, pos3<T> max, material<T> mat) noexcept
      : box_min(min), box_max(max) {
    sides.add(xy_rect<T>{min.x, max.x, min.y, max.y, min.z, mat});
    sides.add(xy_rect<T>{min.x, max.x, min.y, max.y, max.z, mat});

    sides.add(xz_rect<T>{min.x, max.x, min.z, max.z, min.y, mat});
    sides.add(xz_rect<T>{min.x, max.x, min.z, max.z, max.y, mat});

    sides.add(yz_rect<T>{min.y, max.y, min.z, max.z, min.x, mat});
    sides.add(yz_rect<T>{min.y, max.y, min.z, max.z, max.x, mat});
  }

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec, Gen& gen) const noexcept {
    return sides.hit(r, t_min, t_max, rec, gen);
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = {box_min, box_max};
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_BOX_HPP

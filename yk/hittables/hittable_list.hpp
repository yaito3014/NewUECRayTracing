#pragma once

#ifndef YK_RAYTRACING_HITTABLE_LIST_HPP
#define YK_RAYTRACING_HITTABLE_LIST_HPP

#include <memory>
#include <utility>
#include <vector>

#include "../custom.hpp"
#include "../hit_record.hpp"
#include "../hittable.hpp"

namespace yk {

template <class T>
struct hittable_list {
  std::vector<std::unique_ptr<hittable<T>>> objects;

  template <class H>
  constexpr void add(H&& h) noexcept {
    add(std::make_unique<hittable<T>>(std::move(h)));
  }

  constexpr void add(std::unique_ptr<hittable<T>>&& h) noexcept {
    objects.emplace_back(std::move(h));
  }

  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec) const noexcept {
    hit_record<T> temp_rec{};
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
      if (yk::custom::hit(*object, r, t_min, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hit_anything;
  }

  constexpr bool bounding_box(T time0, T time1,
                             aabb<T>& output_box) const noexcept {
    if (objects.empty()) return false;

    aabb<T> temp_box;
    bool first_box = true;
    for (const auto& object : objects) {
      if (!yk::custom::bounding_box(*object, time0, time1, temp_box)) return false;
      output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
      first_box = false;
    }
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_HITTABLE_LIST_HPP

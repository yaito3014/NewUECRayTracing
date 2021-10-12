#pragma once

#ifndef YK_RAYTRACING_HITTABLE_LIST_HPP
#define YK_RAYTRACING_HITTABLE_LIST_HPP

#include <vector>

#include "../hit.hpp"
#include "../hit_record.hpp"
#include "../hittable.hpp"

namespace yk {

template <class T>
struct hittable_list {
  std::vector<hittable<T>> objects;

  constexpr void add(hittable<T> h) noexcept { objects.push_back(h); }

  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec) const noexcept {
    hit_record<T> temp_rec{};
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
      if (yk::hit<T>(object, r, t_min, closest_so_far, temp_rec)) {
        hit_anything = true;
        closest_so_far = temp_rec.t;
        rec = temp_rec;
      }
    }

    return hit_anything;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_HITTABLE_LIST_HPP

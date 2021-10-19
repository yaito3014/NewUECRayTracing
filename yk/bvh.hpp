#pragma once

#ifndef YK_RAYTRACING_BVH_HPP
#define YK_RAYTRACING_BVH_HPP

#include <cstddef>
#include <vector>

#include "aabb.hpp"
#include "hit_record.hpp"
#include "hittable.hpp"
#include "hittables/hittable_list.hpp"

namespace yk {

template <class T>
struct bvh_node {
  hittable<T> left;
  hittable<T> right;
  aabb<T> box;

  constexpr bvh_node(const hittable_list<T>& list, T time0, T time1)
      : bvh_node(list.objects, 0, list.objects.size()) {}

  template <class Iter>
  constexpr bvh_node(Iter first, Iter last, T time0, T time1) {
    auto comp = [](const auto& a, const auto& b) {
      return std::visit(
          [](const auto& a, const auto& b) {
            return yk::bounding_box(a).minimum.x <
                   yk::bounding_box(b).minimum.x;
          },
          a, b);
    };

    auto span = std::distance(first, last);

    if (span == 1)
      left = objects[start];
    else if (span == 2)
      std::tie(left, right) = std::minmax(*first, *(first + 1), comp);
    else {
      auto mid = first + span / 2;
      std::vector<hittable<T>> objects(first, last);
      std::sort(objects.begin(), objects.end(), comp);
      left = bvh_node<T>(objects.begin(), mid, time0, time1);
      right = bvh_node<T>(mid, objects.end(), time0, time1);
    }
  }

  constexpr bool hit(const ray<T>& r, T t_min, T t_max,
                     hit_record<T>& rec) const noexcept {
    if (box.hit(r, t_min, t_max)) return false;
    bool hit_left = yk::hit(left, r, t_min, t_max, rec);
    bool hit_right = yk::hit(right, r, t_min, t_max, rec);
    return hit_left || hit_right;
  }

  constexpr bool bounding_box(T time0, T time1,
                              aabb<T>& output_box) const noexcept {
    output_box = box;
    return true;
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_BVH_HPP

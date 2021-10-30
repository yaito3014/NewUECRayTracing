#pragma once

#ifndef YK_RAYTRACING_BVH_HPP
#define YK_RAYTRACING_BVH_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include "aabb.hpp"
#include "../hit_record.hpp"
#include "../hittable.hpp"
#include "hittable_list.hpp"

namespace yk {

template <class T>
struct bvh_node {
  std::unique_ptr<hittable<T>> left;
  std::unique_ptr<hittable<T>> right;
  aabb<T> box;

  template <class Gen>
  constexpr bvh_node(hittable_list<T>&& list, T time0, T time1, Gen& gen)
      : bvh_node(std::make_move_iterator(list.objects.begin()),
                 std::make_move_iterator(list.objects.end()), time0, time1,
                 gen) {}

  template <class Iter, class Gen>
  constexpr bvh_node(Iter first, Iter last, T time0, T time1, Gen& gen) {
    auto axis = std::array{&pos3<T>::x, &pos3<T>::y,
                           &pos3<T>::z}[uniform_int_distribution<>{0, 2}(gen)];
    auto comp = [&](const std::unique_ptr<hittable<T>>& a,
                    const std::unique_ptr<hittable<T>>& b) {
      aabb<T> box_a{};
      aabb<T> box_b{};
      if (!(a && custom::bounding_box(*a, time0, time1, box_a)) ||
          !(b && custom::bounding_box(*b, time0, time1, box_b)))
        std::cerr << "No bounding box in bvh_node constructor.(comp)\n";
      return std::invoke(axis, box_a.minimum) <
             std::invoke(axis, box_b.minimum);
    };

    auto span = std::distance(first, last);

    if (span == 1)
      left = std::move(*first);
    else if (span == 2) {
      left = *first;
      right = *(first + 1);
      if (!comp(left, right)) std::swap(left, right);
    } else {
      std::vector<std::unique_ptr<hittable<T>>> objects(first, last);
      std::sort(objects.begin(), objects.end(), comp);
      auto mid = std::make_move_iterator(objects.begin() + span / 2);
      left = std::make_unique<hittable<T>>(bvh_node<T>(
          std::make_move_iterator(objects.begin()), mid, time0, time1, gen));
      right = std::make_unique<hittable<T>>(bvh_node<T>(
          mid, std::make_move_iterator(objects.end()), time0, time1, gen));
    }

    aabb<T> box_left{};
    if (left && custom::bounding_box(*left, time0, time1, box_left)) {
      aabb<T> box_right{};
      if (right) {
        if (custom::bounding_box(*right, time0, time1, box_right))
          box = surrounding_box(box_left, box_right);
        else
          std::cerr << "No bounding box in bvh_node constructor.\n";
      } else
        box = box_left;
    } else
      std::cerr << "No bounding box in bvh_node constructor.\n";
  }

  template <class Gen>
  constexpr bool hit(const ray<T>& r, T t_min, T t_max, hit_record<T>& rec,
                     Gen& gen) const noexcept {
    if (!box.hit(r, t_min, t_max, gen)) return false;
    bool hit_left = left && custom::hit(*left, r, t_min, t_max, rec, gen);
    bool hit_right = right && custom::hit(*right, r, t_min,
                                          hit_left ? rec.t : t_max, rec, gen);
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

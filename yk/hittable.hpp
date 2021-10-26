#pragma once

#ifndef YK_RAYTRACING_HITTABLE_HPP
#define YK_RAYTRACING_HITTABLE_HPP

#include <variant>

#include "material.hpp"
#include "ray.hpp"

namespace yk {

template <class T>
struct sphere;

template <class T>
struct hittable_list;

template <class T>
struct moving_sphere;

template <class T>
struct bvh_node;

template <class T>
struct xy_rect;

template <class T>
struct xz_rect;

template <class T>
struct yz_rect;

template <class T>
using hittable = std::variant<sphere<T>, hittable_list<T>, moving_sphere<T>,
                              bvh_node<T>, xy_rect<T>, xz_rect<T>, yz_rect<T>>;

}  // namespace yk

#endif  // !YK_RAYTRACING_HITTABLE_HPP

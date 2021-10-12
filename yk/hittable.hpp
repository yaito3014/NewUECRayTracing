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
using hittable = std::variant<sphere<T>, hittable_list<T>>;

}  // namespace yk

#endif  // !YK_RAYTRACING_HITTABLE_HPP
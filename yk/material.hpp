#pragma once

#ifndef YK_RAYTRACING_MATERIAL_HPP
#define YK_RAYTRACING_MATERIAL_HPP

#include <variant>

#include "color.hpp"
#include "ray.hpp"

namespace yk {

 template <class T>
 struct lambertian;

template <class T>
struct metal;

template <class T>
using material = std::variant<lambertian<T>, metal<T>>;

}  // namespace yk

#endif  // !YK_RAYTRACING_MATERIAL_HPP

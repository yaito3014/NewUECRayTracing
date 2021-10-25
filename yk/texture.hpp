#pragma once

#ifndef YK_RAYTRACING_TEXTURE_HPP
#define YK_RAYTRACING_TEXTURE_HPP

#include <variant>

namespace yk {

template <class T>
struct solid_texture;

template <class T>
struct checker_texture;

template <class T>
struct noise_texture;

template <class T>
struct image_texture;

template <class T>
using texture = std::variant<solid_texture<T>, checker_texture<T>,
                             noise_texture<T>, image_texture<T>>;

}  // namespace yk

#endif  // !YK_RAYTRACING_TEXTURE_HPP

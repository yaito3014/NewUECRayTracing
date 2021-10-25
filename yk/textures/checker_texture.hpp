#pragma once

#ifndef YK_RAYTRACING_CHECKER_TEXTURE_HPP
#define YK_RAYTRACING_CHECKER_TEXTURE_HPP

#include <memory>

#include "../color.hpp"
#include "../texture.hpp"

namespace yk {

template <class T>
struct checker_texture {
  std::unique_ptr<texture<T>> even;
  std::unique_ptr<texture<T>> odd;

  template <class T1, class T2>
  constexpr checker_texture(T1&& ev, T2&& od)
      : even(std::make_unique<texture<T>>(std::forward<T1>(ev))),
        odd(std::make_unique<texture<T>>(std::forward<T2>(od))) {}

  constexpr checker_texture() noexcept = default;
  constexpr checker_texture(const checker_texture& other) noexcept
      : even(std::make_unique<texture<T>>(*other.even)),
        odd(std::make_unique<texture<T>>(*other.odd)) {}
  constexpr checker_texture(checker_texture&&) noexcept = default;
  constexpr checker_texture& operator=(const checker_texture& other) noexcept {
    even = std::make_unique<texture<T>>(*other.even);
    odd = std::make_unique<texture<T>>(*other.odd);
    return *this;
  }
  constexpr checker_texture& operator=(checker_texture&&) noexcept = default;

  constexpr color<T> value(T u, T v, const pos3<T>& p) const {
    auto sines =
        math::sin(10 * p.x) * math::sin(10 * p.y) * math::sin(10 * p.z);
    return custom::value(sines < 0 ? *odd : *even, u, v, p);
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_CHECKER_TEXTURE_HPP

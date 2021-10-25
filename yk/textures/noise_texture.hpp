#pragma once

#ifndef YK_RAYTRACING_NOISE_TEXTURE_HPP
#define YK_RAYTRACING_NOISE_TEXTURE_HPP

#include <numeric>
#include <utility>

#include "../color.hpp"
#include "../math.hpp"
#include "../pos3.hpp"
#include "../random.hpp"
#include "../vec3.hpp"

namespace yk {

template <class T>
struct perlin {
  static constexpr auto point_count = 256;
  std::array<vec3<T>, point_count> ranvec;
  vec3<std::array<int, point_count>> perm;

  template <class Gen>
  constexpr perlin(Gen& gen) noexcept {
    std::generate(ranvec.begin(), ranvec.end(),
                  [&]() { return vec3<T>::random(-1, 1, gen); });
    auto perlin_generate_perm =
        [&](std::array<int, point_count>& arr) noexcept {
          std::iota(arr.begin(), arr.end(), 0);
          for (int i = point_count; i-- > 0;)
            std::swap(arr[i], arr[uniform_int_distribution<int>(0, i)(gen)]);
        };
    perlin_generate_perm(perm.x);
    perlin_generate_perm(perm.y);
    perlin_generate_perm(perm.z);
  }
  T noise(const vec3<T>& p) const noexcept {
    auto u = p.x - math::floor(p.x);
    auto v = p.y - math::floor(p.y);
    auto w = p.z - math::floor(p.z);

    auto i = static_cast<int>(math::floor(p.x));
    auto j = static_cast<int>(math::floor(p.y));
    auto k = static_cast<int>(math::floor(p.z));
    vec3<T> c[2][2][2];

    for (int di = 0; di < 2; ++di)
      for (int dj = 0; dj < 2; ++dj)
        for (int dk = 0; dk < 2; ++dk)
          c[di][dj][dk] =
              ranvec[perm.x[(i + di) & 255] ^ perm.y[(j + dj) & 255] ^
                     perm.z[(k + dk) & 255]];

    return [](vec3<T> c[2][2][2], T u, T v, T w) noexcept {
      auto uu = u * u * (3 - 2 * u);
      auto vv = v * v * (3 - 2 * v);
      auto ww = w * w * (3 - 2 * w);
      T accum = 0;
      for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
          for (int k = 0; k < 2; ++k)
            accum += (i * uu + (1 - i) * (1 - uu)) *
                     (j * vv + (1 - j) * (1 - vv)) *
                     (k * ww + (1 - k) * (1 - ww)) *
                     dot(c[i][j][k], vec3<T>{u - i, v - j, w - k});
      return accum;
    }(c, u, v, w);
  }

  constexpr T turb(const vec3<T>& p, int depth = 7) const noexcept {
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
      accum += weight * noise(temp_p);
      weight *= 0.5;
      temp_p *= 2;
    }

    return math::abs(accum);
  }
};

template <class T>
struct noise_texture {
  perlin<T> noise;
  T scale;

  constexpr color<T> value(T u, T v, const pos3<T>& p) const noexcept {
    vec3<T> p2{p.x, p.y, p.z};
    return color<T>{1, 1, 1} * 0.5 *
           (1 + math::sin(scale * p2.z + 10 * noise.turb(p2)));
  }
};

}  // namespace yk

#endif  // !YK_RAYTRACING_NOISE_TEXTURE_HPP

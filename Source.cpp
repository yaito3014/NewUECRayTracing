#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "thirdparty/stb_image_write.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <utility>

#include "yk/bvh.hpp"
#include "yk/camera.hpp"
#include "yk/color.hpp"
#include "yk/custom.hpp"
#include "yk/hittables/aarect.hpp"
#include "yk/hittables/hittable_list.hpp"
#include "yk/hittables/moving_sphere.hpp"
#include "yk/hittables/sphere.hpp"
#include "yk/materials/dielectric.hpp"
#include "yk/materials/diffuse_light.hpp"
#include "yk/materials/lambertian.hpp"
#include "yk/materials/metal.hpp"
#include "yk/random.hpp"
#include "yk/textures/checker_texture.hpp"
#include "yk/textures/image_texture.hpp"
#include "yk/textures/noise_texture.hpp"
#include "yk/textures/solid_texture.hpp"

template <class T, class Gen>
constexpr yk::color<T> ray_color(const yk::ray<T>& r,
                                 const yk::color<T>& background,
                                 const yk::hittable<T>& world,
                                 unsigned int depth, Gen& gen,
                                 yk::color<T> a = {1, 1, 1},
                                 yk::color<T> b = {0, 0, 0}) noexcept {
  if (depth == 0) return {0, 0, 0};
  yk::hit_record<T> rec{};
  if (!yk::custom::hit(world, r, 0.001, std::numeric_limits<T>::infinity(),
                       rec))
    return a * background + b;

  yk::ray<T> scattered;
  yk::color<T> attenuation;
  yk::color<T> emitted = yk::custom::emitted(rec.mat, rec.u, rec.v, rec.pos);

  if (!yk::custom::scatter(rec.mat, r, rec, attenuation, scattered, gen))
    return a * emitted + b;

  return ray_color(scattered, background, world, depth - 1, gen,
                   a * attenuation, attenuation * b + emitted);
}

template <class T>
constexpr yk::color<std::uint8_t> into(const yk::color<T>& c) {
  return {
      static_cast<std::uint8_t>(256 *
                                std::clamp<T>(yk::math::sqrt(c.r), 0, 0.999)),
      static_cast<std::uint8_t>(256 *
                                std::clamp<T>(yk::math::sqrt(c.g), 0, 0.999)),
      static_cast<std::uint8_t>(256 *
                                std::clamp<T>(yk::math::sqrt(c.b), 0, 0.999)),
  };
}

template <class T, class Gen>
constexpr yk::hittable<T> random_scene(Gen& gen) noexcept {
  yk::hittable_list<T> world;

  auto checker = yk::checker_texture<T>(yk::solid_texture<T>{{0.2, 0.3, 0.1}},
                                        yk::solid_texture<T>{{0.9, 0.9, 0.9}});
  world.add(yk::sphere<T>{{0, -1000, 0}, 1000, yk::lambertian<T>{checker}});

  yk::uniform_real_distribution<T> dist(0, 1);

  for (int a = -11; a < 11; ++a) {
    for (int b = -11; b < 11; ++b) {
      auto choose_mat = dist(gen);
      yk::pos3<T> center{a + 0.8 * dist(gen), 0.2, b + 0.8 * dist(gen)};
      if ((center - yk::pos3<T>{4, 0.2, 0}).length() <= 0.9) continue;

      if (choose_mat < 0.8) {
        // diffuse
        auto albedo = yk::color<T>::random(gen) * yk::color<T>::random(gen);
        auto center2 = center + yk::vec3<T>{0, dist(gen) / 2, 0};
        world.add(yk::moving_sphere<T>{
            center, center2, 0, 1, 0.2,
            yk::lambertian<T>{yk::solid_texture<T>{albedo}}});
      } else if (choose_mat < 0.95) {
        // metal
        auto albedo = yk::color<T>::random(0.5, 1, gen);
        auto fuzz = dist(gen) / 2;
        world.add(yk::sphere<T>{center, 0.2, yk::metal<T>{albedo, fuzz}});
      } else {
        // glass
        world.add(yk::sphere<T>{center, 0.2, yk::dielectric<T>{1.5}});
      }
    }
  }

  world.add(yk::sphere<T>{{0, 1, 0}, 1.0, yk::dielectric<T>(1.5)});

  world.add(
      yk::sphere<T>{{-4, 1, 0},
                    1.0,
                    yk::lambertian<T>{yk::solid_texture<T>{{0.4, 0.2, 0.1}}}});

  world.add(yk::sphere<T>{{4, 1, 0}, 1.0, yk::metal<T>({0.7, 0.6, 0.5}, 0.0)});

  return yk::bvh_node<T>(std::move(world), 0, 1, gen);
}

template <class T>
constexpr auto two_spheres() noexcept {
  yk::hittable_list<T> objects;

  auto checker = yk::checker_texture<T>(yk::solid_texture<T>{{0.2, 0.3, 0.1}},
                                        yk::solid_texture<T>{{0.9, 0.9, 0.9}});

  objects.add(yk::sphere<T>{{0, -10, 0}, 10, yk::lambertian<T>{checker}});
  objects.add(yk::sphere<T>{{0, 10, 0}, 10, yk::lambertian<T>{checker}});

  return objects;
}

template <class T, class Gen>
constexpr auto two_perlin_spheres(Gen& gen) noexcept {
  yk::hittable_list<T> objects;

  auto pertext = yk::noise_texture<T>{{gen}, 4};
  objects.add(yk::sphere<T>{{0, -1000, 0}, 1000, yk::lambertian<T>{pertext}});
  objects.add(yk::sphere<T>{{0, 2, 0}, 2, yk::lambertian<T>{pertext}});

  return objects;
}

template <class T>
auto earth() {
  auto earth_texture = yk::image_texture<T>("earthmap.jpg");
  auto earth_surface = yk::lambertian<T>{earth_texture};
  auto globe = yk::sphere<T>{{0, 0, 0}, 2, earth_surface};
  return globe;
}

template <class T, class Gen>
constexpr auto simple_light(Gen& gen) {
  yk::hittable_list<T> objects;

  auto pertext = yk::noise_texture<T>{{gen}, 4};
  objects.add(yk::sphere<T>{{0, -1000, 0}, 1000, yk::lambertian<T>{pertext}});
  objects.add(yk::sphere<T>{{0, 2, 0}, 2, yk::lambertian<T>{pertext}});

  auto difflight = yk::diffuse_light<T>{yk::solid_texture<T>{{4, 4, 4}}};
  objects.add(yk::xy_rect<T>{3, 5, 1, 3, -2, difflight});

  return objects;
}

template <class T>
constexpr auto cornell_box() {
  yk::hittable_list<T> objects;

  auto red = yk::lambertian<T>{yk::solid_texture<T>{{.65, .05, .05}}};
  auto white = yk::lambertian<T>{yk::solid_texture<T>{{.73, .73, .73}}};
  auto green = yk::lambertian<T>{yk::solid_texture<T>{{.12, .45, .15}}};
  auto light = yk::diffuse_light<T>{yk::solid_texture<T>{{15, 15, 15}}};

  objects.add(yk::yz_rect<T>{0, 555, 0, 555, 555, green});
  objects.add(yk::yz_rect<T>{0, 555, 0, 555, 0, red});
  objects.add(yk::xz_rect<T>{213, 343, 227, 332, 554, light});
  objects.add(yk::xz_rect<T>{0, 555, 0, 555, 0, white});
  objects.add(yk::xz_rect<T>{0, 555, 0, 555, 555, white});
  objects.add(yk::xy_rect<T>{0, 555, 0, 555, 555, white});

  return objects;
}

template <class T>
constexpr auto render() noexcept {
  auto R = yk::math::cos(yk::math::numbers::pi / 4);

  yk::mt19937 mt(std::random_device{}());
  yk::uniform_real_distribution<T> dist(0, 1);

  yk::hittable<T> world;
  yk::pos3<T> lookfrom;
  yk::pos3<T> lookat;
  yk::color<T> background{0, 0, 0};
  auto vfov = 40.0;
  auto aperture = 0.0;

  switch (0) {
    case 1:
      world = random_scene<T>(mt);
      background = {0.7, 0.8, 1.0};
      lookfrom = {13, 2, 3};
      lookat = {0, 0, 0};
      vfov = 20.0;
      aperture = 0.1;
      break;

    case 2:
      world = two_spheres<T>();
      background = {0.7, 0.8, 1.0};
      lookfrom = {13, 2, 3};
      lookat = {0, 0, 0};
      vfov = 20.0;
      break;

    case 3:
      world = two_perlin_spheres<T>(mt);
      background = {0.7, 0.8, 1.0};
      lookfrom = {13, 2, 3};
      lookat = {0, 0, 0};
      vfov = 20.0;
      break;

    case 4:
      world = earth<T>();
      background = {0.7, 0.8, 1.0};
      lookfrom = {13, 2, 3};
      lookat = {0, 0, 0};
      vfov = 20.0;
      break;

    default:
    case 5:
      world = simple_light<T>(mt);
      yk::samples_per_pixel = 400;
      background = {0, 0, 0};
      lookfrom = {26, 3, 6};
      lookat = {0, 2, 0};
      vfov = 20.0;
      break;

    case 6:
      world = cornell_box<T>();
      yk::aspect_ratio = 1.0;
      yk::image_width = 600;
      yk::samples_per_pixel = 200;
      background = {0, 0, 0};
      lookfrom = {278, 278, -800};
      lookat = {278, 278, 0};
      vfov = 40.0;
      break;
  }

  yk::image_height = std::size_t(yk::image_width / yk::aspect_ratio);

  std::vector<yk::color<std::uint8_t>> img(yk::image_width * yk::image_height);

  auto dist_to_focus = 10.0;
  yk::vec3<T> vup{0, 1, 0};
  yk::camera<T> cam(lookfrom, lookat, vup, vfov, yk::aspect_ratio, aperture,
                    dist_to_focus, 0, 1);

  std::vector<std::pair<int, int>> pixels;
  pixels.reserve(yk::image_height * yk::image_width);

  for (int h = 0; h < yk::image_height; ++h)
    for (int w = 0; w < yk::image_width; ++w) pixels.emplace_back(h, w);

  std::for_each(std::execution::par_unseq, pixels.cbegin(), pixels.cend(),
                [&](const auto& t) {
                  const auto& [h, w] = t;
                  // if (w == 0) std::cout << "line : " << h << '\n';
                  std::vector<yk::ray<T>> rays(yk::samples_per_pixel);
                  std::generate(rays.begin(), rays.end(), [&, h = h, w = w]() {
                    auto u = T(w + dist(mt)) / yk::image_width;
                    auto v =
                        T(yk::image_height - h - dist(mt)) / yk::image_height;
                    return cam.get_ray(u, v, mt);
                  });
                  yk::color<T> pixel_color =
                      std::transform_reduce(
                          std::execution::par_unseq, rays.cbegin(), rays.cend(),
                          yk::color<T>{0, 0, 0}, std::plus<>{},
                          [&](const yk::ray<T>& r) {
                            return ray_color(r, background, world,
                                             yk::constants::max_depth, mt);
                          }) *
                      1.0 / yk::samples_per_pixel;
                  img[h * yk::image_width + w] = into(pixel_color);
                });

  return img;
}

int main() {
  auto img = render<double>();

  if (!stbi_write_png("image.png", yk::image_width, yk::image_height, 3,
                      img.data(), sizeof(img[0]) * yk::image_width))
    std::cerr << "error!" << std::endl;
}

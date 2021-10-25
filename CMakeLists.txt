# CMakeList.txt : NewUECRayTracing の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif()

project ("NewUECRayTracing")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STACK_SIZE 1000000000)


# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (NewUECRayTracing "Source.cpp"   "yk/vec3.hpp" "yk/math.hpp" "yk/pos3.hpp" "yk/color.hpp" "yk/ray.hpp" "yk/camera.hpp" "yk/hittable.hpp" "yk/hittables/sphere.hpp" "yk/hittables/hittable_list.hpp" "yk/config.hpp" "yk/material.hpp" "yk/materials/lambertian.hpp" "yk/random.hpp" "yk/materials/metal.hpp"   "yk/hit_record.hpp" "yk/materials/dielectric.hpp" "yk/hittables/moving_sphere.hpp" "yk/aabb.hpp" "yk/custom.hpp" "yk/bvh.hpp" "yk/texture.hpp" "yk/textures/solid_texture.hpp" "yk/textures/checker_texture.hpp" "yk/textures/noise_texture.hpp" "yk/textures/image_texture.hpp")

if (UNIX)
find_package(TBB REQUIRED)
target_link_libraries(NewUECRayTracing tbb)
endif (UNIX)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(NewUECRayTracing PRIVATE ${STB_INCLUDE_DIRS})

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。

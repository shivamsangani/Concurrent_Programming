cmake_minimum_required(VERSION 3.13)

project(hashsets
    VERSION 1.0
    DESCRIPTION "C++ Concurrency Examples"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto")

find_package(Threads REQUIRED)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL AppleClang)
  add_compile_options(-Werror -Wall -Wextra -pedantic -Weverything)
  add_compile_options(
          -Wno-c++98-compat
          -Wno-c++98-compat-pedantic
          -Wno-padded
          -Wno-weak-vtables
          -Wno-unused-function
  )
  set(USE_SANITIZER "" CACHE STRING
          "Use the clang sanitizer [asan|tsan]")
  if("${USE_SANITIZER}" STREQUAL "asan")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
    add_link_options(-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope)
  elseif("${USE_SANITIZER}" STREQUAL "tsan")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
  elseif(NOT "${USE_SANITIZER}" STREQUAL "")
    message(FATAL_ERROR "Unknown argument to USE_SANITIZER: ${USE_SANITIZER} - options are [asan|tsan]")
  endif()
else()
  message(FATAL_ERROR "The clang compiler is required")
endif()

function(add_example name)
  add_executable(${name} src/${name}.cc)
  target_link_libraries(${name} PRIVATE Threads::Threads)
endfunction()

add_example(playground)
add_example(1_hello_threads)
add_example(2_hello_threads_vector)
add_example(3_hello_threads_vector_id_chatty)
add_example(4_logger_class_mutex)
add_example(5_logger_bad_bounded_string_race)
add_example(6_logger_bad_bounded_string_deadlock)
add_example(7_logger_with_our_scoped_lock)
add_example(8_logger_with_std_scoped_lock)
add_example(9_data_race_and_compiler_optimization)
add_example(10_bounded_fifo)

# UNIVR - Graphics Programming - 2024 Edition

This repository contains the code for the Graphics Programming course at the University of Verona (UNIVR), 2024 edition, offered by the Master's Degree in Computer Science and Engineering and held by professor [Umberto Castellani](https://github.com/cstmrt42)

## How to build

```
git clone --recursive https://github.com/Sparkhand/graphics-programming-univr.git
cd graphics-programming-univr

mkdir build
cd build

cmake ..
cmake --build .
```

## Inspired by
- The directory tree is highly inspired by [LearnOpenGL repo](https://github.com/JoeyDeVries/LearnOpenGL) and [OpenGL tutorials](https://learnopengl.com/)
- The CMake configuration is inspired by [Glitter](https://github.com/Polytonic/Glitter), a "dead simple boilerplate for OpenGL"

## Homework

Each exercise/homework assigned by the professor has a corresponding folder in the repository:

- **Shaders**
  - [Exercise 1: *shader with uniform* {1.3.1.shaders_uniform}](src/1.3.1.shaders_uniform)
  - [Exercise 2: *shader interpolation* {1.3.2.shaders_interpolation}](src/1.3.2.shaders_interpolation)
  - [Exercise 3: *shader class* {1.3.3.shaders_class}](src/1.3.3.shaders_class)
  - [Homework 1: *shader class with uniform* {1.3.3.1.shaders_class_uniform}](src/1.3.3.1.shaders_class_uniform/)
  - [Homework 2: *upside-down triangle* {1.3.3.2.shaders_class_upside_down}](src/1.3.3.2.shaders_class_upside_down/)
  - [Homework 3: *move triangle along horizontal axis* {1.3.3.3.shaders_class_movement}](src/1.3.3.3.shaders_class_movement/)
  - [Homework 4: *color triangle with fragment position* {1.3.3.4.shaders_class_fragment_position}](src/1.3.3.4.shaders_class_fragment_position/)
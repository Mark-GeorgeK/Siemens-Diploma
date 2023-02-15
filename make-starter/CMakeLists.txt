cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CalculatorProject)

# Add the executable
add_executable(calc main.c add.c divide.c multiply.c subtract.c)


# Set the C++ standard to C++11
target_compile_features(calc PRIVATE cxx_std_11)
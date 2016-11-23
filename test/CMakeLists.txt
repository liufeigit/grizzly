cmake_minimum_required(VERSION 3.5.1)

project(grizzly-test)

add_definitions(-std=c++1z -Wall)
include_directories(/usr/local/include)

<<<<<<< HEAD
add_executable(grizzly-test main.cpp CircularBuffer.cpp AllPassFilter.cpp MidSide.cpp Normalize.cpp Ramp.cpp Waveform.cpp)
=======
add_executable(grizzly-test main.cpp CircularBuffer.cpp Convolution.cpp MidSide.cpp Normalize.cpp Ramp.cpp Waveform.cpp Window.cpp)
>>>>>>> 9767608debcc0a53b2e8e2a41f9031d74a046b80

cmake_minimum_required(VERSION 3.5.1)

project(grizzly-test)

add_definitions(-std=c++1z -Wall)
include_directories(/usr/local/include)

add_executable(grizzly-test main.cpp AllPassFilter.cpp CircularBuffer.cpp Convolution.cpp MidSide.cpp Normalize.cpp Ramp.cpp Waveform.cpp WaveShaper.cpp Window.cpp)

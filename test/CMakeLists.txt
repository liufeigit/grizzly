cmake_minimum_required(VERSION 3.5.1)

project(grizzly-test)

add_definitions(-std=c++1z -Wall)
include_directories(/usr/local/include)

add_executable(grizzly-test
    main.cpp
    AllPassFilter.cpp
    AnalogEnvelope.cpp
    CircularBuffer.cpp
    Convolution.cpp
    FastFourierTransform.cpp
    MidSide.cpp
    Normalize.cpp
    Ramp.cpp
    SpectralCentroid.cpp
    Spectrum.cpp
    Waveform.cpp
    WaveShaper.cpp
    Window.cpp)

find_library(Grizzly grizzly)
target_link_libraries(grizzly-test ${Grizzly})

if (APPLE)
	find_library(Accelerate Accelerate REQUIRED)
	target_link_libraries(grizzly-test ${Accelerate})
endif (APPLE)
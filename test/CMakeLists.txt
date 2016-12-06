cmake_minimum_required(VERSION 3.5.1)

project(grizzly-test)

add_definitions(-std=c++1z -Wall)
include_directories(/usr/local/include)

set(SOURCES
    main.cpp
    AllPassFilter.cpp
    AnalogOnePoleFilter.cpp
    AnalyticTransform.cpp
    Biquad.cpp
    CircularBuffer.cpp
    CombFilter.cpp
    Convolution.cpp
    Delay.cpp
    FastFourierTransformOoura.cpp
    FirstOrderFilter.cpp
    GordonSmithOscillator.cpp
    HilbertTransform.cpp
    HighFrequencyContent.cpp
    ImpulseResponse.cpp
    MidSide.cpp
    Ramp.cpp
    SegmentEnvelope.cpp
    SpectralCentroid.cpp
    Spectrum.cpp
    Waveform.cpp
    Window.cpp
    ZTransform.cpp)

add_executable(grizzly-test ${SOURCES})
target_sources(grizzly-test PRIVATE ${SOURCES})

find_library(Grizzly grizzly)
target_link_libraries(grizzly-test ${Grizzly})

if (APPLE)
    target_sources(grizzly-test PRIVATE FastFourierTransformAccelerate.cpp)
	find_library(Accelerate Accelerate REQUIRED)
	target_link_libraries(grizzly-test ${Accelerate})
endif (APPLE)

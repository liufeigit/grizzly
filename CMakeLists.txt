cmake_minimum_required(VERSION 3.5.1)

project(grizzly CXX)

add_definitions(-std=c++1z -Wall)
include_directories(/usr/local/include)

add_library(grizzly SHARED "")
install(TARGETS grizzly DESTINATION lib)

set(HEADERS
    AllPassFilter.hpp
	AnalogOnePoleFilter.hpp
	AnalyticTransform.hpp
	Biquad.hpp
	BiquadCoefficients.hpp
    CircularBuffer.hpp
    CombFilter.hpp
	Convolution.hpp
	Delay.hpp
	DownSample.hpp
	EnvelopeDetector.hpp
    FastFourierTransform.hpp
	FastFourierTransformBase.hpp
	FirstOrderCoefficients.hpp
	FirstOrderFilter.hpp
	FourierTransform.hpp
	GainComputer.hpp
	GordonSmithOscillator.hpp
	HaarWaveletTransform.hpp
	HilbertTransform.hpp
	HighFrequencyContent.hpp
	MidSide.hpp
    Ramp.hpp
    SegmentEnvelope.hpp
    SpectralCentroid.hpp
    Spectrum.hpp
    Transfer.hpp
	UpSample.hpp
    Waveform.hpp
	WaveShaper.hpp
	Window.hpp)

set(SOURCES
	FastFourierTransformBase.cpp)

target_sources(grizzly PRIVATE ${HEADERS} ${SOURCES})
source_group(\\ FILES ${HEADERS} ${SOURCES})
install (FILES ${HEADERS} DESTINATION include/grizzly)

# Ooura
set(OOURA_HEADERS
    Ooura/FastFourierTransformOoura.hpp
	Ooura/fftsg.h)

set(OOURA_SOURCES
	Ooura/FastFourierTransformOoura.cpp
	Ooura/fftsg.cpp)

target_sources(grizzly PRIVATE ${OOURA_HEADERS} ${OOURA_SOURCES})
source_group(\\Ooura FILES ${OOURA_HEADERS} ${OOURA_SOURCES})
install (FILES ${OOURA_HEADERS} DESTINATION include/grizzly/Ooura)

# Apple
if (APPLE)
    set(APPLE_HEADERS
        Apple/FastFourierTransformAccelerate.hpp)

	set(APPLE_SOURCES
		Apple/FastFourierTransformAccelerate.cpp)

    target_sources(grizzly PRIVATE ${APPLE_HEADERS} ${APPLE_SOURCES})
    source_group(\\Apple FILES ${APPLE_HEADERS} ${APPLE_SOURCES})
    install (FILES ${APPLE_HEADERS} DESTINATION include/grizzly/Apple)

	find_library(Accelerate Accelerate REQUIRED)
	target_link_libraries(grizzly ${Accelerate})
endif()

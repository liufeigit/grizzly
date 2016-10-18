cmake_minimum_required(VERSION 3.5.1)

project(grizzly)

add_definitions(-std=c++1z -Wall)
include_directories(/usr/local/include)

add_library(grizzly SHARED "")
install(TARGETS grizzly DESTINATION lib)

set(HEADERS
	AnalogEnvelope.hpp
	AnalogLadderFilter.hpp
	AnalogOnePoleFilter.hpp
    AnalogStateVariableFilter.hpp
	AnalyticTransform.hpp
	Biquad.hpp
	BiquadCoefficients.hpp
	Buffer.hpp
	Centroid.hpp
	Convolution.hpp
	Delay.hpp
	DownSample.hpp
	Envelope.hpp
	EnvelopeDetector.hpp
	FastFourierTransform.hpp
	Filter.hpp
	FirstOrderCoefficients.hpp
	FirstOrderFilter.hpp
	FourierTransform.hpp
	GainComputer.hpp
	GordonSmithOscillator.hpp
	HaarWaveletTransform.hpp
	HilbertTransform.hpp
	HighFrequencyContent.hpp
	InputStream.hpp
	MidSide.hpp MidSide.cpp
    Oscillator.hpp
    Parallel.hpp
    Ramp.hpp
    SpectralCentroid.hpp
    Transfer.hpp
	UpSample.hpp
	WaveShaper.hpp
	WeightedSum.hpp
	Window.hpp
	YinPitchDetection.hpp)

set(SOURCES
	AnalyticTransform.cpp
	Envelope.cpp
	FastFourierTransform.cpp
	FourierTransform.cpp
	HilbertTransform.cpp
	MidSide.cpp
    Parallel.cpp
	Window.cpp)

target_sources(grizzly PRIVATE ${HEADERS} ${SOURCES})
source_group(\\ FILES ${HEADERS} ${SOURCES})
install (FILES ${HEADERS} DESTINATION include/grizzly)

# Ooura
set(OOURA_HEADERS
	Ooura/fftsg.h)

set(OORUA_SOURCES
	Ooura/FastFourierTransformOoura.cpp
	Ooura/fftsg.cpp)

target_sources(grizzly PRIVATE ${OOURA_HEADERS} ${OOURA_SOURCES})
source_group(\\Ooura FILES ${OOURA_HEADERS} ${OOURA_SOURCES})
install (FILES ${OOURA_HEADERS} DESTINATION include/grizzly/Ooura)

# Apple
if (APPLE)
    set(APPLE_HEADERS
        Apple/Parallel.hpp)

	set(APPLE_SOURCES
		Apple/FastFourierTransformAccelerate.cpp)

    target_sources(grizzly PRIVATE ${APPLE_HEADERS} ${APPLE_SOURCES})
    source_group(\\Apple FILES ${APPLE_HEADERS} ${APPLE_SOURCES})
    install (FILES ${APPLE_HEADERS} DESTINATION include/grizzly/Apple)

	find_library(Accelerate Accelerate REQUIRED)
	target_link_libraries(grizzly ${Accelerate})
endif()

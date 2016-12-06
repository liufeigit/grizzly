# Grizzly

Grizzly is a digital signal processing library for modern C++. We wrote it with readability and modularity in mind, and it takes care of most of the common tasks in DSP:

 - Filters
 	- Biquad
 	- First-order filters
 	- Analog one-pole
 	- All-pass
 	- Comb
 - Transforms
 	- Fast Fourier transform and STFT
 	- Z-transform
 	- Hilbert transform
 	- Analytic transform
 	- Common windows
 - Delay lines
 - Convolution
 - Up- and down-sampling
 - Envelope generation and detection
 - Stereo to mid/side conversion
 - Dynamic range compression and expansion

***Grizzly is built with c++17. Make sure to add the -std=c++1z flag to your compiler!***

## Sample Code

```
// Construct a biquad
Biquad<float> biquad;
lowPass(biquad.coefficients, 44100_Hz, 1000Hz, 0.707);

// Insert a new value and read the output
biquad.write(0.975);
biquad.read();
```

```
// Create a fast fourier transform
FastFourierTransform fft(512);

// Forward analyze an input signal of 512 samples
Spectrum<float> spectrum = fft.forward(floatPtr);

// Loop over and output all magnitudes
for (auto& x : spectrum.magnitudes())
	cout << x << endl;
```

```
// Create an ADSR envelope
auto env = SegmentEnvelope<float>::adsr(0.5_sec, 1_sec, 0.2, 2_sec);

// Move the envelope forward by 1/10th of a second
env.increment(0.1_sec);

// Read the current value of the envelope
env.read();
```

## Installation

A [Homebrew](http://brew.sh) and [LinuxBrew](http://linuxbrew.sh) formula exists for macOS and Linux respectively.

```
brew tap dsperados/tap
brew install grizzly
```

Grizzly can also be built manually, by running the CMake script in the root.

```
mkdir build
cd build
cmake ..
make
sudo make install
```

This library is written in c++17. Make sure you have the **latest version** of your compiler (on macOS this would be **Xcode 7** or higher), and add the **-std=c++1z** flag to your compiler!

## Your Own Projects with Grizzly

Grizzly is built on top of C++17 and works with *clang*. If you would like to create your own projects with Grizzly, here's some pointers:

 - You'll need to add `/usr/local/include` to your header search paths
 - You'll need to add `/usr/local/lib` to your linker search paths
 - You'll need to add `-lgrizzly` as a linker flag
 - Unfortunately, IDE's such as Xcode do not yet have c++17 as one of the dialects of choice. You can easily enable c++17 by **adding the flag `-std=c++1z`** to `other C++ flags` in your Build Settings.

## License

Grizzly is licensed under GPLv3 and can be used for free in open-source, non-commercial applications. Commercial licensing is available upon request.

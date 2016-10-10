//
//  Window.cpp
//  libbear
//
//  Created by Stijn on 05/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#include <cmath>

#include <dsperados/math/utility.hpp>
#include "Window.hpp"

using namespace gsl;
using namespace math;
using namespace std;

namespace bear::dsp
{
    void fillTriangleWindow(span<float> window, double t)
    {
        const size_t l = window.size() * t;
        for (size_t i = 0; i < l; ++i)
            window[i] = i / double(l);

        const size_t r = window.size() - l;
        for (size_t i = 0; i < r; ++i)
            window[l + i] = 1 - (i / double(r));
    }

    vector<float> createTriangleWindow(size_t size, double t)
    {
        vector<float> window(size);
        fillTriangleWindow(window, t);

        return window;
    }

    vector<float> createHannWindow(size_t size)
    {
        vector<float> window(size, 0.0f);

        for (auto n = 0; n < size; ++n)
            window[n] = (1 - cos(TWO_PI<double> * n / size)) / 2;

        return window;
    }

    vector<float> createHammingWindow(size_t size)
    {
        vector<float> window(size, 0.0f);

        for (auto n = 0; n < size; ++n)
            window[n] = 0.54 - 0.46 * cos(TWO_PI<double> * n / size);

        return window;
    }

    vector<float> createBlackmanWindow(size_t size)
    {
        vector<float> window(size, 0.0f);
        
        for (auto n = 0; n < size; ++n)
            window[n] = 0.42 - 0.5 * cos(TWO_PI<double> * n / size) + 0.08 * cos(2 * TWO_PI<double> * n / size);

        return window;
    }

    // Sincfunction with a window (Nu met hanning, wellicht enkel sinc doen, geen extra window)
    vector<float> createSincWindow(size_t size, float normalizedCutOff, float gain)
    {
        // Create the hanning window
        auto hanningWindow = createHannWindow(size);

        // halfSize must be signed
        float halfSize = size / 2.0;
        double sum = 0;

        // Fill the sinc window
        vector<float> sinc(size);
        for (auto i = 0; i < size; ++i)
        {
            if (i == halfSize)
                sinc[i] = PI<double> * normalizedCutOff * hanningWindow[i];
            else
                sinc[i] = sin(PI<double> * normalizedCutOff * (i - halfSize) ) / (i - halfSize) * hanningWindow[i];

            sum += sinc[i];
        }

        // Normalize the window
        for (auto& value : sinc)
            value = (value / sum) * gain;

        return sinc;
    }

    vector<float> createSymmetricSincWindow(const size_t size, const double normalizedFrequency)
    {
        double halfSize = (size - 1) / 2.0;
        double delayIndex;

        // Output
        vector<float> sinc(size);

        // Generate a sinc function shifted to halfSize (i.e. delay by halfSize)
        for (auto i = 0; i < size; i++)
        {
            if (i ==  halfSize)
            {
                sinc[i] = 2.0 * normalizedFrequency;
            } else {
                delayIndex = static_cast<double>(i) - halfSize;
                sinc[i] = sin(TWO_PI<double> * normalizedFrequency * delayIndex) / (PI<double> * delayIndex);
            }
        }

        return sinc;
    }

    vector<float> createSymmetricHannWindow(const size_t size)
    {
        vector<float> window(size);
        double M = size - 1;

        for (auto i = 0; i < size; i++)
        {
            window[i] = 0.5 * (1.0 - cos(TWO_PI<double> * static_cast<double>(i) / M));
        }

        return window;
    }

    double besselizero(double x)
    {
        double temp;
        double sum = 1.0;
        double u = 1.0;
        double halfx = x / 2.0;
        size_t n = 1;

        do
        {
            temp = halfx / static_cast<double>(n);
            u *= temp * temp;
            sum += u;
            n++;
        }
        while (u >= 1e-21 * sum);

        return(sum);
    }

    vector<float> createSymmetricKaiserWindow(size_t size, double betaFactor)
    {
        vector<float> window(size);
        double tmp;
        double k1 = 1.0 / besselizero(betaFactor);
        size_t k2 = 1 - (size & 1);
        size_t halfSize = (size + 1) >> 1;

        // Calculate window coefficients
        for (auto i = 0; i < halfSize; ++i)
        {
            tmp = static_cast<double>(2 * i + k2) / (static_cast<double>(size) - 1.0);
            window[halfSize - (1 & (!k2)) + i] = window[halfSize - 1 - i] = k1 * besselizero(betaFactor * sqrt(1.0 - tmp * tmp));
        }

        return window;
    }

}


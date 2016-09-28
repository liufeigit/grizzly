//
//  YinPitchDetection.hpp
//  libbear
//
//  Created by Milan van der Meer on 19/11/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef YinPitchDetection_hpp
#define YinPitchDetection_hpp

#include <math.h>
#include <stdio.h>
#include <vector>

template <class T>
class YinPitchDetection
{
    
    using audioFrame = std::vector<T>;
    
public:
    
    YinPitchDetection(audioFrame audioFrame, float sampleRate);
    YinPitchDetection(std::vector<audioFrame> vectorWithAudioFrames, float sampleRate);
    
    void doYin();
    
    T&              getOneFramePitch(unsigned int frameIndex)   {return outputBuffer[frameIndex];}
    std::vector<T>& getAllFramePitches()                        {return outputBuffer;}
    
private:
    std::vector<std::vector<T>> inputBuffer;
    std::vector<T> outputBuffer;
    
    float probability = 0;
    float threshold = 0.15;
    float sampleRate = 1;
    
};

template <class T>
YinPitchDetection<T>::YinPitchDetection(audioFrame audioFrame, float sampleRate) : sampleRate(sampleRate)
{
    inputBuffer.resize(1, std::vector<T>(audioFrame.size()));
    inputBuffer[0] = audioFrame;
    outputBuffer.resize(1);
    doYin();
}

template <class T>
YinPitchDetection<T>::YinPitchDetection(std::vector<audioFrame> vectorWithAudioFrames, float sampleRate) : sampleRate(sampleRate)
{
    inputBuffer = vectorWithAudioFrames;
    outputBuffer.resize(vectorWithAudioFrames.size());
    doYin();
}

template <class T>
void YinPitchDetection<T>::doYin()
{
    size_t frameSize = inputBuffer[0].size();
    size_t halfFrameSize = frameSize/2;
    std::vector<float> yinBuffer(halfFrameSize);
    
    for (auto frame = 0; frame < inputBuffer.size(); frame++)
    {
        ////// step 1: difference
        float delta;
        
        for(int tau = 0 ; tau < halfFrameSize; tau++){
            
            /* Take the difference of the signal with a shifted version of itself, then square it.
             * (This is the Yin algorithm's tweak on autocorellation) */
            for(int i = 0; i < halfFrameSize; i++)
            {
                delta = inputBuffer[frame][i] - inputBuffer[frame][i + tau];
                yinBuffer[tau] += delta * delta;
            }
        }
        
        
        //////  Step 2: Calculate the cumulative mean on the normalised difference calculated in step 1
        float runningSum = 0;
        yinBuffer[0] = 1;
        
        /* Sum all the values in the autocorellation buffer and nomalise the result, replacing
         * the value in the autocorellation buffer with a cumulative mean of the normalised difference */
        for (int tau = 1; tau < halfFrameSize; tau++)
        {
            runningSum += yinBuffer[tau];
            yinBuffer[tau] *= tau / runningSum;
        }
        
        
        ////// Step 3: Search through the normalised cumulative mean array and find values that are over the threshold
        
        int tau;
        for (tau = 2; tau < halfFrameSize; tau++)
        {
            if (yinBuffer[tau] < threshold)
            {
                while (tau + 1 < halfFrameSize && yinBuffer[tau + 1] < yinBuffer[tau])
                {
                    tau++;
                }
                /* found tau, exit loop and return
                 * store the probability
                 * From the YIN paper: The yin->threshold determines the list of
                 * candidates admitted to the set, and can be interpreted as the
                 * proportion of aperiodic power tolerated
                 * within a periodic signal.
                 *
                 * Since we want the periodicity and and not aperiodicity:
                 * periodicity = 1 - aperiodicity */
                probability = 1 - yinBuffer[tau];
                break;
            }
        }
        
        
        float pitch = 0;
        
        /* if no pitch found, tau => 0 */
        if (tau == halfFrameSize || yinBuffer[tau] >= threshold)
        {
            tau = 0;
            probability = 0;
        }
        else
        {
            ////// Step 5: Interpolate the shift value (tau) to improve the pitch estimate
            
            float betterTau;
            //float Estimatetau = tau;
            int x0;
            int x2;
            
            /* Calculate the first polynomial coeffcient based on the current estimate of tau */
            if (tau < 1)
            {
                x0 = tau;
            }
            else
            {
                x0 = tau - 1;
            }
            
            /* Calculate the second polynomial coeffcient based on the current estimate of tau */
            if (tau + 1 < halfFrameSize)
            {
                x2 = tau + 1;
            }
            else
            {
                x2 = tau;
            }
            
            // Parabolische interpolatie!!!!!!!!!!!!!1
            /* Algorithm to parabolically interpolate the shift value tau to find a better estimate */
            if (x0 == tau)
            {
                if (yinBuffer[tau] <= yinBuffer[x2])
                {
                    betterTau = tau;
                }
                else
                {
                    betterTau = x2;
                }
            }
            else if (x2 == tau)
            {
                if (yinBuffer[tau] <= yinBuffer[x0])
                {
                    betterTau = tau;
                }
                else
                {
                    betterTau = x0;
                }
            }
            else
            {
                float s0, s1, s2;
                s0 = yinBuffer[x0];
                s1 = yinBuffer[tau];
                s2 = yinBuffer[x2];
                // fixed AUBIO implementation, thanks to Karl Helgason:
                // (2.0f * s1 - s2 - s0) was incorrectly multiplied with -1
                betterTau = tau + (s2 - s0) / (2 * (2 * s1 - s2 - s0));
            }
            
            pitch = sampleRate / betterTau;
        }
        
        outputBuffer[frame] = pitch;
    }
    
}


#endif /* YinPitchDetection_hpp */

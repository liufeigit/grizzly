//
//  APAudioEnvelope.h
//  MTGSC
//
//  Created by Danny van Swieten on 20-05-14.
//
//

#ifndef __MTGSC__APAudioEnvelope__
#define __MTGSC__APAudioEnvelope__

namespace bear::audio
{
    class Envelope
    {
    public:
        
        Envelope();
        float getAmplitude();
        void calculateMultiplier(float startLevel,
                                 float endLevel,
                                 long int time);
        
        enum EnvelopeState
        {
            OFF,
            ATTACK,
            DECAY,
            SUSTAIN,
            RELEASE,
            NUMSTATES
        };
        
        void enterNextStage(EnvelopeState state);
        inline EnvelopeState getCurrentState() const {return currentState;};
        
    private:
        
        EnvelopeState currentState;
        float amplitude;
        float multiplier;
        long int currentSampleIndex;
        long int indexForNextState;
        float stateValues[EnvelopeState::NUMSTATES];
        
        const float minValue;
    };
}


#endif /* defined(__MTGSC__APAudioEnvelope__) */

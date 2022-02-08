#pragma once
#include <JuceHeader.h>

enum CurveType { linear, exponential };

struct MidiEvent
{
    MidiEvent(juce::MidiMessage curMessage) : midiMessage(curMessage), timeElapsed(0),
        nextRepetitionTime(0), numRepetitions(0)
    {
    }
    
    void setNextRepetitionTime(int rate, int duration, CurveType curveType) {
        if (nextRepetitionTime >= duration)
        {
            nextRepetitionTime = 0;
            timeElapsed = 0;
            numRepetitions = 0;
        }
        
        ++numRepetitions;
        
        // behavior for different curve types is added here
        switch(curveType)
        {
            case CurveType::linear      :
                nextRepetitionTime += rate;
                break;
            case CurveType::exponential :
                nextRepetitionTime = (int)(sqrt(numRepetitions) * rate);
        }
        
        if (nextRepetitionTime > duration)  nextRepetitionTime = duration;
        
    }
    
    juce::MidiMessage midiMessage;
    int timeElapsed;
    int nextRepetitionTime;
    int numRepetitions;
    
};

class MidiProcessor {
public:    
    void prepare(double envSampleRate);
    void processBuffer(juce::MidiBuffer& midiMessages, double bufferSize);
    void addAndRemoveEvents(juce::MidiBuffer &midiMessages);
    
    void setSampleRate(double rate);
    void setRate(double rateInSecs);
    void setDuration(double rateInSecs);
    void setCurve(CurveType curve);
    
private:
    double sampleRate;
    int rate;
    int duration;
    CurveType curveType;
    std::vector<MidiEvent> midiEvents;
    
};

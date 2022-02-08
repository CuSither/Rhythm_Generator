#include "MidiProcessor.h"

void MidiProcessor::prepare(double envSampleRate)
{
    sampleRate = envSampleRate;
    rate = (int) (0.5 * sampleRate);
    duration = (int) (2.0 * sampleRate);
    curveType = CurveType::linear;
    DBG("Prepared");
}

void MidiProcessor::processBuffer(juce::MidiBuffer &midiMessages, double bufferSize)
{
    addAndRemoveEvents(midiMessages);

    for (MidiEvent& event : midiEvents)
    {
        while (event.nextRepetitionTime + 1 < event.timeElapsed + bufferSize)
        {
            juce::MidiMessage message = event.midiMessage;
            midiMessages.addEvent(juce::MidiMessage::noteOff(message.getChannel(), message.getNoteNumber()),
                                  event.nextRepetitionTime - event.timeElapsed);
            midiMessages.addEvent(event.midiMessage, event.nextRepetitionTime - event.timeElapsed + 1);
            event.setNextRepetitionTime(rate, duration, curveType);
            
        }
        
        event.timeElapsed += bufferSize;
    }
}

void MidiProcessor::addAndRemoveEvents(juce::MidiBuffer &midiMessages)
{
    for (const auto message : midiMessages)
    {
        juce::MidiMessage curMessage = message.getMessage();
        
        if (curMessage.isNoteOn())
        {
            MidiEvent newEvent = MidiEvent(curMessage);
            midiEvents.push_back(newEvent);
            midiMessages.clear(curMessage.getTimeStamp(), 1);
        }
        else if (curMessage.isNoteOff())
        {
            for (auto event = begin(midiEvents); event != end(midiEvents);)
            {
                if (event->midiMessage.getNoteNumber() == curMessage.getNoteNumber())   midiEvents.erase(event);
                else    ++event;
            }
        }
    }
}

void MidiProcessor::setSampleRate(double rate) { sampleRate = rate; }

void MidiProcessor::setRate(double rateInSecs) { rate = (int) (rateInSecs * sampleRate); }

void MidiProcessor::setDuration(double rateInSecs) { duration = (int) (rateInSecs * sampleRate); }

void MidiProcessor::setCurve(CurveType curve) { curveType = curve; }

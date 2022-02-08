/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
const double defaultRate = 0.5;
const double defaultRateMin = 0.2;
const double defaultRateMax = 1.0;

const double defaultDuration = 2.0;
const double defaultDurationMin = 0.5;
const double defaultDurationMax = 8.0;


class Rhythm_GeneratorAudioProcessorEditor  :
    public juce::AudioProcessorEditor,
    public juce::Slider::Listener,
    public juce::ComboBox::Listener
{
public:
    Rhythm_GeneratorAudioProcessorEditor (Rhythm_GeneratorAudioProcessor&);
    ~Rhythm_GeneratorAudioProcessorEditor() override;
    
    void addKnob(juce::Slider& knob, juce::Label& label, std::string name, double min, double max, double startValue);

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Rhythm_GeneratorAudioProcessor& audioProcessor;
    
    juce::Slider rateKnob;
    juce::Label rateLabel;
    juce::Slider durationKnob;
    juce::Label durationLabel;
    juce::ComboBox curveTypeMenu;
    
    void sliderValueChanged(juce::Slider* sliderChanged) override;
    void comboBoxChanged(juce::ComboBox* comboBoxChanged) override;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Rhythm_GeneratorAudioProcessorEditor)
};

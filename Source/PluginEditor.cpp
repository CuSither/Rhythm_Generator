/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Rhythm_GeneratorAudioProcessorEditor::Rhythm_GeneratorAudioProcessorEditor (Rhythm_GeneratorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addKnob(rateKnob, rateLabel, "Rate", defaultRateMin, defaultRateMax, defaultRate);
    addKnob(durationKnob, durationLabel, "Duration", defaultDurationMin, defaultDurationMax, defaultDuration);
    
    addAndMakeVisible(curveTypeMenu);
    curveTypeMenu.addListener(this);
    curveTypeMenu.addItem("Linear", 1);
    curveTypeMenu.addItem("Exponential", 2);
    curveTypeMenu.setSelectedId(1);
    
    setOpaque(true);
    
    setSize (400, 200);
}

Rhythm_GeneratorAudioProcessorEditor::~Rhythm_GeneratorAudioProcessorEditor()
{
}

//==============================================================================
void Rhythm_GeneratorAudioProcessorEditor::addKnob(juce::Slider& knob, juce::Label& label,
                                                   std::string name, double min, double max, double startValue)
{
    addAndMakeVisible(knob);
    knob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    knob.addListener(this);
    knob.setRange(min, max, 0.01);
    knob.setValue(startValue);
    
    addAndMakeVisible(label);
    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.attachToComponent(&knob, false);
}

void Rhythm_GeneratorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    
}

void Rhythm_GeneratorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Rectangle<int> bounds(getLocalBounds());
    juce::Rectangle<int> top(bounds.removeFromTop(30));
    curveTypeMenu.setBounds(top);
    
    bounds.removeFromTop(20);
    juce::Rectangle<int> bottomLeft(bounds.removeFromLeft(bounds.getWidth() / 2));
    
    rateKnob.setBounds(bottomLeft.reduced(10));
    durationKnob.setBounds(bounds.reduced(10));
}

void Rhythm_GeneratorAudioProcessorEditor::sliderValueChanged(juce::Slider* sliderChanged)
{
    if (sliderChanged == &rateKnob)
    {
        durationKnob.setRange(fmax(defaultDurationMin, rateKnob.getValue() + 0.01), defaultDurationMax, 0.01);
        audioProcessor.midiProcessor.setRate(rateKnob.getValue());
        repaint();
    }
    else if (sliderChanged == &durationKnob)
    {
        rateKnob.setRange(defaultRateMin, fmin(defaultRateMax, durationKnob.getValue() - 0.01), 0.01);
        audioProcessor.midiProcessor.setDuration(durationKnob.getValue());
        repaint();
    }
}

void Rhythm_GeneratorAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxChanged)
{
    if (comboBoxChanged == &curveTypeMenu)
    {
        int curId = comboBoxChanged->getSelectedId();
        
        if (curId == 1)         audioProcessor.midiProcessor.setCurve(CurveType::linear);
        else if (curId == 2)    audioProcessor.midiProcessor.setCurve(CurveType::exponential);
        
    }
}

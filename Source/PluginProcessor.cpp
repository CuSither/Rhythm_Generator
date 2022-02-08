/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Rhythm_GeneratorAudioProcessor::Rhythm_GeneratorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

Rhythm_GeneratorAudioProcessor::~Rhythm_GeneratorAudioProcessor()
{
}

//==============================================================================
const juce::String Rhythm_GeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Rhythm_GeneratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Rhythm_GeneratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Rhythm_GeneratorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Rhythm_GeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Rhythm_GeneratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Rhythm_GeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Rhythm_GeneratorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Rhythm_GeneratorAudioProcessor::getProgramName (int index)
{
    return {};
}

void Rhythm_GeneratorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Rhythm_GeneratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    midiProcessor.prepare(sampleRate);
}

void Rhythm_GeneratorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Rhythm_GeneratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// This is where the buffer is processed! Midi and audio come in two separate buffers and can be processed uniquely. 
void Rhythm_GeneratorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    midiProcessor.processBuffer(midiMessages, buffer.getNumSamples());
}

//==============================================================================
bool Rhythm_GeneratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Rhythm_GeneratorAudioProcessor::createEditor()
{
    return new Rhythm_GeneratorAudioProcessorEditor (*this);
}

//==============================================================================
void Rhythm_GeneratorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Rhythm_GeneratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Rhythm_GeneratorAudioProcessor();
}

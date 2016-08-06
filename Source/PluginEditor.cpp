/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HrtfPluginAudioProcessorEditor::HrtfPluginAudioProcessorEditor (HrtfPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible (leftKnob);
    addAndMakeVisible (rightKnob);
    leftKnob->setSliderStyle(Slider::Rotary);
    leftKnob->setRotaryParameters(PI,PI*3,true);
    leftKnob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    leftKnob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    leftKnob->setRange(-180,180,0);
    leftKnob->addListener(this);
    leftKnob->setValue(processor.azimuth->get());
    rightKnob->setSliderStyle(Slider::Rotary);
    rightKnob->setRotaryParameters(PI,PI*3,true);
    rightKnob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    rightKnob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    rightKnob->setRange(-180,180,0);
    //rightKnob->addListener(this);
    //rightKnob->setValue(processor.azimuth->get());
    //processor.uileftKnob = leftKnob;
    setSize (450, 210);
}

HrtfPluginAudioProcessorEditor::~HrtfPluginAudioProcessorEditor()
{
    delete leftKnob;
    delete rightKnob;
}

//==============================================================================
void HrtfPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::silver);
    //leftKnob->setValue(processor.azimuth->get());
   // g.setColour (Colours::black);
   // g.setFont (15.0f);
   // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void HrtfPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    leftKnob->setBounds(50,20,170,170);
    rightKnob->setBounds(230,20,170,170);
}

void HrtfPluginAudioProcessorEditor::sliderValueChanged(Slider* slider){
    processor.setAzimuth(slider->getValue());
}

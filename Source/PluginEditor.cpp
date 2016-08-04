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
    addAndMakeVisible (knob);
    knob->setSliderStyle(Slider::Rotary);
    knob->setRotaryParameters(PI,PI*3,true);
    knob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    knob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    knob->setRange(-180,180,0);
    knob->addListener(this);
    knob->setValue(processor.azimuth->get());
    //processor.uiKnob = knob;
    setSize (400, 300);
}

HrtfPluginAudioProcessorEditor::~HrtfPluginAudioProcessorEditor()
{
    delete knob;
}

//==============================================================================
void HrtfPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::silver);
    //knob->setValue(processor.azimuth->get());
   // g.setColour (Colours::black);
   // g.setFont (15.0f);
   // g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void HrtfPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    knob->setBounds(100,50,200,200);

}

void HrtfPluginAudioProcessorEditor::sliderValueChanged(Slider* slider){
    processor.setAzimuth(slider->getValue());
}

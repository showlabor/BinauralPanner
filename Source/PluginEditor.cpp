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
    addAndMakeVisible (leftToggle);
    addAndMakeVisible (rightToggle);
    leftKnob->setSliderStyle(Slider::Rotary);
    leftKnob->setRotaryParameters(PI,PI*3,true);
    leftKnob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    leftKnob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    leftKnob->setRange(-180,180,0);
    leftKnob->addListener(this);
    leftKnob->setValue(processor.getAzimuth(0));
    rightKnob->setSliderStyle(Slider::Rotary);
    rightKnob->setRotaryParameters(PI,PI*3,true);
    rightKnob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    rightKnob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    rightKnob->setRange(-180,180,0);
    rightKnob->addListener(this);
    rightKnob->setValue(processor.getAzimuth(1));
    //processor.uileftKnob = leftKnob;

    leftToggle->addListener(this);
    leftToggle->setButtonText("ON");
    leftToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
    leftToggle->setColour(TextButton::textColourOffId, Colours::black);
    isLeftToggleOff = false;
    rightToggle->addListener(this);
    rightToggle->setButtonText("ON");
    rightToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
    rightToggle->setColour(TextButton::textColourOffId, Colours::black);
    isRightToggleOff = false;
    setSize (450, 210);

    startTimer(200);
}

HrtfPluginAudioProcessorEditor::~HrtfPluginAudioProcessorEditor()
{
    //delete leftKnob;
    //delete rightKnob;
    leftKnob = nullptr;
    rightKnob = nullptr;
    leftToggle = nullptr;
    rightToggle = nullptr;
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
    leftToggle->setBounds(50,15,30,30);
    leftToggle->changeWidthToFitText();
    rightToggle->setBounds(230,15,30,30);
    rightToggle->changeWidthToFitText();
}

void HrtfPluginAudioProcessorEditor::sliderValueChanged(Slider* slider){
    if(slider == leftKnob){
        processor.setAzimuth(0,slider->getValue());
    }else if (slider == rightKnob){
        processor.setAzimuth(1,slider->getValue());
    }
}

void HrtfPluginAudioProcessorEditor::buttonClicked(Button* button){
    if(button == (Button*)leftToggle){
        if(isLeftToggleOff){
            leftToggle->setButtonText("ON");
            leftToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
            leftToggle->setColour(TextButton::textColourOffId, Colours::black);
            isLeftToggleOff = false;
            processor.setMute(0,false);
            DBG("setToOn");
        }else{
            leftToggle->setButtonText("OFF");
            leftToggle->setColour(TextButton::buttonColourId, Colours::whitesmoke);
            leftToggle->setColour(TextButton::textColourOffId, Colours::black);
            isLeftToggleOff= true;
            processor.setMute(0,true);
            DBG("setTOOff");
        }
    }
    if(button == (Button*) rightToggle){
        if(isRightToggleOff){
            rightToggle->setButtonText("ON");
            rightToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
            rightToggle->setColour(TextButton::textColourOffId, Colours::black);
            isRightToggleOff = false;
            processor.setMute(1,false);
        }else{
            rightToggle->setButtonText("OFF");
            rightToggle->setColour(TextButton::buttonColourId, Colours::whitesmoke);
            rightToggle->setColour(TextButton::textColourOffId, Colours::black);
            processor.setMute(1,true);
            isRightToggleOff = true;
        }
    }
}

void HrtfPluginAudioProcessorEditor::timerCallback(){
    if(processor.getAzimuth(0) != (float)(leftKnob->getValue())){
        leftKnob->setValue(processor.getAzimuth(0));
    }
    if(processor.getAzimuth(1) != (float)(rightKnob->getValue())){
        rightKnob->setValue(processor.getAzimuth(1));
    }
}

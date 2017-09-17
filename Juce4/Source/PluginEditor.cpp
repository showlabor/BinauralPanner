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
    addAndMakeVisible (meter);
    addAndMakeVisible (title);

    leftKnob->setSliderStyle(Slider::Rotary);
    leftKnob->setRotaryParameters(PI,PI*3,true);
    //leftKnob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    leftKnob->setColour(Slider::rotarySliderFillColourId, Colours::cyan);
    //leftKnob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    leftKnob->setColour(Slider::rotarySliderOutlineColourId, Colour(35,99,134));
    leftKnob->setRange(-180,180,0);
    leftKnob->addListener(this);
    leftKnob->setValue(processor.getAzimuth(0));
    rightKnob->setSliderStyle(Slider::Rotary);
    rightKnob->setRotaryParameters(PI,PI*3,true);
    leftKnob->setColour(Slider::textBoxBackgroundColourId, Colour(43,43,43));
    leftKnob->setColour(Slider::textBoxTextColourId, Colours::cyan);
    //rightKnob->setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    rightKnob->setColour(Slider::rotarySliderFillColourId, Colours::cyan);
    //rightKnob->setColour(Slider::rotarySliderOutlineColourId, Colours::black);
    rightKnob->setColour(Slider::rotarySliderOutlineColourId, Colour(35,99,134));
    rightKnob->setRange(-180,180,0);
    rightKnob->addListener(this);
    rightKnob->setValue(processor.getAzimuth(1));
    rightKnob->setColour(Slider::textBoxBackgroundColourId, Colour(43,43,43));
    rightKnob->setColour(Slider::textBoxTextColourId, Colours::cyan);
    //processor.uileftKnob = leftKnob;

    leftToggle->addListener(this);
    leftToggle->setButtonText("ON");
    //leftToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
    leftToggle->setColour(TextButton::buttonColourId, Colours::cyan);
    leftToggle->setColour(TextButton::textColourOffId, Colours::black);
    isLeftToggleOff = false;
    rightToggle->addListener(this);
    rightToggle->setButtonText("ON");
    //rightToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
    rightToggle->setColour(TextButton::buttonColourId, Colours::cyan);
    rightToggle->setColour(TextButton::textColourOffId, Colours::black);
    isRightToggleOff = false;


    title->setText( String("Stereo Binaural Panner"),NotificationType::dontSendNotification);
    title->setColour(Label::backgroundColourId, Colour(43,43,43));
    title->setColour(Label::textColourId, Colours::cyan);
    //title->setFont(Font("Arial",10,1));

    setSize (450, 205);
    startTimer(135);

}

HrtfPluginAudioProcessorEditor::~HrtfPluginAudioProcessorEditor()
{

    //delete leftKnob;
    //delete rightKnob;
    leftKnob = nullptr;
    rightKnob = nullptr;
    leftToggle = nullptr;
    rightToggle = nullptr;
    meter = nullptr;
    title = nullptr;

}

//==============================================================================
void HrtfPluginAudioProcessorEditor::paint (Graphics& g)
{

    g.fillAll (Colour(73,73,73));
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
    meter->setBounds(50, 165, meter->getX(),meter->getY());
    title->setBounds(300,2,150,30);

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
            //leftToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
            leftToggle->setColour(TextButton::buttonColourId, Colours::cyan);
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
            //rightToggle->setColour(TextButton::buttonColourId, Colour(56,255,56));
            rightToggle->setColour(TextButton::buttonColourId, Colours::cyan);
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
    meter->setValue(0,processor.getRms(0));
    meter->setValue(1,processor.getRms(1));

}


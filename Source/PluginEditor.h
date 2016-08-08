/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class HrtfPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Slider::Listener,
                                        public TextButton::Listener,
                                        public Timer
{
public:
    HrtfPluginAudioProcessorEditor (HrtfPluginAudioProcessor&);
    ~HrtfPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void timerCallback() override;
    void buttonClicked(Button* button) override;

private:

    const float PI = 3.14159265359;
    ScopedPointer<Slider> leftKnob = new Slider();
    ScopedPointer<Slider> rightKnob = new Slider();
    ScopedPointer<TextButton> leftToggle= new TextButton();
    ScopedPointer<TextButton> rightToggle= new TextButton();
    bool isLeftToggleOff, isRightToggleOff;

    //TextButton* leftToggle= new TextButton();
    //TextButton* rightToggle= new TextButton();
    //Slider* leftKnob = new Slider();
    //Slider* rightKnob = new Slider();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    //float PI = 3.14159265359;
    HrtfPluginAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HrtfPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED

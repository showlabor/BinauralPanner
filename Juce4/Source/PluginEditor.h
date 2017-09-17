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
#include "RmsMeter.h"


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
    ScopedPointer<RmsMeter> meter = new RmsMeter(350,30,2);
    ScopedPointer<Label> title = new Label();
    bool isLeftToggleOff, isRightToggleOff;


    HrtfPluginAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HrtfPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED

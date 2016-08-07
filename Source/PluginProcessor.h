/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <fftw3.h>


//==============================================================================
/**
*/
class HrtfPluginAudioProcessor  : public AudioProcessor{
public:
    //==============================================================================
    HrtfPluginAudioProcessor();
    ~HrtfPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //===============================================================================
    void setAzimuth(int channel, float value);
    float getAzimuth(int channel);
    void setMute(int channel, bool value);
    int getHrirIndex(int channel);

    //AudioParameterFloat* azimuth;
    //Slider* uiKnob;


private:
    //==============================================================================

    const float PI = 3.14159265359;
    int hrirLength, fftSize;// flooredIndex;
    //float previousAzimuth, floorAmp, ceilAmp, previousFloorAmp, previousCeilAmp;
    //bool isFirstBuffer;

    struct ChannelVariables{
        bool isFirstBuffer, isMute;
        int flooredIndex;
        float previousAzimuth, floorAmp, ceilAmp, rms;
        AudioParameterFloat* azimuth;
    }leftChannelVariables, rightChannelVariables;

    Array<Array<float>> hrir;
    Array<float> rightPreviousOutput,leftPreviousOutput;
    //Array<float> *leftHrirArray,*rightHrirArray,*leftInterpHrirArray,*rightInterpHrirArray;

    //fftwf_complex *inputSignal, *leftHRIR, *rightHRIR, *outLeft, *outRight;


    struct ChannelFftData{
        fftwf_complex *inputSignal, *leftHRIR, *rightHRIR;
        fftwf_plan inputFFT, leftHrirFFT, rightHrirFFT;
        Array<float> *leftHrirArray,*rightHrirArray,*leftInterpHrirArray,*rightInterpHrirArray;
    }leftChannelFftData, rightChannelFftData;

    fftwf_complex *outLeft, *outRight;
    fftwf_plan outLeftIFFT, outRightIFFT;
    //fftwf_plan inputFFT, leftHrirFFT, rightHrirFFT, outLeftIFFT, outRightIFFT;

    void addHrirsToArray();



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HrtfPluginAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED

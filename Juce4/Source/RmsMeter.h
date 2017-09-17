/*
  ==============================================================================

    RmsMeter.h
    Created: 9 Aug 2016 8:23:26pm
    Author:  Wing Sang Wong

  ==============================================================================
*/

#ifndef RMSMETER_H_INCLUDED
#define RMSMETER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class RmsMeter    : public Component
{
public:
    RmsMeter();
    RmsMeter(int sizeX, int sizeY, int numOfChannel);
    ~RmsMeter();

    void paint (Graphics&) override;
    void resized() override;
    void setValue(int channel, float rms);
    void setSize(int x, int y);
    int getX();
    int getY();
    void setZeroDbPosition(int positionInPixel);
    void setNumOfChannel(int value);
    void setMeterBackgroundColour(Colour bg);
    void setMeterBarColour(Colour barCl);

private:
    std::vector<int> rmsValues = std::vector<int>();
    struct{
    float minimumRms = 0.0001;
    int sizeX = 350;
    int sizeY = 30;
    int zeroDbPosition = 350;
    int numOfChannel = 2;
    Colour meterBarColour = Colours::cyan;
    Colour meterBackground = Colour(43,43,43);
    }meterSetting;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RmsMeter)
};


#endif  // RMSMETER_H_INCLUDED

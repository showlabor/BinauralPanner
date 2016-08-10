/*
  ==============================================================================

    RmsMeter.cpp
    Created: 9 Aug 2016 8:23:26pm
    Author:  wilsonubuntustudio

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RmsMeter.h"

//==============================================================================
RmsMeter::RmsMeter()
{
    for(int i = 0; i< meterSetting.numOfChannel; ++i){
        rmsValues.push_back(0);
    }
}

RmsMeter::RmsMeter(int x, int y, int num){
    meterSetting.sizeX = x;
    meterSetting.sizeY = y;
    meterSetting.numOfChannel = num;
    for(int i = 0; i< num; ++i){
        rmsValues.push_back(0);
    }
}

RmsMeter::~RmsMeter()
{
}

void RmsMeter::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (meterSetting.meterBackground);   // clear the background
    g.setColour (meterSetting.meterBarColour);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    int thickness = meterSetting.sizeY/meterSetting.numOfChannel;
    for(int ch = 0; ch < meterSetting.numOfChannel; ++ch){
        int yPos = ch == 0? (int)(thickness/2) :(int)((meterSetting.sizeY/(ch+1))+(thickness/2));
        g.drawLine(0,yPos,rmsValues[ch],yPos,thickness);
    }
    //g.setColour (Colours::lightblue);
    //g.setFont (14.0f);
    //g.drawText ("RmsMeter", getLocalBounds(),
    //            Justification::centred, true);   // draw some placeholder text
}

void RmsMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void RmsMeter::setValue(int channel, float value){
    if(rmsValues[channel] != value){
        if (value < meterSetting.minimumRms){
            rmsValues[channel] = 0;
        }else{
            rmsValues[channel] = (int)(std::log10(value)*1/5*meterSetting.zeroDbPosition)+meterSetting.zeroDbPosition;
        }
        //DBG(rmsValue);
        repaint();
    }
}

void RmsMeter::setSize(int x, int y){
    meterSetting.sizeX = x;
    meterSetting.sizeY = y;
}

void RmsMeter::setNumOfChannel(int ch){
    meterSetting.zeroDbPosition = ch;
}

void RmsMeter::setZeroDbPosition(int pos){
    meterSetting.zeroDbPosition = pos;
}

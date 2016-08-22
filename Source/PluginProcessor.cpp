/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HrtfPluginAudioProcessor::HrtfPluginAudioProcessor()
{
    //initialize AudioParameeters

    leftChannelVariables.azimuth = new AudioParameterFloat("Azimuth", "Azimuth",-180,180,0);
    leftChannelVariables.previousAzimuth = -180;
    addParameter(leftChannelVariables.azimuth);
    leftChannelVariables.isFirstBuffer = true;
    leftChannelVariables.isMute = false;

    rightChannelVariables.azimuth = new AudioParameterFloat("Azimuth", "Azimuth",-180,180,0);
    rightChannelVariables.previousAzimuth = -180;
    addParameter(rightChannelVariables.azimuth);
    rightChannelVariables.isFirstBuffer = true;
    rightChannelVariables.isMute = false;

    multiChannelsFft = MultiChannelsFft();

}

HrtfPluginAudioProcessor::~HrtfPluginAudioProcessor()
{
/*
//for some reason fftw_free have to be called before destroy plan
    fftwf_free(leftChannelFftData.inputSignal);
    fftwf_free(leftChannelFftData.leftHRIR);
    fftwf_free(leftChannelFftData.rightHRIR);

    fftwf_free(rightChannelFftData.inputSignal);
    fftwf_free(rightChannelFftData.leftHRIR);
    fftwf_free(rightChannelFftData.rightHRIR);
  //  fftwf_free(leftInterpHRIR);
  //  fftwf_free(rightInterpHRIR);
    fftwf_free(outLeft);
    fftwf_free(outRight);

    fftwf_destroy_plan(leftChannelFftData.inputFFT);
    fftwf_destroy_plan(leftChannelFftData.leftHrirFFT);
    fftwf_destroy_plan(leftChannelFftData.rightHrirFFT);

    fftwf_destroy_plan(rightChannelFftData.inputFFT);
    fftwf_destroy_plan(rightChannelFftData.leftHrirFFT);
    fftwf_destroy_plan(rightChannelFftData.rightHrirFFT);
   // fftwf_destroy_plan(leftInterpHrirFFT);
   // fftwf_destroy_plan(rightInterpHrirFFT);
    fftwf_destroy_plan(outLeftIFFT);
    fftwf_destroy_plan(outRightIFFT);
    */

}
//==============================================================================
const String HrtfPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HrtfPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HrtfPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double HrtfPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HrtfPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HrtfPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HrtfPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String HrtfPluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void HrtfPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void HrtfPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    if(hrirSampleRate != (int)sampleRate){
    //DBG(sampleRate);
    //DBG(hrirSampleRate);
    //DBG(hrirLength);
        resampleHRIR(sampleRate);
    }
    multiChannelsFft.resizeComplexArrays(powerOfTwoConst[(int)std::ceil(std::log(samplesPerBlock+hrirLength-1)/std::log(2))]);
    DBG(powerOfTwoConst[(int)std::ceil(std::log(samplesPerBlock+hrirLength-1)/std::log(2))]);
    //call the resizeChannels if more than 2 channels are required
/*
    //initialize fftSize fftw arrays and fftw plans
    //fftSize = std::pow(2,std::ceil(std::log(samplesPerBlock+hrirLength-1)/std::log(2)));
    if(!isFftwPrepared){
    fftSize = powerOfTwoConst[(int)std::ceil(std::log(samplesPerBlock+hrirLength-1)/std::log(2))];

    leftChannelFftData.inputSignal = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);
    leftChannelFftData.leftHRIR = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);
    leftChannelFftData.rightHRIR = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);

    rightChannelFftData.inputSignal = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);
    rightChannelFftData.leftHRIR = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);
    rightChannelFftData.rightHRIR = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);

    outLeft = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);
    outRight = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*fftSize);

    for(int i =0; i<fftSize ;i++){
        outLeft[i][0] = 0;
        outRight[i][0] = 0;
    }

    leftChannelFftData.inputFFT =
    fftwf_plan_dft_1d(fftSize, leftChannelFftData.inputSignal, leftChannelFftData.inputSignal, FFTW_FORWARD, FFTW_ESTIMATE);
    leftChannelFftData.leftHrirFFT =
    fftwf_plan_dft_1d(fftSize, leftChannelFftData.leftHRIR, leftChannelFftData.leftHRIR, FFTW_FORWARD, FFTW_ESTIMATE);
    leftChannelFftData.rightHrirFFT =
    fftwf_plan_dft_1d(fftSize, leftChannelFftData.rightHRIR, leftChannelFftData.rightHRIR, FFTW_FORWARD, FFTW_ESTIMATE);

    rightChannelFftData.inputFFT =
    fftwf_plan_dft_1d(fftSize, rightChannelFftData.inputSignal, rightChannelFftData.inputSignal, FFTW_FORWARD, FFTW_ESTIMATE);
    rightChannelFftData.leftHrirFFT =
    fftwf_plan_dft_1d(fftSize, rightChannelFftData.leftHRIR, rightChannelFftData.leftHRIR, FFTW_FORWARD, FFTW_ESTIMATE);
    rightChannelFftData.rightHrirFFT =
    fftwf_plan_dft_1d(fftSize, rightChannelFftData.rightHRIR, rightChannelFftData.rightHRIR, FFTW_FORWARD, FFTW_ESTIMATE);

    outLeftIFFT = fftwf_plan_dft_1d(fftSize, outLeft, outLeft, FFTW_BACKWARD, FFTW_ESTIMATE);
    outRightIFFT = fftwf_plan_dft_1d(fftSize, outRight, outRight, FFTW_BACKWARD, FFTW_ESTIMATE);

    isFftwPrepared = true;
    }
*/
    leftChannelVariables.isFirstBuffer = true;
    rightChannelVariables.isFirstBuffer = true;
    DBG(multiChannelsFft.getHrirDebug(0,0));
    DBG("Prepared");

}

void HrtfPluginAudioProcessor::releaseResources()
{
// Ardour 4 will crash if fftw resources are destroyed here, free the memory in the destructor


}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HrtfPluginAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif

void HrtfPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{

    //store the input RMS value -> used for auto muting
    leftChannelVariables.inputRms = buffer.getRMSLevel(0,0,buffer.getNumSamples());
    if(buffer.getNumChannels()>1){
        rightChannelVariables.inputRms = buffer.getRMSLevel(1,0,buffer.getNumSamples());
    }else{
        rightChannelVariables.inputRms =0;
    }

//Left channel Processing
//Copy Hrir vectors
    if(leftChannelVariables.inputRms>0 && !leftChannelVariables.isMute){

        bool updateLeftHrir = (leftChannelVariables.azimuth->get()==leftChannelVariables.previousAzimuth)
        &&(!leftChannelVariables.isFirstBuffer)?false:true;
        leftChannelVariables.previousAzimuth = leftChannelVariables.azimuth->get();

        if(updateLeftHrir){
            leftChannelVariables.isFirstBuffer = false;
            leftChannelVariables.flooredIndex = getHrirIndex(0);
            //DBG(leftChannelVariables.flooredIndex);
            MultiChannelsFft::hrirData currentHrir;
            float rawParam = leftChannelVariables.azimuth->get()+360;
            if(std::floor(rawParam)!=rawParam){
               // leftChannelVariables.ceilAmp = std::sqrt(1-rawParam+std::floor(rawParam));
               // leftChannelVariables.floorAmp = std::sqrt(rawParam-std::floor(rawParam));
                currentHrir.ceilAmp = std::sqrt(1-rawParam+std::floor(rawParam));
                currentHrir.floorAmp = std::sqrt(rawParam-std::floor(rawParam));
            }else{
                //leftChannelVariables.floorAmp = 1;
                //leftChannelVariables.ceilAmp = 0;
                currentHrir.floorAmp = 1;
                currentHrir.ceilAmp = 0;
            }

           // leftChannelFftData.leftHrirArray = hrir[leftChannelVariables.flooredIndex];
           // leftChannelFftData.rightHrirArray = hrir[leftChannelVariables.flooredIndex+1];
           // leftChannelFftData.leftInterpHrirArray = hrir[(leftChannelVariables.flooredIndex+2)%hrir.size()];
           // leftChannelFftData.rightInterpHrirArray = hrir[(leftChannelVariables.flooredIndex+3)%hrir.size()];
            DBG("beforeHRIRCopy");
            DBG(multiChannelsFft.getHrirDebug(0,0));


            currentHrir.hrirBegin[MultiChannelsFft::leftHRIR] = hrir[leftChannelVariables.flooredIndex].begin();
            currentHrir.hrirBegin[MultiChannelsFft::rightHRIR] = hrir[leftChannelVariables.flooredIndex+1].begin();
            currentHrir.hrirBegin[MultiChannelsFft::interpLeftHRIR] = hrir[(leftChannelVariables.flooredIndex+2)%hrir.size()].begin();
            currentHrir.hrirBegin[MultiChannelsFft::interpRightHRIR] = hrir[(leftChannelVariables.flooredIndex+3)%hrir.size()].begin();

            currentHrir.hrirEnd[MultiChannelsFft::leftHRIR] = hrir[leftChannelVariables.flooredIndex].end();
            currentHrir.hrirEnd[MultiChannelsFft::rightHRIR] = hrir[leftChannelVariables.flooredIndex+1].end();
            currentHrir.hrirEnd[MultiChannelsFft::interpLeftHRIR] = hrir[(leftChannelVariables.flooredIndex+2)%hrir.size()].end();
            currentHrir.hrirEnd[MultiChannelsFft::interpRightHRIR] = hrir[(leftChannelVariables.flooredIndex+3)%hrir.size()].end();

            multiChannelsFft.copyHRIR(0,currentHrir);
             DBG("afterFirstHRIRCopy");
            DBG(multiChannelsFft.getHrirDebug(0,0));


       }


//merge HRIR, copy input data and zero padding
        //const float* leftChannelDataRead = buffer.getReadPointer(0);
        multiChannelsFft.copyInput(0,buffer.getReadPointer(0),buffer.getNumSamples());

        if(updateLeftHrir){
            //fftwf_execute(leftChannelFftData.leftHrirFFT);
            //fftwf_execute(leftChannelFftData.rightHrirFFT);
            multiChannelsFft.transformHrir(0);
            DBG("afterFirstTransform");
            DBG(multiChannelsFft.getHrirDebug(0,0));
             //DBG(multiChannelsFft.getHrirDebug(0,0));
        }
        //fftwf_execute(leftChannelFftData.inputFFT);
        multiChannelsFft.transformInput(0);
        //if(updateLeftHrir){
        //DBG("afterFirstInputTransform");
       // DBG(multiChannelsFft.getHrirDebug(0,0));
        //}

    }else{
        buffer.clear(0,0,buffer.getNumSamples());
        leftChannelVariables.isFirstBuffer = true;
    }

//Right channel Processing
    if(buffer.getNumChannels()>1){
     if(rightChannelVariables.inputRms>0 && !rightChannelVariables.isMute ){

        bool updateRightHrir = (rightChannelVariables.azimuth->get()==rightChannelVariables.previousAzimuth)
        &&(!rightChannelVariables.isFirstBuffer)?false:true;
        rightChannelVariables.previousAzimuth = rightChannelVariables.azimuth->get();

        if(updateRightHrir){
            rightChannelVariables.isFirstBuffer = false;
            rightChannelVariables.flooredIndex = getHrirIndex(1);
            //DBG("Loading Hrir "+rightChannelVariables.flooredIndex);
            float rawParam = rightChannelVariables.azimuth->get()+360;
            MultiChannelsFft::hrirData currentHrir;
            if(std::floor(rawParam)!=rawParam){
                //rightChannelVariables.ceilAmp = std::sqrt(1-rawParam+std::floor(rawParam));
                //rightChannelVariables.floorAmp = std::sqrt(rawParam-std::floor(rawParam));
                currentHrir.ceilAmp = std::sqrt(1-rawParam+std::floor(rawParam));
                currentHrir.floorAmp = std::sqrt(rawParam-std::floor(rawParam));
            }else{
                //rightChannelVariables.floorAmp = 1;
                //rightChannelVariables.ceilAmp = 0;
                currentHrir.floorAmp = 1;
                currentHrir.ceilAmp = 0;
            }
            DBG("beforeRightHRIRCopy");
            DBG(multiChannelsFft.getHrirDebug(0,0));
            //rightChannelFftData.leftHrirArray = hrir[rightChannelVariables.flooredIndex];
            //rightChannelFftData.rightHrirArray = hrir[rightChannelVariables.flooredIndex+1];
            //rightChannelFftData.leftInterpHrirArray = hrir[(rightChannelVariables.flooredIndex+2)%hrir.size()];
            //rightChannelFftData.rightInterpHrirArray = hrir[(rightChannelVariables.flooredIndex+3)%hrir.size()];
            currentHrir.hrirBegin[MultiChannelsFft::leftHRIR] = hrir[rightChannelVariables.flooredIndex].begin();
            currentHrir.hrirBegin[MultiChannelsFft::rightHRIR] = hrir[rightChannelVariables.flooredIndex+1].begin();
            currentHrir.hrirBegin[MultiChannelsFft::interpLeftHRIR] = hrir[(rightChannelVariables.flooredIndex+2)%hrir.size()].begin();
            currentHrir.hrirBegin[MultiChannelsFft::interpRightHRIR] = hrir[(rightChannelVariables.flooredIndex+3)%hrir.size()].begin();

            currentHrir.hrirEnd[MultiChannelsFft::leftHRIR] = hrir[rightChannelVariables.flooredIndex].end();
            currentHrir.hrirEnd[MultiChannelsFft::rightHRIR] = hrir[rightChannelVariables.flooredIndex+1].end();
            currentHrir.hrirEnd[MultiChannelsFft::interpLeftHRIR] = hrir[(rightChannelVariables.flooredIndex+2)%hrir.size()].end();
            currentHrir.hrirEnd[MultiChannelsFft::interpRightHRIR] = hrir[(rightChannelVariables.flooredIndex+3)%hrir.size()].end();

            multiChannelsFft.copyHRIR(1,currentHrir);
            DBG("afterRightHRIRCopy");
            DBG(multiChannelsFft.getHrirDebug(0,0));

        }
        multiChannelsFft.copyInput(1,buffer.getReadPointer(1),buffer.getNumSamples());
       // const float* rightChannelDataRead = buffer.getReadPointer(1);
        if(updateRightHrir){
            //fftwf_execute(rightChannelFftData.leftHrirFFT);
           // fftwf_execute(rightChannelFftData.rightHrirFFT);
           multiChannelsFft.transformHrir(1);
           DBG("rightChannelFirstTransform");
           DBG(multiChannelsFft.getHrirDebug(0,0));
        }
       //fftwf_execute(rightChannelFftData.inputFFT);
       multiChannelsFft.transformInput(1);
    }else{
        buffer.clear(1,0,buffer.getNumSamples());
        rightChannelVariables.isFirstBuffer = true;
    }
}

//multiply the inputs and the Hrirs
    if (leftChannelVariables.inputRms > 0 && rightChannelVariables.inputRms > 0 && !leftChannelVariables.isMute && !rightChannelVariables.isMute){
        multiChannelsFft.convolveAll();
        multiChannelsFft.transformOutputs();
      
    }
    else if(leftChannelVariables.inputRms > 0 && !leftChannelVariables.isMute){
        multiChannelsFft.convolveOneChannel(0);
        multiChannelsFft.transformOutputs();
    
    } else if (rightChannelVariables.inputRms > 0 && !rightChannelVariables.isMute){
        multiChannelsFft.convolveOneChannel(1);
        multiChannelsFft.transformOutputs();
    
    }

//put the result to the buffer
    if(buffer.getNumChannels()>1) {
    float* leftChannelDataWrite = buffer.getWritePointer(0);
    float* rightChannelDataWrite = buffer.getWritePointer(1);
    int samplesInpreviousOutput = leftPreviousOutput.size();

    for(int sample = 0; sample < multiChannelsFft.getFftSize();sample++){

        //float leftSample = outLeft[sample][0]/fftSize;
        //float rightSample = outRight[sample][0]/fftSize;

        float leftSample = multiChannelsFft.getOutput(0,sample);
        float rightSample = multiChannelsFft.getOutput(1,sample);
       // if(sample == 100) {DBG(leftSample);}

        if(sample < buffer.getNumSamples()){
            if(sample<samplesInpreviousOutput){
                leftChannelDataWrite[sample] = leftSample + leftPreviousOutput[sample];
                rightChannelDataWrite[sample] = rightSample + rightPreviousOutput[sample];
            }else{
                leftChannelDataWrite[sample] = leftSample;
                rightChannelDataWrite[sample] = rightSample;
            }
        }
        else if(sample<samplesInpreviousOutput){
            leftPreviousOutput[sample] += leftSample;
            rightPreviousOutput[sample] += rightSample;
        }else{
            leftPreviousOutput.push_back(leftSample);
            rightPreviousOutput.push_back(rightSample);
        }
       // outLeft[sample][0] = 0;
       // outRight[sample][0] = 0;
    }
    multiChannelsFft.clearOutputs();

    if(samplesInpreviousOutput > buffer.getNumSamples()){
        leftPreviousOutput.erase(leftPreviousOutput.begin(),leftPreviousOutput.begin()+buffer.getNumSamples());
        rightPreviousOutput.erase(rightPreviousOutput.begin(),rightPreviousOutput.begin()+buffer.getNumSamples());
    }else if(samplesInpreviousOutput>0){
        leftPreviousOutput.erase(leftPreviousOutput.begin(),leftPreviousOutput.begin()+samplesInpreviousOutput);
        rightPreviousOutput.erase(rightPreviousOutput.begin(),rightPreviousOutput.begin()+samplesInpreviousOutput);
    }

    leftChannelVariables.outputRms = buffer.getRMSLevel(0,0,buffer.getNumSamples());
    rightChannelVariables.outputRms = buffer.getRMSLevel(1,0,buffer.getNumSamples());
}else{
        
        float* leftChannelDataWrite = buffer.getWritePointer(0);
        int samplesInpreviousOutput = leftPreviousOutput.size();
        
        for(int sample = 0; sample < multiChannelsFft.getFftSize();sample++){
            
            //float leftSample = outLeft[sample][0]/fftSize;
            //float rightSample = outRight[sample][0]/fftSize;
            
            float leftSample = multiChannelsFft.getOutput(0,sample);
            // if(sample == 100) {DBG(leftSample);}
            
            if(sample < buffer.getNumSamples()){
                if(sample<samplesInpreviousOutput){
                    leftChannelDataWrite[sample] = leftSample + leftPreviousOutput[sample];
                }else{
                    leftChannelDataWrite[sample] = leftSample;
                }
            }
            else if(sample<samplesInpreviousOutput){
                leftPreviousOutput[sample] += leftSample;
            }else{
                leftPreviousOutput.push_back(leftSample);
            }
            // outLeft[sample][0] = 0;
            // outRight[sample][0] = 0;
        }
        multiChannelsFft.clearOutputs();
        
        if(samplesInpreviousOutput > buffer.getNumSamples()){
            leftPreviousOutput.erase(leftPreviousOutput.begin(),leftPreviousOutput.begin()+buffer.getNumSamples());
        }else if(samplesInpreviousOutput>0){
            leftPreviousOutput.erase(leftPreviousOutput.begin(),leftPreviousOutput.begin()+samplesInpreviousOutput);
        }
        
        leftChannelVariables.outputRms = buffer.getRMSLevel(0,0,buffer.getNumSamples());
        
    }

}

//==============================================================================
bool HrtfPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* HrtfPluginAudioProcessor::createEditor()
{
    return new HrtfPluginAudioProcessorEditor (*this);

}

//==============================================================================
void HrtfPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HrtfPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HrtfPluginAudioProcessor();
}

void HrtfPluginAudioProcessor::setAzimuth(int channel, float value){
     switch(channel){
        case 0:
            *leftChannelVariables.azimuth=value;
        break;
        case 1:
            *rightChannelVariables.azimuth=value;
        break;
    }

}

float HrtfPluginAudioProcessor::getAzimuth(int channel){
    switch(channel){
        case 0:
            return leftChannelVariables.azimuth->get();
        break;
        case 1:
            return rightChannelVariables.azimuth->get();
        break;
    }
}

int HrtfPluginAudioProcessor::getHrirIndex(int channel){
   // DBG( ((360-(int)azimuth->get())/15*2)%hrir.size());
    switch(channel){
        case 0:
            return ((360-(int)leftChannelVariables.azimuth->get()+360)/15*2)%hrir.size();
        break;
        case 1:
            return ((360-(int)rightChannelVariables.azimuth->get()+360)/15*2)%hrir.size();
        break;
    }
}

void HrtfPluginAudioProcessor::setMute(int channel, bool value){
    switch(channel){
        case 0:
            leftChannelVariables.isMute = value;
        break;
        case 1:
            rightChannelVariables.isMute = value;
        break;
    }
}

float HrtfPluginAudioProcessor::getRms(int channel){
    switch(channel){
        case 0:
            return leftChannelVariables.outputRms;
        case 1:
            return rightChannelVariables.outputRms;
        break;
    }
}

void HrtfPluginAudioProcessor::resampleHRIR(double targetSampleRate){

    int srcError = 0;
    SRC_STATE* srcState = src_new (SRC_SINC_BEST_QUALITY, 1, &srcError) ;

    SRC_DATA srcData;
    srcData.src_ratio = hrirSampleRate/targetSampleRate; //must be input sf / output sf
    srcData.end_of_input = 1;
    srcData.input_frames = hrirLength;
    srcData.output_frames = std::floor(hrirLength*targetSampleRate/hrirSampleRate);
    //DBG(srcData.output_frames);

    int numOfOutFrames = (int)srcData.output_frames;
    vector<float> tempOutput;
    vector<vector<float>>::iterator iter;

    for(iter = hrir.begin(); iter!= hrir.end(); iter++){
        tempOutput = vector<float>(numOfOutFrames);
        srcData.data_out = tempOutput.data();
        srcData.data_in = (*iter).data();
        src_process(srcState,&srcData);
        (*iter).swap(tempOutput);
        src_reset(srcState); // reset the src so previous data wont be used for the next hrir
       DBG((int)(*iter).size());
    }

    hrirLength = srcData.output_frames;
    hrirSampleRate = targetSampleRate;
    //DBG(hrirSampleRate);
    src_delete (srcState) ;
}


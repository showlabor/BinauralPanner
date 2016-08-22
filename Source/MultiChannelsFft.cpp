/*
  ==============================================================================

    MultiChannelsFft.cpp
    Created: 15 Aug 2016 11:32:40pm
    Author:  Wing Sang Wong

  ==============================================================================
  TODO

  free plan locally and create it dynamically


*/

#include "MultiChannelsFft.h"

MultiChannelsFft::MultiChannelsFft(){


}

MultiChannelsFft::MultiChannelsFft(int channels,int newFftSize){
    resizeChannels(channels);
    resizeComplexArrays(newFftSize);
}

MultiChannelsFft::~MultiChannelsFft(){
/*
    for(int ch = 0 ; ch < numOfChannels; ch++){
        fftwf_destroy_plan(inputFFTs[ch]);
        fftwf_destroy_plan(leftHrirFFTs[ch]);
        fftwf_destroy_plan(rightHrirFFTs[ch]);
    }
*/

   // if(outRightIFFT)fftwf_destroy_plan(outRightIFFT);
   // fftwf_cleanup();

}

void MultiChannelsFft::resizeComplexArrays(int newFftSize){

    if(fftSize != newFftSize){
        fftSize = newFftSize;

        outLeft.assign(fftSize, complex<float>(0,0));
        outRight.assign(fftSize, complex<float>(0,0));
        //outLeftIFFT =  fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(outLeft.data()),reinterpret_cast<fftwf_complex*>(outLeft.data()),FFTW_BACKWARD, FFTW_ESTIMATE);
        //outRightIFFT = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(outRight.data()),reinterpret_cast<fftwf_complex*>(outRight.data()),FFTW_BACKWARD, FFTW_ESTIMATE);

        for(int ch = 0; ch< numOfChannels; ch++){
            inputSignals[ch].assign(fftSize,complex<float>(0,0));
            leftHRIRs[ch].assign(fftSize,complex<float>(0,0));
            rightHRIRs[ch].assign(fftSize,complex<float>(0,0));
           // inputFFTs[ch] = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(inputSignals[ch].data()),reinterpret_cast<fftwf_complex*>(inputSignals[ch].data()),FFTW_FORWARD, FFTW_ESTIMATE);
           // leftHrirFFTs[ch] = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(leftHRIRs[ch].data()),reinterpret_cast<fftwf_complex*>(leftHRIRs[ch].data()),FFTW_FORWARD, FFTW_ESTIMATE);
            //rightHrirFFTs[ch] = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(rightHRIRs[ch].data()),reinterpret_cast<fftwf_complex*>(rightHRIRs[ch].data()),FFTW_FORWARD, FFTW_ESTIMATE);
        }
    }

}

void MultiChannelsFft::resizeChannels(int channels){

    if( numOfChannels !=channels){
        numOfChannels = channels;
        inputSignals.resize(numOfChannels);
        leftHRIRs.resize(numOfChannels);
        rightHRIRs.resize(numOfChannels);
        //inputFFTs.resize(numOfChannels);
        //leftHrirFFTs.resize(numOfChannels);
        //rightHrirFFTs.resize(numOfChannels);
    }

}

void MultiChannelsFft::copyInput(int channel, const float* inputSignal, int bufferSize){

    int inputIndex = 0;
    for( vector<complex<float>>::iterator inputIt = inputSignals[channel].begin();inputIt != inputSignals[channel].end(); inputIt++){
        if(inputIndex < bufferSize){
            inputIt->real(inputSignal[inputIndex]);
            inputIt->imag(0);
        }else{
            (*inputIt) = complex<float>(0,0);
        }
        inputIndex++;
    }
}

void MultiChannelsFft::copyHRIR(int channel, const struct hrirData data){

    leftHRIRs[channel].assign(leftHRIRs[channel].size(),complex<float>(0,0));
    rightHRIRs[channel].assign(rightHRIRs[channel].size(),complex<float>(0,0));

    vector<float>::iterator leftHrirIt, interpLeftHrirIt, rightHrirIt, interpRightHrirIt;
    vector<complex<float>>::iterator channelLeftHrirIt, channelRightHrirIt;

    for(leftHrirIt = data.hrirBegin[MultiChannelsFft::leftHRIR],
        interpLeftHrirIt = data.hrirBegin[MultiChannelsFft::interpLeftHRIR],
        channelLeftHrirIt = leftHRIRs[channel].begin(),
        rightHrirIt = data.hrirBegin[MultiChannelsFft::rightHRIR],
        interpRightHrirIt = data.hrirBegin[MultiChannelsFft::interpRightHRIR],
        channelRightHrirIt = rightHRIRs[channel].begin();
        leftHrirIt != data.hrirEnd[MultiChannelsFft::leftHRIR];
        leftHrirIt++, interpLeftHrirIt++, channelLeftHrirIt++,
        rightHrirIt++, interpRightHrirIt++, channelRightHrirIt++
        ){
            channelLeftHrirIt->real((*leftHrirIt)*data.floorAmp + (*interpLeftHrirIt)*data.ceilAmp);
            channelRightHrirIt->real((*rightHrirIt)*data.floorAmp + (*interpRightHrirIt)*data.ceilAmp);
        }

}
void MultiChannelsFft::transformHrir(int channel){
    fftwf_plan leftHrirPlan = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(leftHRIRs[channel].data()),reinterpret_cast<fftwf_complex*>(leftHRIRs[channel].data()),FFTW_FORWARD, FFTW_ESTIMATE);
    fftwf_plan rightHrirPlan = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(rightHRIRs[channel].data()),reinterpret_cast<fftwf_complex*>(rightHRIRs[channel].data()),FFTW_FORWARD, FFTW_ESTIMATE);
    fftwf_execute(leftHrirPlan);
    fftwf_execute(rightHrirPlan);
    fftwf_destroy_plan(leftHrirPlan);
    fftwf_destroy_plan(rightHrirPlan);
}
void MultiChannelsFft::transformInput(int channel){
    fftwf_plan inputPlan = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(inputSignals[channel].data()),reinterpret_cast<fftwf_complex*>(inputSignals[channel].data()),FFTW_FORWARD, FFTW_ESTIMATE);
    fftwf_execute(inputPlan);
    fftwf_destroy_plan(inputPlan);
}
void MultiChannelsFft::transformOutputs(){
    fftwf_plan leftOutPlan = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(outLeft.data()),reinterpret_cast<fftwf_complex*>(outLeft.data()),FFTW_BACKWARD, FFTW_ESTIMATE);
    fftwf_plan rightOutPlan = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(outRight.data()),reinterpret_cast<fftwf_complex*>(outRight.data()),FFTW_BACKWARD, FFTW_ESTIMATE);
    fftwf_execute(leftOutPlan);
    fftwf_execute(rightOutPlan);
    fftwf_destroy_plan(leftOutPlan);
    fftwf_destroy_plan(rightOutPlan);
}
void MultiChannelsFft::convolveOneChannel(int channel){
    vector<complex<float>>::iterator leftOutIt, rightOutIt, inputIt, leftHrirIt, rightHrirIt;
    for(
    leftOutIt = outLeft.begin(), rightOutIt = outRight.begin(),
    inputIt = inputSignals[channel].begin(),
    leftHrirIt = leftHRIRs[channel].begin(), rightHrirIt = rightHRIRs[channel].begin();
    leftOutIt!=outLeft.end();
    leftOutIt++, rightOutIt++, inputIt++, leftHrirIt++, rightHrirIt++){
        (*leftOutIt) = (*inputIt)*(*leftHrirIt);
        (*rightOutIt) = (*inputIt)*(*rightHrirIt);
    }
}
void MultiChannelsFft::convolveAll(){
    vector<complex<float>>::iterator leftOutIt, rightOutIt, leftInputIt, rightInputIt,
    leftChannelLeftHrirIt, leftChannelRightHrirIt,
    rightChannelLeftHrirIt, rightChannelRightHrirIt;
    for(
    leftOutIt = outLeft.begin(), rightOutIt = outRight.begin(),
    leftInputIt = inputSignals[0].begin(), rightInputIt = inputSignals[1].begin(),
    leftChannelLeftHrirIt = leftHRIRs[0].begin(), leftChannelRightHrirIt = rightHRIRs[0].begin(),
    rightChannelLeftHrirIt = leftHRIRs[1].begin(), rightChannelRightHrirIt = rightHRIRs[1].begin();
    leftOutIt!=outLeft.end();
    leftOutIt++, rightOutIt++, leftInputIt++, rightInputIt++,
    leftChannelLeftHrirIt++, leftChannelRightHrirIt++,
    rightChannelLeftHrirIt++, rightChannelRightHrirIt++){
        (*leftOutIt) = (*leftInputIt)*(*leftChannelLeftHrirIt) + (*rightInputIt)*(*rightChannelLeftHrirIt);
        (*rightOutIt) = (*leftInputIt)*(*leftChannelRightHrirIt) + (*rightInputIt)*(*rightChannelRightHrirIt);
      // (*leftOutIt) = (*leftInputIt) + (*rightInputIt);
      // (*rightOutIt) = (*leftInputIt) + (*rightInputIt);

    }
}

float MultiChannelsFft::getOutput(int channel, int idx){
    switch(channel){
        case 0:
            return outLeft[idx].real()/fftSize;
            break;
        case 1:
            return outRight[idx].real()/fftSize;
            break;
        default:
            return 0;
    }
}

int MultiChannelsFft::getFftSize(){
    return fftSize;
}

void MultiChannelsFft::clearOutputs(){
    outLeft.assign(outLeft.size(),complex<float>(0,0));
    outRight.assign(outLeft.size(),complex<float>(0,0));
}

float MultiChannelsFft::getHrirDebug(int channel, int idx){
    return leftHRIRs[channel].size();
}

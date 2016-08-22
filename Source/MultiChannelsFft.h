/*
  ==============================================================================

    MultiChannelsFft.h
    Created: 15 Aug 2016 11:32:40pm
    Author:  Wing Sang Wong

  ==============================================================================
*/

#ifndef MULTICHANNELSFFT_H_INCLUDED
#define MULTICHANNELSFFT_H_INCLUDED

#include <fftw3.h>
#include <vector>
#include <iterator>
#include <complex>

using namespace std;

class MultiChannelsFft{
    public:
    MultiChannelsFft();
    MultiChannelsFft(int channels, int newFftSize);
    ~MultiChannelsFft();
    void resizeComplexArrays(int newFftSize);
    void resizeChannels(int channels);
    void copyInput(int channel, const float* inputSignal, int bufferSize);
    void transformHrir (int channel);
    void transformInput (int channel);
    void transformOutputs ();
    void convolveOneChannel(int channel);
    void convolveAll();
    float getOutput(int channel, int idx);
    int getFftSize();
    void clearOutputs();
    float getHrirDebug(int ch, int idx);



    enum HrirID {leftHRIR = 0, rightHRIR, interpLeftHRIR, interpRightHRIR };
    struct hrirData{
        float floorAmp, ceilAmp;
        vector<float>::iterator hrirBegin [4];
        vector<float>::iterator hrirEnd [4];
    };
    void copyHRIR(int channel, const struct hrirData data);

    private:
    int fftSize = 1024;
    int numOfChannels = 2;



    vector<vector<complex<float>>> inputSignals = vector<vector<complex<float>>>(numOfChannels,vector<complex<float>>(fftSize,complex<float>(0,0)));
    vector<vector<complex<float>>> leftHRIRs = vector<vector<complex<float>>>(numOfChannels,vector<complex<float>>(fftSize,complex<float>(0,0)));
    vector<vector<complex<float>>> rightHRIRs = vector<vector<complex<float>>>(numOfChannels,vector<complex<float>>(fftSize,complex<float>(0,0)));
    vector<complex<float>> outLeft = vector<complex<float>>(fftSize,complex<float>(0,0));
    vector<complex<float>> outRight = vector<complex<float>>(fftSize,complex<float>(0,0));
/*
    fftwf_plan outLeftIFFT =  fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(outLeft.data()),reinterpret_cast<fftwf_complex*>(outLeft.data()),FFTW_BACKWARD, FFTW_ESTIMATE);

    fftwf_plan outRightIFFT = fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(outRight.data()),reinterpret_cast<fftwf_complex*>(outRight.data()),FFTW_BACKWARD, FFTW_ESTIMATE);

    vector<fftwf_plan> inputFFTs = vector<fftwf_plan>{
        fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(inputSignals[0].data()),reinterpret_cast<fftwf_complex*>(inputSignals[0].data()),FFTW_FORWARD, FFTW_ESTIMATE),
        fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(inputSignals[1].data()),reinterpret_cast<fftwf_complex*>(inputSignals[1].data()),FFTW_FORWARD, FFTW_ESTIMATE)
    };
    vector<fftwf_plan> leftHrirFFTs = vector<fftwf_plan>{
        fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(leftHRIRs[0].data()),reinterpret_cast<fftwf_complex*>(leftHRIRs[0].data()),FFTW_FORWARD, FFTW_ESTIMATE),
        fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(leftHRIRs[1].data()),reinterpret_cast<fftwf_complex*>(leftHRIRs[1].data()),FFTW_FORWARD, FFTW_ESTIMATE)
    };
    vector<fftwf_plan> rightHrirFFTs = vector<fftwf_plan>{
        fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(rightHRIRs[0].data()),reinterpret_cast<fftwf_complex*>(rightHRIRs[0].data()),FFTW_FORWARD, FFTW_ESTIMATE),
        fftwf_plan_dft_1d(fftSize, reinterpret_cast<fftwf_complex*>(rightHRIRs[1].data()),reinterpret_cast<fftwf_complex*>(rightHRIRs[1].data()),FFTW_FORWARD, FFTW_ESTIMATE)
    };
*/


};



#endif  //  MULTICHANNELSFFT_H_INCLUDED

/*
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along with this program;
 * if not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PAWRAPPER_H
#define PAWRAPPER_H

#include "portaudio.h"
#include <QString>
#include <iostream>

using namespace std;

class paWrapper
{
public:
    paWrapper();
    ~paWrapper();
    void initAudio();
    int startAudio();
    int stopAudio();
    int runOnce();

    bool isOutputDevice(int deviceIndex);
    bool isInputDevice(int deviceIndex);

    //wrapper for the host api
    inline int getApiCount()
    {
        return Pa_GetHostApiCount();
    }

    inline const char* getApiName(int apiIndex)
    {
        return Pa_GetHostApiInfo(apiIndex)->name;
    }

    inline int getHostApi()
    {
        return selectedHostApi;
    }

    inline int setHostApi()
    {
        return selectedHostApi;
    }

    //wrapper for devices
    inline int getDeviceCount()  { return Pa_GetDeviceCount();  }
    const char* getDeviceName(int deviceIndex)
    {
        QString name;
        name = Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceIndex)->hostApi)->name;
        name += ": ";
        name += Pa_GetDeviceInfo(deviceIndex)->name;
        return name.toLatin1();
    }

    inline int getOutputDevice()
    {
        return outputStreamParam.device;
    }

    int setOutputDevice(int deviceIndex);


    inline int getInputDevice()
    {
        return inputStreamParam.device;
    }

    inline int setInputDevice(int deviceIndex)
    {
    if(deviceIndex < Pa_GetDeviceCount())
    {
        inputStreamParam.device = deviceIndex;
        return 0;
    }
    else
        return -1;
    }

    //wrapper for sample rates
    inline int isSampleRateSupported(double sampleRate)
    {
        return Pa_IsFormatSupported(&inputStreamParam,&outputStreamParam,sampleRate);
    }

    inline double getSampleRate()
    {
        return sampleRate;
    }

    inline int setSampleRate(double sampleRate)
    {
        int err=isSampleRateSupported(sampleRate);
        if(err < 0)
            return err;

        else
        {
            this->sampleRate = sampleRate;
            return 0;
        }
    }

    inline double getDefaultSampleRate(int deviceIndex)
    {
        return Pa_GetDeviceInfo(deviceIndex)->defaultSampleRate;
    }

    //wrapper for frame length
    inline void setFrameLength(int frameLength)
    {
        this->frameLength = frameLength;
    }

    inline int getFrameLength()
    {
        return frameLength;
    }

    //misc wrapper methods
    const char* getErrorText(int errorId);

enum PawErrorCode
{
  pawErrorAudioIsRunning = -100
} pawErrorCode;

protected:
  PaTime suggestedLatency;

  PaHostApiIndex selectedHostApi;
  PaStreamParameters inputStreamParam;
  PaStreamParameters outputStreamParam;
  double sampleRate;
  int frameLength;
  bool isRunning; //flag signaling that the audio processing is running or not

  //data processing callback method
  virtual int processingCallback(const void *inputBuffer,
                                 void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags);

  //portaudio need a static member function as callback,
  //this calls the non-static processing callback method by passing the object via userData
  static int paStaticCallback(const void *inputBuffer,
                              void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData)
  {
    return ((paWrapper*)userData)->processingCallback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
  }

};


#endif // PAWRAPPER_H

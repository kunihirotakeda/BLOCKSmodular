/*
 Util.h for BLOCKSmodular
 Created by Akiyuki Okayasu
 License: GPLv3
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <Bela.h>
#include <sndfile.h>// to load audio files
#include <string>
#include <iostream>
#include <cstdlib>
#include <atomic>

static constexpr float Pi = 3.14159265359f;
static constexpr float twoPi = 6.28318530718f;
static constexpr float halfPi = 1.57079632679f;

static inline int getNumChannels(std::string file)
{
    SNDFILE *sndfile ;
    SF_INFO sfinfo ;
    sfinfo.format = 0;
    if (!(sndfile = sf_open (file.c_str(), SFM_READ, &sfinfo)))
    {
        std::cout << "Couldn't open file " << file << ": " << sf_strerror(sndfile) << std::endl;
        return -1;
    }
    return sfinfo.channels;
}

static inline int getNumFrames(std::string file)
{
    SNDFILE *sndfile ;
    SF_INFO sfinfo ;
    sfinfo.format = 0;
    if (!(sndfile = sf_open (file.c_str(), SFM_READ, &sfinfo)))
    {
        std::cout << "Couldn't open file " << file << ": " << sf_strerror(sndfile) << std::endl;
        return -1;
    }
    return sfinfo.frames;
}

class HighResolutionControlChange {
public:
    void set(char v, bool isUpperByte) {
        if(isUpperByte) {
            up = v;
        }else{
            low = v;
        }
    }
    
    bool update() {
        if(up < 0 || low < 0) {
            return false;
        }
        else {
            value = (float)((up << 7) | (low & 0b1111111)) / 16384.0f;
            up = -1;
            low = -1;
            return true;
        }
    }
    
    float get() {
        return value;
    }
    
private:
    std::atomic<int> up{-1};
    std::atomic<int> low{-1};
    float value = 0.0f;
};

class Smoothing
{
public:
    Smoothing(){}
    ~Smoothing(){}
    
    void set(const float target)
    {
        stepSize = (target - currentValue.load()) / (float)smoothingLength;
        index = 0;
    }
    
    float getNextValue()
    {
        if (index.load() < smoothingLength)
        {
            currentValue = currentValue.load() + stepSize.load();
            index++;
        }
        return currentValue;
    }
    
private:
    std::atomic<float> currentValue{0.0f};
    std::atomic<float> stepSize{0.0f};
    std::atomic<int> index{0};
    static constexpr int smoothingLength = 1200;
};

#endif//Util.h

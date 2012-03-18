#pragma once
#include "ofMain.h"

#define CUBE_SCREEN_WIDTH 1024
#define CUBE_SCREEN_HEIGHT 768
#define CAMERA_PROJECTION_SCREEN_WIDTH 1280
#define CAMERA_PROJECTION_SCREEN_HEIGHT 1024

#define CAM_WIDTH 960
#define CAM_HEIGHT 720
#define USE_FX

#define NUM_AUDIO_CHANNELS 6

class AppAssets {
    
private:
    static AppAssets * instance;
public:
    
    ~AppAssets();
    static AppAssets * inst();
    ofTrueTypeFont     appFont;
    
    
};

//
//  DrawScene.h
//  MainApp
//
//  Created by Todd Vanderlin on 2/28/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#pragma once
#include "BaseScene.h"
#include "ofxBox2d.h"
#include "Viner.h"
#include "Particle.h"
#include "VectorField.h"

#include "rxSpring.h"
#include "rxParticles.h"
#include "rxParticle.h"
// Particles test
#ifdef USE_SWIRPS
rxParticles particles;
float repel_effect;
float follow_effect;
#endif


// ----------------------------------------------------
class Chaser : public Particle {
public:
    Chaser() {
        radius       = ofRandom(10, 40);
        nConnections = 0;
        rotation = 0;
    }
    int nConnections;
    
    float rotation;
    
};



// ----------------------------------------------------
class VinePoop : public ofxBox2dCircle {
public:
    VinePoop() {
        dotSize = 1;
        dotSizeD = 1;
        img = NULL;
        birthdate = ofGetElapsedTimef();
        bFade =false;
        alpha = 255;
    }
    
    void updateColor() {
        color.r += (colorD.r - color.r) * 0.2;
        color.g += (colorD.g - color.g) * 0.2;
        color.b += (colorD.b - color.b) * 0.2;
    }
  
    bool bFade;
    float alpha;
    float birthdate;
    float dotSize, dotSizeD;
    ofImage * img;
    ofColor color, colorD;    
};


// ----------------------------------------------------
class DrawnScene : public BaseScene {
    
private:
protected:
public:
    
    ofxBox2d        box2d;
    
    ofxFloatSlider  damping;
    ofxFloatSlider  frequency;
    
    void setup();
    void update();
    void draw(); 
	virtual void drawGui();
    void keyPressed(int key);
    void mousePressed(int x, int y, int button);
    void mouseMoved(int x, int y );
	void handleCommands(TwitterCommand& cmd, Effects& fx);   
    void respondToNewComplimentaryColours();
    void enterScene(){
        generateComplimentaryColours();
        respondToNewComplimentaryColours();
        ofRegisterMouseEvents(&gui);
    }
    
    VectorField field;
    
    // chasers and the stuff that comes out of them
    void makePoop();
    void addChasers();
    void addBush(float startX);
    vector <Chaser>     chasers;
    vector <VinePoop>   poop;
    vector <Viner>      vines;
    vector <Viner*>     poopVines;
    
    // images
    vector <ofImage> tris;
    vector <ofImage> dots;
    ofImage          eyeA;
    float            eyeRatio;
    
    // Particles test
#ifdef USE_SWIRPS
	rxParticles particles;
	float repel_effect;
	float follow_effect;
#endif
  
};
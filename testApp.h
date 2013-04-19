#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

#include "OscSend.h"

//#include "OscReceive.h"

#include "ofxCvMain.h"

#include "ofVbo.h"

#include <pthread.h>

#define NUM_BILLBOARDS 300

#define NUM_PAOPAO 5000

#define NUM_THREADS 1

#define _USE_LIVE_VIDEO  

#define W 1024
#define H 768


class testApp : public ofBaseApp{
    
    public:
        void setup();
        void update();
        void draw();
        int highb(vector<int> x);
        int lowb(vector<int> x);
        int center(vector<int> x);
        int minindex(vector<float> x);
        int exist(vector<int> x, vector<int> y, int vx, int vy);
        void filter(vector<int> x, vector<int> y);
        void cluster(vector<int> x, vector<int> y);
        float dist(vector<int> x, vector<int> y);
        void drawFbo();
        void keyPressed  (int key);

        ofVideoGrabber			vidGrabber;
        ofxCvColorImage		    colorImg;
        ofxCvColorImage		    colorImgHSV;
    
        ofxCvGrayscaleImage		hueImg;
        ofxCvGrayscaleImage		satImg;
        ofxCvGrayscaleImage		valImg;
    
        ofImage                 calder;
    
        unsigned char *         colorTrackedPixels;
        ofTexture               trackedTexture;
    
        vector<int> px;
        vector<int> py;
    
        vector<int> cx;
        vector<int> cy;
    
        int minHue, maxHue, minSat, maxSat, minVal, maxVal;
        float hue, hueWidth, sat, satWidth, val, valWidth;

        int K = 2;
        vector<int> ux;
        vector<int> uy;
        vector<int> uxx;
        vector<int> uyy;

        int alpha=20;
        ofFbo rgbaFbo;
        int flag1 = 0;
        int flag2 = 0;
    
        int p=0;
        int xx0 = 2000;
        int yy0 = 2000;
        int xx1 = 2000;
        int yy1 = 2000;
        int xx3 = 2000;
        int yy3 = 2000;
        int xx4 = 0;
        int yy4 = 0;
        vector<int> xx2;
        vector<int> yy2;
        int ppx;
        int ppy;
        vector<int> pvx;
        vector<int> pvy;
        float a=2000;
        int flag=0;
    int cxx;
    int cyy;
    
    void addPoint(float x, float y) {
        points.push_back(ofVec2f(x, y));
        speeds.push_back(ofVec2f(ofRandom(0, 10), ofRandom(0, 10)));
    }
    vector <ofVec2f> points;
    vector <ofVec2f> speeds;
    
    
    
    
    
    ofVec3f cameraRotation;
    float zoom, zoomTarget;
    int paopao = 0;
    
    int backr = 110;
    int backg = 0;
    int backb = 220;
    
    // billboard particles
    float billboardSizeTarget[NUM_BILLBOARDS];
    float billboardSizeTargetFinal[NUM_PAOPAO];
    
    ofShader billboardShader;
    ofImage texture;
    
    ofVboMesh billboards;
    ofVboMesh billboardsFinal;
    ofVec3f billboardVels[NUM_BILLBOARDS];
    ofVec3f billboardVelsFinal[NUM_PAOPAO];
    
    int step1 = 1;
    int step2 = 0;
    int step3 = 0;
    int step4 = 0;
    int step5 = 0;

    int timer = 0;
    
};


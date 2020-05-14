#pragma once

#include "ofMain.h"
#include "ofxPlaygroundScene.h"

#include "WanderersCommon.h"

#define NUM_WANDERERS 17

class RectagonScene : public ofxPlaygroundScene {
    public:
        void setup();
        void draw();
        void drawTarget(glm::vec2 pos);
        void update();

        void drawConnectorFromGridToTarget(glm::vec2 gridPos, glm::vec2 targetPos, float seed);
        //void drawWanderer(glm::vec2 pos, glm::vec2 dir, int id);
        void wandererTurn(int wandererIndex);

        float wandererGetRandomValueForTimer();

    glm::vec2 wandererLocs[NUM_WANDERERS];
    glm::vec2 wandererDirs[NUM_WANDERERS];
    float wandererTurnTimers[NUM_WANDERERS];
    float wandererSpeeds[NUM_WANDERERS];
    bool wandererJustCrossed[NUM_WANDERERS];

    ofPolyline squareShape;
    WanderersCommon common;

    ofParameter<float> wandererDefaultSpeed;
    ofParameter<float> wandererRushingSpeed;
    ofParameter<float> wandererRepelSpeed;
    ofParameter<float> wandererRepelDistance;

    ofParameter<float> bgGridItemSize;
    ofParameter<ofColor> bgColor;
    ofParameter<ofColor> bgGridSquaresColor;
    ofParameter<ofColor> bgGridSquaresHighlightColor;
    ofParameter<ofColor> bgGridLinesColor;
    ofParameter<ofColor> targetColor;
    ofParameter<ofColor> connectorColor;
};


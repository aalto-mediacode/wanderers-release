#pragma once

#include "ofMain.h"
#include "ofxPlaygroundScene.h"

class RectagonScene : public ofxPlaygroundScene {
    public:
        void setup();
        void draw();
        void drawTarget(glm::vec2 pos);
        void update();

        void drawConnectorsFromGridToTargets(glm::vec2 pos);

    float angle;
    ofPolyline squareShape;
};


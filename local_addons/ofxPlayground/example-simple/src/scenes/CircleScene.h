#pragma once

#include "ofMain.h"
#include "ofxPlaygroundScene.h"

class CircleScene : public ofxPlaygroundScene {
    public:
        void setup();
        void draw();
        void drawTarget(glm::vec2 pos);
        void update();
};


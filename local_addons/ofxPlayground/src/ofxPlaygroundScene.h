#pragma once

#include "ofMain.h"
#include "ofxPlaygroundHelpers.h"

class ofxPlaygroundScene : public ofxPlaygroundHelpers {
    public:
        virtual void setup() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void drawTarget(glm::vec2 pos) = 0;
        virtual ~ofxPlaygroundScene() = default;

    vector <glm::vec2> targets;
    float scaleFactor;
};


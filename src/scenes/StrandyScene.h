#pragma once

#include "ofMain.h"
#include "ofxPlaygroundScene.h"

#define NUM_WANDERERS 17

class StrandyScene : public ofxPlaygroundScene {
    public:
        void setup();
        void draw();
        void drawTarget(glm::vec2 pos);
        void update();

        void windBendStrandsHorizontal(glm::vec3 & vert,
                 float windSourceHeight, float windStrength, float maxDist);
        void attractStrandsToWanderers(glm::vec3 & vert,
                 float maxDist, float snapDist, float attractionAmount);
        void repelStrandsFromTargets(glm::vec3 & vert,
                 float maxDist, float repelAmount);
        void attractWanderersToTargets(float minDist, float maxDist);
        void repelWanderersFromEachOther(float repelTresh, float repelStrength);
        void drawWanderer(glm::vec2 pos, glm::vec2 dir, int id);

    ofPolyline line;
    glm::vec2 wandererLocs[NUM_WANDERERS];
    glm::vec2 wandererDirs[NUM_WANDERERS];
    float wandererSpeeds[NUM_WANDERERS];
    float wandererDefaultSpeed;

    float angle;
};

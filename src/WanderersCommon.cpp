#include "WanderersCommon.h"

void WanderersCommon::drawWanderer(glm::vec2 pos, glm::vec2 dir, int id, float scaleFactor) {
    ofFill();
    ofSetColor(240, 240, 240, 140);
    //ofSetColor(240, 40, 40, 140);

    float wAngle = glm::angle(dir, {0,1});

    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateRad(wAngle);
    ofScale(1.0/scaleFactor);

    float wandererTimeTrack = ofGetElapsedTimef()*0.31f;

    int numSpheres = 9;
    glm::vec2 scatter = {32.0f, 46.0f};
    scatter.x = scatter.x*scaleFactor;
    scatter.y = scatter.y*scaleFactor;
    float maxRadius = 3.5f;
    for (int i=0; i<numSpheres; i++) {
        float x = ofMap(ofNoise(id*0.436+i*0.259+wandererTimeTrack),
                        0, 1, -scatter.x/2, scatter.x/2);
        float y = ofMap(ofNoise(id*0.913+i*0.831+wandererTimeTrack),
                        0, 1, -scatter.y/2, scatter.y/2);
        float radius = ofMap(ofNoise(id*0.913+wandererTimeTrack),
                        0, 1, maxRadius*0.2, maxRadius);
        ofPushMatrix();
        ofDrawCircle(x, y, radius);
        ofPopMatrix();
    }

    ofPopMatrix();
}


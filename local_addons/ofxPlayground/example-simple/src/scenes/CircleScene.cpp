#include "CircleScene.h"


void CircleScene::setup() {
    ofBackground(0);
    ofSetCircleResolution(100);
}


void CircleScene::draw() {
    int circleDiameter = 200;
    ofSetColor(240, 15, 15, 255);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, circleDiameter/2);
}


// target is displayed at mouse location when clicked
// as well as at each touch location on mobile
void CircleScene::drawTarget(glm::vec2 pos) {
    int circleDiameter = 100;
    ofSetColor(140, 15, 15, 255);
    ofDrawCircle(pos.x, pos.y, circleDiameter/2);
}


void CircleScene::update() {

}


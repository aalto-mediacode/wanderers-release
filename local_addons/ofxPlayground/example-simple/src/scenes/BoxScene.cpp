#include "BoxScene.h"


void BoxScene::setup() {

}


void BoxScene::draw() {
    int boxSize = 200;
    ofSetColor(15, 15, 240, 255);
    ofDrawRectangle(
        ofGetWidth()/2-boxSize/2, ofGetHeight()/2-boxSize/2,
        boxSize, boxSize );
}


// target is displayed at mouse location when clicked
// as well as at each touch location on mobile
void BoxScene::drawTarget(glm::vec2 pos) {
    int boxSize = 120;
    ofSetColor(15, 15, 140, 255);
    ofDrawRectangle(pos.x-boxSize/2, pos.y-boxSize/2, boxSize, boxSize);
}


void BoxScene::update() {

}


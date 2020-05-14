#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    pg.setup();

    pg.addScene( (ofxPlaygroundScene &) box );
    pg.addScene( (ofxPlaygroundScene &) circle );

    pg.setupScenes();
}

//--------------------------------------------------------------
void ofApp::draw() {
    pg.draw();
}

//--------------------------------------------------------------
void ofApp::update() {
    pg.update();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    pg.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    pg.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {
    pg.mouseMoved(x, y);
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    pg.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    pg.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    pg.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
    pg.mouseEntered(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
    pg.mouseExited(x, y);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    pg.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
    pg.gotMessage(msg);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
    pg.dragEvent(dragInfo);
}






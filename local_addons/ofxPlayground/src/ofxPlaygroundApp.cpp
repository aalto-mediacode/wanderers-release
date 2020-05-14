#include "ofxPlaygroundApp.h"

// input handling and platform
// abstraction defined in this file
//
// scene-related calls are forwarded
// to SceneManager

void ofxPlaygroundApp::setup() {
    sm.setup();
    scaleFactor = 600.0f/ofGetWidth();

    #ifdef PLAYGROUND_PLATFORM_IOS
    sceneSwitcherButtonScale = 20.0f/scaleFactor;
    #endif // PLAYGROUND_PLATFORM_IOS
}

void ofxPlaygroundApp::draw() {
    sm.draw();

    // scene switcher button
    #ifdef PLAYGROUND_PLATFORM_IOS
    ofFill();
    ofSetColor(220, 210, 220, 100);
    ofDrawCircle(
        sceneSwitcherButtonScale, sceneSwitcherButtonScale, // xy
        sceneSwitcherButtonScale); // radius
    #endif // PLAYGROUND_PLATFORM_IOS
}

void ofxPlaygroundApp::update() {
    sm.update();

    #ifdef PLAYGROUND_PLATFORM_IOS
    // TODO: there must be a nice C++11
    // lambda closure way to write this
    sm.targets.clear();
    for (auto t : touches) {
        sm.targets.push_back(t.pos);
    }
    #endif // PLAYGROUND_PLATFORM_IOS
}


void ofxPlaygroundApp::setupScenes() {
    for (auto scene : sm.scenes) {
        scene->scaleFactor = scaleFactor;
        scene->setup();
    };
}


void ofxPlaygroundApp::addScene(ofxPlaygroundScene & scene) {
    sm.addScene(scene);
}


void ofxPlaygroundApp::nextScene() {
    sm.nextScene();
}





#ifdef PLAYGROUND_PLATFORM_OSX


//--------------------------------------------------------------
void ofxPlaygroundApp::keyPressed(int key) {
    if (!sm.inSceneTransition) {
        sm.nextScene();
    }
}


//--------------------------------------------------------------
void ofxPlaygroundApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofxPlaygroundApp::mouseMoved(int x, int y ) {

}


//--------------------------------------------------------------
void ofxPlaygroundApp::mouseDragged(int x, int y, int button){
    if (sm.targets.size() > 0) {
        sm.targets[0].x = x;
        sm.targets[0].y = y;
    }
}

//--------------------------------------------------------------
void ofxPlaygroundApp::mousePressed(int x, int y, int button){
    if (sm.targets.size() == 0) {
        sm.targets.push_back({x,y});
    }
    if (sm.targets.size() > 0) {
        sm.targets[0].x = x;
        sm.targets[0].y = y;
    }
}

//--------------------------------------------------------------
void ofxPlaygroundApp::mouseReleased(int x, int y, int button){
    if (sm.targets.size() > 0) {
        sm.targets.clear();
    }
}

//--------------------------------------------------------------
void ofxPlaygroundApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofxPlaygroundApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofxPlaygroundApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofxPlaygroundApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofxPlaygroundApp::gotMessage(ofMessage msg) {

}

#endif // PLAYGROUND_PLATFORM_OSX




#ifdef PLAYGROUND_PLATFORM_IOS

void ofxPlaygroundApp::touchDown(ofTouchEventArgs & touch) {
    float buttonScale = 20.0f/scaleFactor;
    if (touch.x < buttonScale*2.0 && touch.y < buttonScale*2.0) {
        sm.nextScene();
    }

    bool found = false;
    for (auto t : touches) {
        if (t.id == touch.id) {
            t.pos = {touch.x, touch.y};
            found = true;
        }
    }
    if (!found) {
        pgTouch t = {{touch.x, touch.y}, touch.id};
        touches.push_back(t);
    }
}

void ofxPlaygroundApp::touchMoved(ofTouchEventArgs & touch) {
    for (int i=0; i<touches.size(); i++) {
        if (touches[i].id == touch.id) {
            touches[i].pos = {touch.x, touch.y};
        }
    }
}

void ofxPlaygroundApp::touchUp(ofTouchEventArgs & touch) {
    for (int i=0; i<touches.size(); i++) {
        if (touches[i].id == touch.id) {
            vector <pgTouch>::iterator it = touches.begin() + i;
            touches.erase(it);
        }
    }
}

void ofxPlaygroundApp::touchDoubleTap(ofTouchEventArgs & touch) {

}

void ofxPlaygroundApp::touchCancelled(ofTouchEventArgs & touch) {
    for (int i=0; i<touches.size(); i++) {
        if (touches[i].id == touch.id) {
            vector <pgTouch>::iterator it = touches.begin() + i;
            touches.erase(it);
        }
    }
}


void ofxPlaygroundApp::lostFocus() {

}

void ofxPlaygroundApp::gotFocus() {

}

void ofxPlaygroundApp::gotMemoryWarning() {

}

void ofxPlaygroundApp::deviceOrientationChanged(int newOrientation) {

}



#endif // PLAYGROUND_PLATFORM_IOS


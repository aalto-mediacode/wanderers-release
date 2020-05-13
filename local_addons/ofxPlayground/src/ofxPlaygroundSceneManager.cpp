#include "ofxPlaygroundSceneManager.h"


void ofxPlaygroundSceneManager::setup() {
    currentScene = -1;
    previousScene = -1;
    inSceneTransition = false;
    transitionTimeElapsed = 0.0f;
    //targets.push_back({250.0f,250.0f});
}


void ofxPlaygroundSceneManager::update() {
    if (currentScene != -1) {
        scenes[currentScene]->update();
        scenes[currentScene]->targets = targets;
    }
}


void ofxPlaygroundSceneManager::draw() {

    // TRANSITION BETWEEN SCENES = FADE TO/FROM BLACK
    //
    // if in first half of transition:
    //    draw previous scene + black semi-transp overlay
    // if in second half of transition:
    //    draw current (=new) scene + black semi-transp overlay
    // vary transparency based on transition length - timeElapsed

    if (inSceneTransition) { // transition in progress

        if (transitionTimeElapsed > transitionLength) { // transition over
            inSceneTransition = false;
            transitionTimeElapsed = 0.0f;
        } else {
            if (transitionTimeElapsed < transitionLength/2) { // first half
                drawSceneWithTargets(previousScene);
            } else { // second half
                drawSceneWithTargets(currentScene);
            }

            // relative transitionTimeElapsed, between 0..1
            float transitionTime = transitionTimeElapsed/transitionLength;
            drawTransitionBlackOverlay(transitionTime);

            transitionTimeElapsed += ofGetLastFrameTime();
        }; // inSceneTransition

    } else { // regular draw
        drawSceneWithTargets(currentScene);
    };
}


void ofxPlaygroundSceneManager::drawSceneWithTargets(int sceneIndex) {
    if (sceneIndex != -1) {
        scenes[sceneIndex]->draw();
        for (auto t : targets) {
            scenes[sceneIndex]->drawTarget(t);
        }
    }
}


void ofxPlaygroundSceneManager::drawTransitionBlackOverlay(float transitionTime) {
    // opacity based on transition time [0% .. 100% .. 0%]
    ofFill();
    ofSetColor(0, ( 1.0f - glm::abs(transitionTime - 0.5f) * 2.0f ) * 255.0f);
    ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );
}


void ofxPlaygroundSceneManager::addScene(ofxPlaygroundScene & scene) {
    scenes.push_back(&scene);
    currentScene = scenes.size()-1;
}


void ofxPlaygroundSceneManager::nextScene() {
    if (scenes.size() > 0) {
        previousScene = currentScene;
        inSceneTransition = true;
        if (currentScene < (scenes.size()-1) ) {
            currentScene++;
        } else {
            currentScene = 0;
        }
    }
}


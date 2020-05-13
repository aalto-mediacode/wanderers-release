#pragma once

#include "ofMain.h"

#include "ofxPlaygroundDetectPlatform.h"
#include "ofxPlaygroundScene.h"

class ofxPlaygroundSceneManager {
    public:
        void setup();
        void update();
        void draw();
        void drawSceneWithTargets(int sceneIndex);
        void drawTransitionBlackOverlay(float transitionTime);

        void addScene(ofxPlaygroundScene & scene);
        void nextScene();

        bool inSceneTransition;

    vector <ofxPlaygroundScene*> scenes;
    vector <glm::vec2> targets;

    int currentScene;
    int previousScene;
    float transitionTimeElapsed;
    const float transitionLength = 0.75f; // seconds

};

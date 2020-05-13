#pragma once

#include "ofMain.h"

#include "ofxPlaygroundDetectPlatform.h"
#include "ofxPlaygroundSceneManager.h"
#include "ofxPlaygroundScene.h"

#ifdef PLAYGROUND_PLATFORM_IOS // iOS/mobile specific
struct pgTouch {
    glm::vec2 pos;
    int id;
};
#endif // iOS/mobile specific


class ofxPlaygroundApp {

    public:
        // all platforms
        void setup();
        void update();
        void draw();
        void exit();

        // scene management
        void setupScenes();
        void addScene(ofxPlaygroundScene & scene);
        void nextScene();

        #ifdef PLAYGROUND_PLATFORM_OSX // OSX/desktop specific
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        #endif // OSX/desktop specific


    #ifdef PLAYGROUND_PLATFORM_IOS // iOS/mobile specific
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);

    vector <pgTouch> touches;
    float sceneSwitcherButtonScale;
    #endif // iOS/mobile specific

    float scaleFactor;
    ofxPlaygroundSceneManager sm;
};

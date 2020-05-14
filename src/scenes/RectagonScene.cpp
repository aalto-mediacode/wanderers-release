#include "RectagonScene.h"


void RectagonScene::setup() {
    bgGridItemSize = 68.0f/(ofMap(scaleFactor, 0.3, 1, 0.6, 1));

    wandererDefaultSpeed = 1.55f;
    wandererRushingSpeed = 5.55f;
    wandererRepelSpeed = 15.55f;
    wandererRepelDistance = bgGridItemSize*0.8;

    bgColor = ofColor(32, 32, 32, 255);
    bgGridSquaresColor = ofColor(150, 150, 150, 80);
    bgGridSquaresHighlightColor = ofColor(240, 145, 20, 140);
    float gridLineLightness = 8.15;
    bgGridLinesColor = ofColor(
        bgColor->r + gridLineLightness,
        bgColor->g + gridLineLightness,
        bgColor->b + gridLineLightness, 255);
    targetColor = ofColor(160.0f, 240.0f, 0.0f, 255.0f);
    connectorColor = ofColor(240, 190, 70, 25);


    for (int i=0; i<NUM_WANDERERS; i++) {
        // LOCATIONS: RANDOM
        //wandererLocs[i] = {ofRandomWidth(),ofRandomHeight()};

        // LOCATIONS: ALONG GRID LINES
        wandererLocs[i] = {
            glm::floor( ofRandomf() * ofGetWidth()/bgGridItemSize ) * bgGridItemSize,
            glm::floor( ofRandomf() * ofGetHeight()/bgGridItemSize ) * bgGridItemSize };

        // random directions: up, down, left, right
        wandererDirs[i] = {-1,0};
        float seed = ofMap( ofNoise( (i*1.0)*14.5 ), 0, 1, 1, 0);
        if      (seed > 0.0   && seed < 0.333) { wandererDirs[i] = {1,0};  }
        else if (seed > 0.333 && seed < 0.666) { wandererDirs[i] = {0,-1}; }
        else if (seed > 0.666 && seed < 1.0)   { wandererDirs[i] = {0,1};  }
        else {}

        wandererTurnTimers[i] = wandererGetRandomValueForTimer();

        // NOT WORKING FOR NOW DUE TO GETTING OVERRIDDEN IN UPDATE
        //wandererSpeeds[i] = wandererDefaultSpeed + ofMap( ofRandomf(), 0, 1, -1, 1)*0.4;

        wandererSpeeds[i] = wandererDefaultSpeed;
        wandererJustCrossed[i] = false;
    }

    squareShape = generateSubidividedSquare(10); // from PlaygroundScene
    for (auto &vert : squareShape.getVertices()) {
        vert.x *= 0.1;
        vert.y *= 0.1;
    }
}


void RectagonScene::draw() {
    // just a circle to see that drawing works
    /*ofFill();
    ofSetColor(220, 210, 220, 100);
    ofDrawCircle(180.0f, 180.0f, 68.0f);*/
    ofBackground(bgColor);

    // BACKGROUND GRID

    for (int i=0; i<ofGetWidth()/bgGridItemSize; i++) {
        for (int j=0; j<ofGetHeight()/bgGridItemSize; j++) {

            glm::vec2 pos = {
                bgGridItemSize/2 + i*bgGridItemSize, // x
                bgGridItemSize/2 + j*bgGridItemSize}; // y
            ofColor col = bgGridSquaresColor;

            float seed = 859 * (85.0 + ofNoise(i*44.28)*0.349 + ofNoise(j*19.83)*0.196);
            float bgGridTimeTrack = ofGetElapsedTimeMillis()*0.0014f;
            float size = bgGridItemSize*0.73
                * ofMap( ofNoise(bgGridTimeTrack+seed), 0, 1, 0.5, 1.0 ) // jiggle
                * ofMap( ofNoise(seed), 0, 1, 0.0022, 1.0); // large-scale variation

            // highlight grid squares near targets
            float highlightTimeTrack = ofGetElapsedTimef()*0.5;
            float distToNearestTarget = 99999.9;
            for (auto t : targets) {
                float dist = glm::distance(pos, {t.x, t.y});
                if (dist < distToNearestTarget) {
                    distToNearestTarget = dist;
                }
            }
            if (distToNearestTarget < 90.0/scaleFactor && ofNoise(highlightTimeTrack + (i+100)*(j+30)*0.3) < 0.5) {
                col = bgGridSquaresHighlightColor;
            }

            ofPushMatrix();
            ofTranslate(pos.x, pos.y);
            this->drawConcentric(squareShape,
                size, col, 0.5f /* lineWidth */ );
            ofPopMatrix();

            int t_id = 0;
            for (auto t : targets) {
                if (ofNoise(seed*0.134 + (t_id*1.0)*0.378) < 0.28) {
                    drawConnectorFromGridToTarget(pos, {t.x, t.y}, seed);
                }
                t_id++;
            }
        }
    }
    // END BACKGROUND GRID



    // LINES BETWEEN GRID SQUARES
    ofSetColor(bgGridLinesColor);
    ofSetLineWidth(0.85);
    float gridLinesTimeTrack = ofGetElapsedTimef()*0.085;

    for (int i=0; i<ofGetWidth()/bgGridItemSize; i++) { // VERTICAL LINES
        float gridLineBaseLength = ofGetHeight()*0.135;
        float lineStartRand = ofMap( ofNoise(i*843.1693), 0, 1, 0, ofGetHeight() );
        float lineLengthRand = ofMap( ofNoise(i*434.9316), 0, 1, 0, ofGetHeight()/4 );
        float lineLength = gridLineBaseLength + lineLengthRand;
        float speedRandAmount = 0.65;
        float lineSpeedRand = ofMap( ofNoise(i*611.9183), 0, 1, 1-speedRandAmount/2, 1+speedRandAmount/2 );
        float lineStart = ofWrap( gridLinesTimeTrack*ofGetHeight()*lineSpeedRand+lineStartRand, 0, ofGetHeight() );
        //float lineStart = ofWrap( gridLinesTimeTrack*ofGetHeight(), 0, ofGetHeight() );
        float lineEnd = lineStart + lineLength;

        float lineEndWrapped = ofWrap(lineEnd, 0, ofGetHeight());
        if ( glm::abs( lineEnd - lineEndWrapped ) < 0.01 ) { // line end does not wrap
            ofDrawLine({1.0*i*bgGridItemSize, lineStart}, {1.0*i*bgGridItemSize, lineEnd});
        } else { // line end wraps so draw line in two pieces
            ofDrawLine({1.0*i*bgGridItemSize, lineStart}, {1.0*i*bgGridItemSize, ofGetHeight()});
            ofDrawLine({1.0*i*bgGridItemSize, 0}, {1.0*i*bgGridItemSize, lineEndWrapped});
        }
    }

    for (int j=0; j<ofGetHeight()/bgGridItemSize; j++) { // HORIZONTAL LINES
        float gridLineBaseLength = ofGetWidth()*0.935;
        float lineStartRand = ofMap( ofNoise(j*843.1693), 0, 1, 0, ofGetWidth() );
        float lineLengthRand = ofMap( ofNoise(j*434.9316), 0, 1, 0, ofGetWidth()/4 );
        float lineLength = gridLineBaseLength - lineLengthRand;
        float speedRandAmount = 0.65;
        float lineSpeedRand = ofMap( ofNoise(j*611.9183), 0, 1, 1-speedRandAmount/2, 1+speedRandAmount/2 );
        float lineStart = ofWrap( gridLinesTimeTrack*ofGetWidth()*lineSpeedRand+lineStartRand, 0, ofGetWidth() );
        //float lineStart = ofWrap( gridLinesTimeTrack*ofGetHeight(), 0, ofGetHeight() );
        float lineEnd = lineStart + lineLength;

        float lineEndWrapped = ofWrap(lineEnd, 0, ofGetWidth());
        if ( glm::abs( lineEnd - lineEndWrapped ) < 0.01 ) { // line end does not wrap
            ofDrawLine({lineStart, 1.0*j*bgGridItemSize}, {lineEnd, 1.0*j*bgGridItemSize});
        } else { // line end wraps so draw line in two pieces
            ofDrawLine({lineStart, 1.0*j*bgGridItemSize}, {ofGetWidth(), 1.0*j*bgGridItemSize});
            ofDrawLine({0, 1.0*j*bgGridItemSize}, {lineEndWrapped, 1.0*j*bgGridItemSize});
        }
    }


    // simple variant, full lines with no motion
    /*for (int i=0; i<ofGetWidth()/bgGridItemSize; i++) { // VERTICAL LINES
        ofDrawLine({1.0*i*bgGridItemSize, 0}, {1.0*i*bgGridItemSize, ofGetHeight()});
    }
    for (int j=0; j<ofGetHeight()/bgGridItemSize; j++) { // HORIZONTAL LINES
        ofDrawLine({0, 1.0*j*bgGridItemSize}, {ofGetWidth(), 1.0*j*bgGridItemSize});
    }*/

    // END LINES BETWEEN GRID SQUARES



    // DRAW WANDERERS
    for (int i=0; i<NUM_WANDERERS; i++) {
        common.drawWanderer(wandererLocs[i], wandererDirs[i], i, scaleFactor);
    }
}


// target is displayed at mouse location when clicked
// as well as at each touch location on mobile
void RectagonScene::drawTarget(glm::vec2 pos) {
    // a circle at mouse/touch position
    /*ofFill();
    ofSetColor(240, 190, 20, 100);
    ofDrawCircle(x, y, 168.0f);*/


    // ORBITING RECTANGLES
    float orbitTimeTrack = ofGetElapsedTimef() * 1.36;

    float orbitAngle = ofWrap( orbitTimeTrack, 0, 360 );
    float orbitDist = 82.0f;
    float orbiterSize = 6.8f/scaleFactor;
    glm::vec2 orbitPos = this->angleToPosOnUnitSquareDeg( ofRadToDeg(orbitAngle) )*orbitDist;

    float orbitAngle2 = orbitAngle * 1.4;
    float orbitDist2 = 98.0f;
    float orbiterSize2 = 2.8f/scaleFactor;
    glm::vec2 orbitPos2 = this->angleToPosOnUnitSquareDeg( ofRadToDeg(orbitAngle2) )*orbitDist2;

    float orbitAngle3 = orbitAngle * 1.14;
    float orbitDist3 = 108.0f;
    float orbiterSize3 = 2.2f/scaleFactor;
    glm::vec2 orbitPos3 = this->angleToPosOnUnitSquareDeg( ofRadToDeg(orbitAngle3) )*orbitDist3;

    ofFill();
    ofSetColor(240, 190, 70, 255);
    ofDrawRectangle(pos.x + orbitPos.x, pos.y + orbitPos.y, orbiterSize, orbiterSize);
    ofDrawRectangle(pos.x + orbitPos2.x, pos.y + orbitPos2.y, orbiterSize2, orbiterSize2);
    ofDrawRectangle(pos.x + orbitPos3.x, pos.y + orbitPos3.y, orbiterSize3, orbiterSize3);
    //ofDrawCircle(pos.x + sin(orbitAngle2) * 110.0f, pos.y + cos(orbitAngle2) * 110.0f, 4.8f/scaleFactor);

    // END ORBITING RECTANGLES


    //for (auto vert : squareShape.getVertices()){
    ofPolyline sqCopy = squareShape;
    float jiggleScale = 0.75;
    for (auto &vert : sqCopy.getVertices()){
        vert.x += ofMap( ofNoise( ofGetElapsedTimeMillis()*0.004f ),
            0.0f, 1.0f, -jiggleScale, jiggleScale );
        vert.y += ofMap( ofNoise( ofGetElapsedTimeMillis()*0.004f + 1203.0f ),
            0.0f, 1.0f, -jiggleScale, jiggleScale );
    }

    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    this->drawConcentric( sqCopy, 75.0f, targetColor, 3.0f );
    ofPopMatrix();
}


void RectagonScene::update() {

    for (int i=0; i<NUM_WANDERERS; i++) {
        wandererLocs[i].x = glm::mod(
            wandererLocs[i].x + wandererDirs[i].x * wandererSpeeds[i],
            ofGetWidth()*1.0f );
        wandererLocs[i].y = glm::mod(
            wandererLocs[i].y + wandererDirs[i].y * wandererSpeeds[i],
            ofGetHeight()*1.0f );



        float loc_in_square_along_dir = 0.0f;
        if (glm::abs(wandererDirs[i].x) > 0.5) { // moving horizontally
            loc_in_square_along_dir = glm::abs( ofWrap(wandererLocs[i].x, 0, bgGridItemSize) );
        } else { // moving vertically
            loc_in_square_along_dir = glm::abs( ofWrap(wandererLocs[i].y, 0, bgGridItemSize) );
        }

        if (loc_in_square_along_dir > bgGridItemSize/2)
            wandererJustCrossed[i] = false;

        if (!wandererJustCrossed[i] // wanderer did not just cross a line
            && (loc_in_square_along_dir < bgGridItemSize/10) ) { // wanderer is near a line

            if ( wandererSpeeds[i] > wandererRushingSpeed-0.05 ) { // rushing, no timers
                wandererTurn(i);
                wandererJustCrossed[i] = true;

                // not used during rush but needs to have a sensible
                // value for when rushing has ended
                wandererTurnTimers[i] = wandererGetRandomValueForTimer(); // reset timer to random value

            } else { // not rushing, use timers
                if ( wandererTurnTimers[i] < 0 ) { // it's time to turn again (turn timer is up)
                    wandererTurn(i);
                    wandererJustCrossed[i] = true;
                    wandererTurnTimers[i] = wandererGetRandomValueForTimer(); // reset timer to random value
                }
            }

        }


        bool nearOtherWanderer = false;
        for (int j=0; j<NUM_WANDERERS; j++) {
            if (i != j) { // do not check distance to self
                float dist = glm::distance(wandererLocs[i],wandererLocs[j]);
                if ( dist < wandererRepelDistance ) {
                    nearOtherWanderer = true;
                }
            }
        }

        // if there are targets we rush towards them
        if (targets.size() > 0) {
            wandererSpeeds[i] = wandererRushingSpeed;
            // no repel when rushing
        } else {
            wandererSpeeds[i] = wandererDefaultSpeed;
            if (nearOtherWanderer) {
                wandererSpeeds[i] = wandererRepelSpeed;
            }
        }

    } // for (int i=0; i<NUM_WANDERERS; i++) {

}


void RectagonScene::drawConnectorFromGridToTarget(glm::vec2 gridPos, glm::vec2 targetPos, float seed) {

    // HIGHLIGHT RECTANGLE ON GRID
    ofColor rectCol = connectorColor;
    rectCol.a = 12; // rect alpha value
    ofSetColor(rectCol);
    ofDrawRectangle(gridPos.x-bgGridItemSize/2, gridPos.y-bgGridItemSize/2,
                    bgGridItemSize, bgGridItemSize);


    seed = ofWrap(seed*999.9, 0, 1000);

    targetPos += ofMap( ofNoise(seed*235.2), 0, 1, -15, 15);

    glm::vec2 scale = targetPos - gridPos;

    // GENERATE POINTS BETWEEN (0,0)..(1,1)
    glm::vec2 points[4];
    points[0] = {0,0};
    points[3] = {1,1};

    if (ofNoise(seed*252.43) < 0.5) { // middle y value randomized
        points[1].x = 0;
        points[1].y = ofMap( ofNoise( seed*0.02 ), 0, 1, 0.2, 0.5 );
        points[2].x = 1;
        points[2].y = points[1].y;
    } else { // middle x value randomized
        points[1].x = ofMap( ofNoise( seed*0.02 ), 0, 1, 0.2, 0.5 );
        points[1].y = 0;
        points[2].x = points[1].x;
        points[2].y = 1;
    }

    // TRANSLATE & SCALE POINTS
    for (int i=0; i<4; i++) {
        points[i] *= scale;
        points[i] += gridPos;
    }

    // DRAW
    ofFill();
    ofSetColor(connectorColor);
    ofSetLineWidth(1.45);

    for (int i=0; i<3; i++) {
        ofDrawLine(points[i], points[i+1]);
    }

    //this->drawAngularLine(gridPos, targetPos, connectorColor, 2.5, 3, 0.5, seed);
}



void RectagonScene::wandererTurn(int wandererIndex) {
    int i = wandererIndex;

    float distToNearestTarget = 999999999.9f;
    glm::vec2 nearestTargetPos = {0.0f,0.0f};

    for (auto t : targets) {
        float dist = glm::distance( {t.x, t.y}, wandererLocs[i] );
        if (dist < distToNearestTarget) {
            distToNearestTarget = dist;
            nearestTargetPos = {t.x, t.y};
        }
    }


    if (distToNearestTarget < 99999.9) { // at least one target active
        float horizontalDist = wandererLocs[i].x - nearestTargetPos.x;
        float verticalDist = wandererLocs[i].y - nearestTargetPos.y;

        if ( glm::abs(horizontalDist) > glm::abs(verticalDist) ) { // further horizontally than vertically
            if (horizontalDist > 0) { // target on left side of wanderer
                wandererDirs[i] = {-1,0};
            } else { // target on right side of wanderer
                wandererDirs[i] = {1,0};
            }
        } else { // further vertically than horizontally
            if (verticalDist > 0) { // target below wanderer
                wandererDirs[i] = {0,-1};
            } else { // target above wanderer
                wandererDirs[i] = {0,1};
            }
        }


    } else { // no targets active
        if (wandererDirs[i].x == 1 || wandererDirs[i].x == -1) {
            wandererDirs[i].x = 0;
            if (ofRandomf() < 0.5) {
                wandererDirs[i].y = 1;
            } else {
                wandererDirs[i].y = -1;
            }
        } else if (wandererDirs[i].y == 1 || wandererDirs[i].y == -1) {
            wandererDirs[i].y = 0;
            if (ofRandomf() < 0.5) {
                wandererDirs[i].x = 1;
            } else {
                wandererDirs[i].x = -1;
            }
        } else {}
    }

}


float RectagonScene::wandererGetRandomValueForTimer() {
    return ofRandomf()*2.74;
}


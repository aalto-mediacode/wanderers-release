#include "StrandyScene.h"


void StrandyScene::setup() {
    strandColor = ofColor(120, 120, 120, 60);
    particleColor = ofColor(160, 160, 160, 28);

    angle = 0.0f;

    wandererDefaultSpeed = 2.15f;
    for (int i=0; i<NUM_WANDERERS; i++) {
        wandererLocs[i] = {ofRandomWidth(),ofRandomHeight()};
        wandererDirs[i] = {ofRandomf(),ofRandomf()};
        wandererDirs[i] = glm::normalize( wandererDirs[i] );
        wandererSpeeds[i] = wandererDefaultSpeed;
    }

    line = this->generateSubidividedLine(120);
    // rotate vertices 90deg and scale down 10x
    // now they are from (0,-1) to (0,1)
    for (auto &vert : line.getVertices()) {
        vert.y = vert.x;
        vert.x = 0.0;
    }
}


void StrandyScene::draw() {
    /*ofFill();
    ofSetColor(220, 210, 220, 100);
    ofDrawCircle(180.0f, 180.0f, 68.0f);*/
    ofBackground(32, 32, 32, 255);

    // debug visualization of targets
    for (auto t : targets) {
        ofFill();
        ofSetColor(210, 210, 210, 40);
        ofDrawCircle(t.x, t.y, 12.0f);
    }

    // to make sure even fx-distorted lines don't leave
    // empty areas at left or right edge of screen
    int padding = 300;

    float windTimeTrack = ofGetElapsedTimef()*0.27f;
    float windStrength =
          ofMap( sin( windTimeTrack ), -1, 1, 24.5f, 46.1f )
        + ofMap( ofNoise( windTimeTrack ), 0, 1, -8.5f, 12.6f );
    windStrength *= 3.0f;

    // strands
    int stepSize = 40;
    stepSize = (int) (stepSize/scaleFactor);
    for (int i=-padding; i<ofGetWidth()+padding; i+=stepSize) {

        glm::vec2 pos = {i + stepSize/2, ofGetHeight()/2};

        // apparently 0.05*height scales strands to full screen height
        float scale = ofGetHeight()*0.055f;

        // lines nearer or further from one another (waves)
        float xoffTimeTrack = 999 + ofGetElapsedTimef()*0.63f;
        float xoffScale = stepSize*1.05;
        pos.x += ofMap(sin(xoffTimeTrack + pos.x*2980.62), -1, 1, -xoffScale, xoffScale);

        ofPolyline cpLine = line;

        // transform each vertex to screen space
        // (where targets' and wanderers' coordinates are defined)
        for (auto &vert : cpLine.getVertices()) {
            vert = {(vert.x*scale) + pos.x,
                    (vert.y*scale) + pos.y,
                    0.0};
        }

        // FORCES (vertex fx)
        for (auto &vert : cpLine.getVertices()) {
            windBendStrandsHorizontal(vert,
                0.33f*ofGetHeight(), windStrength, ofGetHeight()*0.58f);
            repelStrandsFromTargets(vert,
                190.0f, 101.5f/scaleFactor);
        }

        // vertically moving particles
        float pFlowSeed = ofNoise(i*1.472)*0.3467f;
        // TODO: glm::vec2 particlePos = pos + {1.3f,0.0f};
        this->drawParticleFlow(
            cpLine, 263.1f, 6.0f, 2, {1.5f, 4.0f}, particleColor, pFlowSeed);

        // wanderers attract strands but not particles
        // so this force is after drawing particle flow
        // FORCES (vertex fx)
        for (auto &vert : cpLine.getVertices()) {
            attractStrandsToWanderers(vert, 105.0f, 27.0f, 22.7f);
        }

        this->drawScatter(cpLine,
            {15.95f, 10.5f}, 5, i*0.2145,
            strandColor, 1.2f);

    }

    // DRAW WANDERERS
    for (int i=0; i<NUM_WANDERERS; i++) {
        common.drawWanderer(wandererLocs[i], wandererDirs[i], i, scaleFactor);
    }

    // TODO: fullscreen effects..? bloom?

}


// target is displayed at mouse location when clicked
// as well as at each touch location on mobile
void StrandyScene::drawTarget(glm::vec2 pos) {

    // DEBUG
    /*ofFill();
    ofSetColor(240, 190, 20, 100);
    ofDrawCircle(pos.x, pos.y, 4.0f);*/


    // (FAKE) RADIAL GRADIENT
    // using stacked circles
    glm::vec3 c = {240, 190, 20};
    ofSetCircleResolution(100);
    ofSetColor(c.x, c.y, c.z, 5); ofDrawCircle(pos.x, pos.y, 170.1f);
    ofSetColor(c.x, c.y, c.z, 15); ofDrawCircle(pos.x, pos.y, 57.1f);
    ofSetColor(c.x, c.y, c.z, 45); ofDrawCircle(pos.x, pos.y, 25.1f);
    ofSetColor(c.x, c.y, c.z, 100); ofDrawCircle(pos.x, pos.y, 14.1f);
    ofSetColor(c.x, c.y, c.z, 250); ofDrawCircle(pos.x, pos.y, 4.1f);
    ofSetCircleResolution(50);
    // END RADIAL GRADIENT


    // ORBIT SYSTEM
    // circles orbiting near mouse/touch position

    int numOrbiters = 8;
    float orbitTimeTrack = ofGetElapsedTimef()*118.327f;
    float orbitWidth = 260.0;
    float orbitInnermostRadius = 15.0f;

    ofFill();

    ofPushMatrix();
    ofTranslate(pos.x, pos.y);

    for (int i=0; i<numOrbiters; i++) {
        ofPushMatrix();
        float orbitAngle = ofWrap(
                       orbitTimeTrack * ofMap( ofNoise(i*0.8856), 0, 1, 0.5, 1.8) // vary speed
                       + ofMap( ofNoise(i*0.1856), 0, 1, 0, 360), // vary starting angle
                   0, 360); // wrap between 0..360
        float currentRadius = orbitInnermostRadius + i*(orbitWidth/numOrbiters-orbitInnermostRadius);

        // draw trails
        ofSetColor(240, 190, 70, 65);
        int trailLength = 25;
        for (int j=0; j<trailLength; j++) {
            ofPushMatrix();
            ofRotateDeg(orbitAngle - j*2.2);
            ofTranslate(currentRadius, 0);
            ofDrawCircle(0, 0, 1.1f);
            ofPopMatrix();
        }

        // draw orbiting "planet" itself
        ofSetColor(240, 190, 70, 135);
        ofRotateDeg(orbitAngle);
        ofTranslate(currentRadius, 0);
        ofDrawCircle(0, 0, 2.4f);


        // draw moon/suborbiter
        ofSetColor(240, 130, 30, 105);
        if (ofNoise(i*0.7325) < 0.75) { // for n-% of orbiters
            ofPushMatrix();
            ofRotateDeg(orbitAngle + 168.5);
            ofTranslate(6.5, 0);
            ofDrawCircle(0, 0, 1.1f);
            ofPopMatrix();
        }

        ofPopMatrix();
    }

    ofPopMatrix();
    // END ORBIT SYSTEM



    // PARTICLE SYSTEM
    // particles drawing in towards target
    int numParticles = 50;
    float particleID = 0;
    float scatterRadius = 86.0f/scaleFactor;
    ofSetColor(230, 190, 210, 80);
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    for (int i=0; i<numParticles; i++) {

        float particleSpeed = -0.16127f;
        float timeTrack = ofGetElapsedTimef()*particleSpeed + 346*i;
        float perParticleOffset = particleID*2.321;
        float particleLifeElapsed = ofWrap( timeTrack+perParticleOffset, 0.0, 1.0);

        float particleAngle = ofMap( ofNoise(particleID*0.32428), 0, 1, 0, 720 );
        float particleDist = particleLifeElapsed*scatterRadius;

        /*float x = pos.x + sin(particleAngle)*particleDist;
        float y = pos.y + cos(particleAngle)*particleDist;*/
        ofPushMatrix();
        ofRotateDeg(particleAngle);
        ofTranslate(particleDist, 0);
        ofDrawRectangle(0, 0, 1.2f, 5.0f);
        ofPopMatrix();

        particleID++;
    }
    ofPopMatrix();
    // END PARTICLE SYSTEM

}




void StrandyScene::update() {
    for (int i=0; i<NUM_WANDERERS; i++) {

        // damp speed on each update cycle
        // to counter repel acceleration effect
        wandererSpeeds[i] = ofLerp( wandererSpeeds[i], wandererDefaultSpeed, 0.85 );

        wandererLocs[i].x = glm::mod(
            wandererLocs[i].x + wandererDirs[i].x * wandererSpeeds[i],
            ofGetWidth()*1.0f );
        wandererLocs[i].y = glm::mod(
            wandererLocs[i].y + wandererDirs[i].y * wandererSpeeds[i],
            ofGetHeight()*1.0f );
    }

    if (targets.size() == 0) { // no touches/clicks
        repelWanderersFromEachOther(42.0f, 8.3f);
    } else {
        attractWanderersToTargets(50.0f, 300.0f);
    }

    angle = angle + 0.0416f;
}




void StrandyScene::windBendStrandsHorizontal(
    glm::vec3 & vert, float windSourceHeight, float windStrength, float maxDist
) {

    glm::vec2 vert2 = {vert.x,vert.y};
    //vert2.y *= lineDrawSize;
    //vert2 += pos;

    float yDist = glm::abs( vert2.y - windSourceHeight );
    float effectAmount = ofClamp( (maxDist-yDist)/maxDist, 0, 1);

    // remap effect amount along curve
    // doing it in a clunky 3D way because I didn't
    // quickly find simple 1D remap functionality in OF
    glm::vec3 curveStart = {0,    0,   0};
    glm::vec3 curveEnd   = {0,    1,   0};
    glm::vec3 curveCP_1  = {0.25, 0.1, 0}; // adjust mapping here (xy)
    glm::vec3 curveCP_2  = {0.75, 0.9, 0}; // adjust mapping here (xy)

    glm::vec3 interpolatedAmount3 = ofCurvePoint(
         curveCP_1, curveStart, curveEnd, curveCP_2,
         effectAmount);
    float interpolatedAmount = interpolatedAmount3.y;

    // based on both y-pos and time
    float timeTrack = ofGetElapsedTimef()*0.17f;
    float noiseAmount = ofMap( ofNoise(
               vert2.y*0.0093f + timeTrack
          ), 0, 1, -3.785f, 3.4f )
          * ofMap( sin( timeTrack+52.403 ), -1, 1, 0.5, 3.5);

    vert.x += windStrength * interpolatedAmount + noiseAmount*scaleFactor;
}




void StrandyScene::attractStrandsToWanderers(
    glm::vec3 & vert, float maxDist, float snapDist, float attractionAmount
) {

    // foreach wanderer
    for (int j=0; j<NUM_WANDERERS; j++) {

        glm::vec2 wLoc = wandererLocs[j];
        glm::vec2 vert2 = {vert.x,vert.y};

        // debug draw all lines
        /*ofSetColor(120, 110, 120, 12);
        ofDrawLine(wLoc, vert2);*/

        float dist = glm::distance(vert2, wLoc);
        glm::vec2 dist2 = { // per-component distance
            glm::distance(vert2.x, wLoc.x),
            glm::distance(vert2.y, wLoc.y) };

        if ( dist < maxDist) { // inside attraction radious
            float effectAmount = (maxDist-dist)/maxDist;

            if (dist < snapDist) { // right next to wanderer so we just snap to it
                vert.x = wLoc.x;
                vert.y = wLoc.y;
            } else { // not right next to wanderer so let's attract

                // debug draw nearby lines
                // could also be used as intentional effect
                /*ofSetColor(220, 10, 20, 150);
                ofSetLineWidth(3.0f);
                ofDrawLine(wLoc, vert2);*/

                // noise near wanderers
                float noiseAmount = 0.0084;
                vert.x += ofRandom(-1,1)*noiseAmount*effectAmount;
                vert.y += ofRandom(-1,1)*noiseAmount*effectAmount;

                // attraction to nearby wandererers
                glm::vec2 dir = glm::normalize(wLoc-vert2);
                glm::vec2 offset2 = dir*effectAmount*attractionAmount;

                vert.x += offset2.x;
                vert.y += offset2.y;
                //vert.y += dir.y*effectAmount*attractionAmount;

            }
        }
    }
}




void StrandyScene::repelStrandsFromTargets(
    glm::vec3 & vert, float maxDist, float repelAmount
) {

    // foreach target
    for (auto t : targets) {

        // convert the current wanderer location
        // to the same coord system as line vertices
        glm::vec2 vert2 = {vert.x,vert.y};
        //vert2.y *= lineDrawSize;
        //vert2 += pos;

        // debug draw all lines
        //ofSetColor(120, 110, 120, 12);
        //ofDrawLine(wLoc, vert2);

        float dist = glm::distance(vert2, t);
        glm::vec2 dist2 = { // per-component distance
            glm::distance(vert2.x, t.x),
            glm::distance(vert2.y, t.y) };

        if ( dist < maxDist) { // inside attraction radious
            float effectAmount = (maxDist-dist)/maxDist;

            // debug draw nearby lines
            // could also be used as intentional effect
            /*ofSetColor(220, 10, 20, 150);
            ofSetLineWidth(3.0f);
            ofDrawLine(wLoc, vert2);*/

            glm::vec2 dir = -glm::normalize(t-vert2);
            glm::vec2 offset2 = dir*effectAmount * repelAmount*scaleFactor;

            vert.x += offset2.x;
            vert.y += offset2.y;
        }
    }
}


void StrandyScene::attractWanderersToTargets(float minDist, float maxDist) {

    if (targets.size() > 0) {

        for (int i=0; i<NUM_WANDERERS; i++) {
            float distToNearest = 999999999.9f;
            glm::vec2 nearestPos = {0.0f,0.0f};

            for (auto t : targets) {
                float dist = glm::distance( {t.x, t.y}, wandererLocs[i] );
                if (dist < distToNearest) {
                    distToNearest = dist;
                    nearestPos = {t.x, t.y};
                }
            }

            if (distToNearest < maxDist) {

                glm::vec2 dir = wandererLocs[i] - nearestPos;
                dir = -dir;

                // go sideways if near target
                if (distToNearest < minDist/scaleFactor) {
                    glm::vec3 dir3 = {dir.x, dir.y, 0};
                    glm::vec3 crossed = glm::cross( dir3, {0,0,1} );
                    dir = {crossed.x, crossed.y};
                    float noiseAmount = 0.15f;
                    float timeTrack = ofGetElapsedTimef()*0.42f;
                    dir += {
                        ofMap( ofNoise(timeTrack+225.0), 0, 1, -noiseAmount, noiseAmount ),
                        ofMap( ofNoise(timeTrack+910.0), 0, 1, -noiseAmount, noiseAmount ) };
                }

                dir = glm::normalize(dir);

                wandererDirs[i].x = ofLerp(dir.x, wandererDirs[i].x, 0.5f);
                wandererDirs[i].y = ofLerp(dir.y, wandererDirs[i].y, 0.5f);

                wandererDirs[i] = glm::normalize( wandererDirs[i] );
            }
        }

    }

}


void StrandyScene::repelWanderersFromEachOther(float repelTresh, float repelStrength) {

    for (int i=0; i<NUM_WANDERERS; i++) {

        glm::vec2 dirSum = {0.0f,0.0f};
        bool near = false; // is near at least one other wanderer

        for (int j=0; j<NUM_WANDERERS; j++) {

            float dist = glm::distance( wandererLocs[i], wandererLocs[j] );
            if (dist < repelTresh && dist > 0.001) {
                dirSum += wandererLocs[i] - wandererLocs[j];
                dirSum = glm::normalize(dirSum);
                float effectAmount = ofClamp( dist/repelTresh, 0.0, 1.0);
                wandererSpeeds[i] *= effectAmount*repelStrength;
                near = true;
            }
        }

        if (near) {
            wandererDirs[i].x = ofLerp(dirSum.x, wandererDirs[i].x, 0.25f);
            wandererDirs[i].y = ofLerp(dirSum.y, wandererDirs[i].y, 0.25f);
        }
    }
}


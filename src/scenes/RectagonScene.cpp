#include "RectagonScene.h"


void RectagonScene::setup() {
    angle = 0.0f;
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
    ofBackground(32, 32, 32, 255);

    // BACKGROUND GRID
    float GRID_ITEM_SIZE = 60.0f;
    for (int i=0; i<ofGetWidth()/GRID_ITEM_SIZE; i++) {
        for (int j=0; j<ofGetHeight()/GRID_ITEM_SIZE; j++) {

            glm::vec2 pos = {
                GRID_ITEM_SIZE/2 + i*GRID_ITEM_SIZE, // x
                GRID_ITEM_SIZE/2 + j*GRID_ITEM_SIZE}; // y
            glm::vec4 col = {210.0f, 210.0f, 210.0f, 40.0f};

            float seed = 859 * (85.0 + ofNoise(i*44.28)*0.349 + ofNoise(j*19.83)*0.196);
            float bgGridTimeTrack = ofGetElapsedTimeMillis()*0.0014f;
            float size = GRID_ITEM_SIZE*0.73
                * ofMap( ofNoise(bgGridTimeTrack+seed), 0, 1, 0.5, 1.0 ) // jiggle
                * ofMap( ofNoise(seed), 0, 1, 0.0022, 1.0); // large-scale variation

            ofPushMatrix();
            ofTranslate(pos.x, pos.y);
            this->drawConcentric(squareShape,
                size, col, 0.5f /* lineWidth */ );
            ofPopMatrix();

            if (ofNoise(seed*0.134) < 0.28) {
                drawConnectorsFromGridToTargets(pos);
            }
        }
    }
    // END BACKGROUND GRID
}


// target is displayed at mouse location when clicked
// as well as at each touch location on mobile
void RectagonScene::drawTarget(glm::vec2 pos) {
    // a circle at mouse/touch position
    /*ofFill();
    ofSetColor(240, 190, 20, 100);
    ofDrawCircle(x, y, 168.0f);*/

    // a circle orbiting near mouse/touch position
    ofFill();
    ofSetColor(240, 190, 70, 255);
    ofDrawCircle(pos.x + sin(angle) * 70.0f, pos.y + cos(angle) * 70.0f, 28.0f);

    // a smaller circle orbiting further from mouse/touch position
    ofFill();
    ofSetColor(240, 190, 70, 255);
    ofDrawCircle(pos.x + sin(angle) * 110.0f, pos.y + cos(angle) * 110.0f, 12.0f);


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
    glm::vec4 target_color = {160.0f, 240.0f, 0.0f, 255.0f};
    this->drawConcentric( sqCopy, 75.0f, target_color, 3.0f );
    ofPopMatrix();
}


void RectagonScene::update() {
    angle = angle + 0.06f;
}


void RectagonScene::drawConnectorsFromGridToTargets(glm::vec2 pos) {
    ofFill();
    ofSetColor(240, 190, 70, 255);
    ofSetLineWidth(2.5);

    int numSegments = 16;
    for (auto target : targets) {
        for (int i=0; i<numSegments; i++) {
            float segmentLength = glm::distance(pos, target) / numSegments;
            glm::vec2 dir = glm::normalize(target - pos);

            glm::vec2 startPos = pos + dir*i*segmentLength;
            glm::vec2 endPos = pos + dir*(i+1)*segmentLength;

            // DROOPING
            // connector is lower in the middle ("droops")
            float droopAmount = 38.5;
            float t_start = (i*1.0)/(numSegments*1.0);
            float t_end = ((i+1)*1.0)/(numSegments*1.0);
            float startDroop = ofMap( glm::abs(t_start - 0.5), 0, 0.5, 0, 1);
            float endDroop   = ofMap( glm::abs(t_end - 0.5),   0, 0.5, 0, 1);

            // remap effect amount along curve
            // doing it in a clunky 3D way because I didn't
            // quickly find simple 1D remap functionality in OF
            glm::vec3 curveStart = {0,    0,   0};
            glm::vec3 curveEnd   = {0,    1,   0};
            glm::vec3 curveCP_1  = {0.25, 0.05, 0}; // adjust mapping here (xy)
            glm::vec3 curveCP_2  = {0.75, 0.55, 0}; // adjust mapping here (xy)

            glm::vec3 interpolatedStartDroop3 = ofCurvePoint(
                 curveCP_1, curveStart, curveEnd, curveCP_2,
                 startDroop);
            glm::vec3 interpolatedEndDroop3 = ofCurvePoint(
                 curveCP_1, curveStart, curveEnd, curveCP_2,
                 endDroop);

            startDroop = interpolatedStartDroop3.y;
            endDroop = interpolatedEndDroop3.y;

            startPos.y = startPos.y - droopAmount*startDroop;
            endPos.y = endPos.y - droopAmount*endDroop;
            // END DROOPING

            ofDrawLine(startPos, endPos);
        }
        //ofDrawLine(pos, target);
    }
}


#include "ofxPlaygroundHelpers.h"


void ofxPlaygroundHelpers::drawScatter(
    ofPolyline shape,
    glm::vec2 scatter_amount, int num_copies, float seed,
    ofColor col, float lineWidth
) {
    // set drawing options
    ofFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(col);

    // transformation
    ofPushMatrix();
    //ofTranslate(pos);
    //ofScale(size);

    for (int i=0; i<num_copies; i++) {
        ofPushMatrix();
        ofTranslate(
            ofNoise( ofGetElapsedTimef()*1.25+i*4304+seed )
                    * scatter_amount.x,
            ofNoise( ofGetElapsedTimef()*1.35+i*9214+seed )
                    * scatter_amount.y );
        shape.draw();
        ofPopMatrix();
    }

    ofPopMatrix();
}


void ofxPlaygroundHelpers::drawConcentric(
    ofPolyline shape, float size, ofColor col, float lineWidth
) {
    // set drawing options
    ofFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(col);

    ofPushMatrix(); ofScale(size);
    shape.draw(); ofPopMatrix();

    ofPushMatrix(); ofScale(size*0.75);
    shape.draw(); ofPopMatrix();

    ofPushMatrix(); ofScale(size*0.55);
    shape.draw(); ofPopMatrix();

    ofPushMatrix(); ofScale(size*0.35);
    shape.draw(); ofPopMatrix();

    ofPushMatrix(); ofScale(size*0.15);
    shape.draw(); ofPopMatrix();
}


void ofxPlaygroundHelpers::drawParticleFlow( // along shape
    ofPolyline shape, float width, float wrapHeight,
    int pNum, glm::vec2 pSize, ofColor col, float seed
) {
    // transformation
    ofPushMatrix();
    //ofTranslate(pos);
    //ofScale(size);

    ofSetColor(col);

    float skip = 0;
    float particleID = 0;
    float yScatterAmount = 3.5;
    for (auto &vert : shape.getVertices()) {
        if ( ofWrap(skip, 0, 7) < 0.5) {
            for (int i=0; i<pNum; i++) {
                float x = vert.x + ofMap( ofNoise(particleID), 0, 1, -width/2.0, width/2.0) - pSize.x/2;
                float y = vert.y + ofMap( ofNoise(particleID), 0, 1, -yScatterAmount, yScatterAmount) - pSize.y/2;

                float particleSpeed = 1.6127f;
                float timeTrack = ofGetElapsedTimef()*particleSpeed + ofMap( ofNoise(particleID), 0, 1, 0, 9999);
                float perParticleOffset = seed*particleID*23.21;
                float particleLifeElapsed = ofWrap( timeTrack+perParticleOffset, 0.0, 1.0);

               // x = x + ofMap(particleLifeElapsed, 0, 1, -xScatterAmount, xScatterAmount);
                y = y - wrapHeight/2.0 + particleLifeElapsed*wrapHeight;

                ofDrawRectangle(x, y, pSize.x, pSize.y);
                particleID++;
            }
        }
        skip++;
        particleID++;
    }

    ofPopMatrix();
}


// WIP IDEA, WORKS BUT NOT VERY BEAUTIFULLY
/*void ofxPlaygroundHelpers::drawDroopingLine(glm::vec2 startPos, glm::vec2 endPos) {
    int numSegments = 16;
    for (int i=0; i<numSegments; i++) {
        float segmentLength = glm::distance(startPos, endPos) / numSegments;
        glm::vec2 dir = glm::normalize(endPos - startPos);

        glm::vec2 segStart = startPos + dir*i*segmentLength;
        glm::vec2 segEnd = startPos + dir*(i+1)*segmentLength;

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

        segStart.y = segStart.y - droopAmount*startDroop;
        segEnd.y = segEnd.y - droopAmount*endDroop;
        // END DROOPING

        ofDrawLine(segStart, segEnd);
    }
}*/



// WIP IDEA, DOES NOT WORK
/*void ofxPlaygroundHelpers::drawAngularLine(
        glm::vec2 startPos, glm::vec2 endPos, ofColor col,
        float lineWidth, int numSegments, float scatterAmount, float seed
) {
    if (numSegments > 95) // hardcoded max segment amount
        numSegments = 95;
    if (numSegments < 1) // hardcoded min segment amount
        numSegments = 1;

    numSegments = 3; // TODO: remove temp debug

    float stepSize = 1.0/numSegments;
    glm::vec2 scale = startPos - endPos;

    // GENERATE POINTS BETWEEN (0,0)..(1,1)
    glm::vec2 points[99]; // hardcoded max amount
    points[0] = {0,0};
    points[numSegments] = {1,1};

    seed = ofWrap(seed, 0, 1000);

    // first non-start point
    if (ofNoise(startPos.x*252.43) < 0.5) {
        points[1].x = 0;
        points[1].y = ofMap( ofNoise( (startPos.x+startPos.y+seed*85.3)*0.02 ), 0, 1, 0, stepSize*0.3 );
    } else {
        points[1].x = ofMap( ofNoise( (startPos.x+startPos.y+seed*85.3)*0.02 ), 0, 1, 0, stepSize*0.3 );
        points[1].y = 0;
    }

    // last non-end point
    if (ofNoise(endPos.x*311.72) < 0.5) {
        points[numSegments-1].x = ofMap( ofNoise( (startPos.x+startPos.y+seed*45.1)*0.02 ), 0, 1, stepSize*0.3, 1 );
        points[numSegments-1].y = 1;
    } else {
        points[numSegments-1].x = 1;
        points[numSegments-1].y = ofMap( ofNoise( (startPos.x+startPos.y+seed*45.1)*0.02 ), 0, 1, stepSize*0.3, 1 );
    }

    // middle points
    //if (numSegments>3) {
    //    for (int i=2; i<points->length()-2; i++) {
    //        glm::vec2 minPos = points[1];
    //        glm::vec2 maxPos = points[numSegments-1];
    //        points[i].x = stepSize*i;//ofMap( ofRandomf(), 0, 1, minPos.x, maxPos.x);
    //        points[i].y = stepSize*i;//ofMap( ofRandomf(), 0, 1, minPos.y, maxPos.y);
    //    }
    //}
    // END GENERATE POINTS


    // TRANSLATE & SCALE POINTS
    for (int i=0; i<numSegments+1; i++) {
        points[i] *= scale;
        points[i] += endPos;
    }

    // DRAW
    ofFill();
    ofSetColor(col);
    ofSetLineWidth(lineWidth);

    for (int i=0; i<numSegments; i++) {
        ofDrawLine(points[i], points[i+1]);
    }
}*/


// generates a square centered at origin
// with coordinates from -10 to 10
// resolution = amount of points per line (minus one)
ofPolyline ofxPlaygroundHelpers::generateSubidividedSquare(int resolution) {
    ofPolyline sq;

    // top line left to right
    for (int i=0; i<=resolution; i++) {
        sq.addVertex(20.0f*i/resolution-10.0f,10.0f);
    }

    // right line top to bottom
    for (int i=0; i<=resolution; i++) {
        sq.addVertex(10.0f,-20.0f*i/resolution+10.0f);
    }

    // bottom line right to left
    for (int i=0; i<=resolution; i++) {
        sq.addVertex(-20.0f*i/resolution+10.0f,-10.0f);
    }

    // left line bottom to top
    for (int i=0; i<=resolution; i++) {
        sq.addVertex(-10.0f,20.0f*i/resolution-10.0f);
    }

    sq.close();
    return sq;
}


// generates a line centered at origin
// with x coordinates from -10 to 10 and y = 0
ofPolyline ofxPlaygroundHelpers::generateSubidividedLine(int resolution) {
    ofPolyline line;

    // line left to right
    for (int i=0; i<=resolution; i++) {
        line.addVertex(20.0f*i/resolution-10.0f, 0.0f);
    }

    return line;
}


// for input value:
// angle in degrees, CCW
// top is 0deg, left is 90deg, bottom is 180deg, right is 270deg (I believe..?)
glm::vec2 ofxPlaygroundHelpers::angleToPosOnUnitSquareDeg(float angle) {

    glm::vec2 pos = {1,0};

    // inside this function:
    // angle in degrees, CCW
    // right is 0deg, top is 90deg, left is 180deg, bottom is 270deg
    float a = ofWrap(angle-90, 0, 360);

    if ( (a>0 && a<45) || (a<360 && a>315) ) { // right edge
        pos.x = 1;
        if (a>0 && a<45)    { pos.y = ofMap(a, 0, 45, 0, -1); }
        if (a<360 && a>315) { pos.y = ofMap(a, 315, 360, 1, 0); }
    } else if (a>45 && a<135) { // top edge
        pos.x = ofMap( a, 45, 135, 1, -1);
        pos.y = -1;
    } else if (a>135 && a<225) { // left edge
        pos.x = -1;
        pos.y = ofMap( a, 135, 225, -1, 1);
    } else if (a>225 && a<315) { // bottom edge
        pos.x = ofMap( a, 225, 315, -1, 1);
        pos.y = 1;
    } else {}

    return pos;
}



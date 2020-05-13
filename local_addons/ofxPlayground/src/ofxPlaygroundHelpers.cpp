#include "ofxPlaygroundHelpers.h"


void ofxPlaygroundHelpers::drawScatter(
    ofPolyline shape,
    glm::vec2 scatter_amount, int num_copies, float seed,
    glm::vec4 color_rgba, float lineWidth
) {
    // set drawing options
    ofFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(color_rgba.r, color_rgba.g, color_rgba.b, color_rgba.a);

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
    ofPolyline shape, float size, glm::vec4 color_rgba, float lineWidth
) {
    // set drawing options
    ofFill();
    ofSetLineWidth(lineWidth);
    ofSetColor(color_rgba.r, color_rgba.g, color_rgba.b, color_rgba.a);

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
    int pNum, glm::vec2 pSize, glm::vec4 color_rgba, float seed
) {
    // transformation
    ofPushMatrix();
    //ofTranslate(pos);
    //ofScale(size);

    ofSetColor( color_rgba.r, color_rgba.g, color_rgba.b, color_rgba.a );

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


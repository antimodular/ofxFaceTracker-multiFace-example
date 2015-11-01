#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
	cam.initGrabber(640, 480);
	
    maxFaces = 4;
	multiTracker.setup(maxFaces,640,480);

}

void testApp::exit() {
    multiTracker.exit();
}

void testApp::update() {
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	cam.update();
	if(cam.isFrameNew()) {
		multiTracker.update(toCv(cam));
	}
}

void testApp::draw() {
	ofSetColor(255);
	cam.draw(0, 0);
	ofDrawBitmapString("camera", 10, 20);
  
    drawMat(multiTracker.buffer, 640, 0);
    ofDrawBitmapString("buffer with blocked out faces", 640, 20);

    drawMat(multiTracker.buffer2, 640, 480);

    
    for(int i=0; i< multiTracker.trackers_active.size(); i++){
        multiTracker.trackers_active[i]->draw();
        ofDrawBitmapString(multiTracker.name_active[i]+ " | "+ofToString(multiTracker.index_active[i]), multiTracker.trackers_active[i]->getPosition());
    }

    ofDrawBitmapString("active   "+ofToString(multiTracker.trackers_active.size()), 20, 60);
    ofDrawBitmapString("inactive "+ofToString(multiTracker.trackers_inactive.size()), 20, 80);
}

void testApp::keyPressed(int key) {

}
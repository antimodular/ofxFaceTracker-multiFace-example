#include "ofApp.h"

using namespace ofxCv;

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	
    cam.setDeviceID(0);
	cam.initGrabber(640, 480);
	
    maxFaces = 2;
	multiTracker.setup(maxFaces,640,480);

}

void ofApp::exit() {
    multiTracker.exit();
}

void ofApp::update() {
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	cam.update();
	if(cam.isFrameNew()) {
        camImg.setFromPixels(cam.getPixels());
       // camImg.mirror(1,0);
		multiTracker.update(toCv(camImg));
	}
    ofGetTimestampString();
}

void ofApp::draw() {
	ofSetColor(255);
	camImg.draw(0, 0);
	ofDrawBitmapString("camera", 10, 20);
  
    drawMat(multiTracker.buffer, 640, 0);
    ofDrawBitmapString("buffer with blocked out faces", 640, 20);

    drawMat(multiTracker.buffer2, 640, 480);

    
    for(int i=0; i< multiTracker.trackers_active.size(); i++){
        multiTracker.trackers_active[i]->draw();
       // ofDrawBitmapString(multiTracker.name_active[i]+ " | "+ofToString(multiTracker.index_active[i]), multiTracker.trackers_active[i]->getPosition());
         ofDrawBitmapString(multiTracker.name_active[i]+ " | "+ofToString(multiTracker.index_active[i]) + " | "+ ofToString(multiTracker.trackers_active[i]->getAge()), multiTracker.trackers_active[i]->getPosition());
    }

  
    ofDrawBitmapStringHighlight("amtFound   "+ofToString(multiTracker.amtFound), 20, 30);

    ofDrawBitmapStringHighlight("active   "+ofToString(multiTracker.trackers_active.size()), 20, 60);
    ofDrawBitmapStringHighlight("inactive "+ofToString(multiTracker.trackers_inactive.size()), 20, 80);


    ofDrawBitmapStringHighlight("tracker 0 "+ofToString(multiTracker.trackers_active[0]->getFound()), 220, 60);
    ofDrawBitmapStringHighlight("tracker 1 "+ofToString(multiTracker.trackers_active[1]->getFound()), 220, 80);

    ofDrawBitmapStringHighlight("wasActive 0 "+ofToString(multiTracker.wasActive[0]), 320, 60);
    ofDrawBitmapStringHighlight("wasActive 1 "+ofToString(multiTracker.wasActive[1]), 320, 80);
    
}

void ofApp::keyPressed(int key) {

}
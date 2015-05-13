#include "ofApp.h"
void ofApp::setup(){
    ofSetWindowShape(1280, 720 + 200);
    
    playCount = 0;
    ofSetFrameRate(30);
    key.open("tracking.txt");
    key.dumpTrackName();
    
    player.loadMovie("sample.mov");
    //    player.loadMovie("sample1.mp4");
    player.play();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    player.update();
    ofPoint pt, pt1, pt2;
    
    F = player.getCurrentFrame();
    x = track.getParam(F, "X pixels");
    y = track.getParam(F, "Y pixels");
    
    x1 = ofMap((float)F, 0, player.getTotalNumFrames(), 0, (float)ofGetWidth());
    y1 = ofMap(x, 0, (float)ofGetWidth(),  (float)ofGetHeight()-200, (float)ofGetHeight());
    
    x2 = ofMap((float)F, 0, player.getTotalNumFrames(), 0, (float)ofGetWidth());
    y2 = ofMap(y, 0, (float)ofGetHeight(),  (float)ofGetHeight()-200, (float)ofGetHeight());
    
    if(playCount == 0){
        pt.set(x, y);
        line.addVertex(pt);
        
        pt1.set(x1, y1);
        lineX.addVertex(pt1);
        
        pt2.set(x2, y2);
        lineY.addVertex(pt2);
        
    }
    
    if (F == player.getTotalNumFrames()){
        playCount++;
    }
    //        cout << F << endl;
    //        cout << playCount << endl;
    
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(200);
    ofSetColor(255);
    ofSetLineWidth(2);
    player.draw(0, 0);
    
    //    ofNoFill();
    ofCircle(x, y, 10, 10);
    line.draw();
    
    ofSetColor(0);
    ofDrawBitmapString("Time: "+ofToString(F), 10, ofGetHeight()-180);
    
    ofSetLineWidth(1);
    
    float t =  ofGetElapsedTimef()*10;
    
    
    ofSetColor(0, 0, 255);
    lineX.draw();
    ofCircle(x1, y1, 4);
    
    ofDrawBitmapString(" X: "+ofToString(x), x1, y1);
    
    ofSetColor(255, 0, 0);
    lineY.draw();
    ofCircle(x2, y2, 4);
    
    ofDrawBitmapString(" Y: "+ofToString(720-y), x2, y2);
    
    
        
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ' ){
        line.clear();
        lineX.clear();
        lineY.clear();
        playCount = 0;
        player.setFrame(0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


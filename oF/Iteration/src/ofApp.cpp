#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    rectWidth = ofGetWidth() / 50;
    rectHeight = ofGetHeight() / 50;
    rectLength = 50;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float t = ofGetElapsedTimef();
    
    ofSeedRandom(t*5);
    
    ofBackground(0,0,0);
    
    for (int i = 0; i < rectLength; i++){
        for (int j = 0; j < rectLength; j++){
            float rand = ofRandom(48);
            rand = int(rand);
            if(i*j % 8 == 0){
                ofSetColor(sin(t*3)*255);
            }else{
                ofSetColor(ofRandom(255));
            }
            ofRect(i*rectWidth,j*rectHeight,rectLength,rectLength);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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

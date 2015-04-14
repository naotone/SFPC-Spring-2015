#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    //		void dragEvent(ofDragInfo dragInfo);
    //		void gotMessage(ofMessage msg);
    
    
};

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0,0,0);
    ofSetCircleResolution(100);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    //    ofSeedRandom(0);
    float t = ofGetElapsedTimef()*3;
    float t2 = ofGetElapsedTimef()*2;
    
    float step = ofRandomf() * 3.2 + 10.1;
    float rotate = ofRandomf() * 4.02 + 30.02;
    float freq = ofRandom(1) * 3.2 + 0.5;
    
    float rand = t;
    
    if ((int)t2 % 10 ==0){
        t = t * ofRandom(-2, 2);
        
        float xorig = ofGetWidth() / 2;
        float yorig = ofGetHeight() / 2;
        
        for (int k = 0; k < 200; k++){
            
            float radius = 20 + k;
            float angle = ofGetElapsedTimef() * (1 + k / 10.0);
            float x = xorig + radius * cos(angle);
            float y = yorig + radius * -sin(angle);
            
            ofSetColor(255,255,255);
            ofCircle(x, y, 2);
        }
        
        
    }else{
        
        
        for (int i = 1; i < 5; i++) {
            for (int j = 1; j < 4; j++) {
                float xorig = ofGetWidth() / 5 * (float)i;
                float yorig = ofGetHeight() / 4 *  (float)j;
                
                for (int k = 0; k < 200; k++){
                    
                    float radius = k*0.5;
                    float angle = ofGetElapsedTimef() * (1 + k / 100.0);
                    float x = xorig + radius * cos(angle+t+j);
                    float y = yorig + radius * -sin(angle+t2+i) ;
                    
                    //                    float x = xorig + cos(rotate *  step *k) + radius;
                    //                   float y = yorig + sin(rotate * step * k) + radius;
                    
                    //                    tX = target.x + Math.cos(target.rotate + target.step * ii) * tRad;
                    //                    tY = target.y + Math.sin(target.rotate + target.step * ii) * tRad;
                    
                    ofSetColor(255,255,255);
                    ofCircle(x, y, 2);
                }
            }
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


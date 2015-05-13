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
    
    
    int drawX, drawY;
    int size;
    
    
    
};

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(0);
    ofSetCircleResolution(100);
    
    size = 150;
    drawX = (int)ofGetWidth() / size;
    drawY = (int)ofGetHeight() /size;
    
    ofSetColor(255);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSeedRandom(0);
    float t = ofGetElapsedTimef()*0.1;
    
    t = sin(t);

//    cout << t << endl;

//    if ((int)t % 2001000 != 0){
    if(1 != 1){
        float xorig = ofGetWidth() / 2;
        float yorig = ofGetHeight() / 2;

        float c = t;
        if(c > 20){
            c=2.0;
        }
//        c = ofMap(c, -1, 1, 3, 20);
    
        for (int k = 0; k < ofGetWindowHeight()/2.5; k++){
            
            //                    float radius = size;
            float angle = ofGetElapsedTimef()*ofGetWindowHeight()/2.5 /1.0; //ofGetElapsedTimef() * (1 + k / 100.0);
            
            float rad = t*k * PI / 180;
            
            float v = cos(c *rad);
            float r = ofGetWindowHeight()/2.5* v;
            float x = xorig + r * cos(t+rad);
            float y = yorig + r * sin(t+rad);
            
            ofCircle(x, y, 2);
        }
    }else{
        
        
        for (int i = 1; i < drawX; i++) {
            for (int j = 1; j < drawY; j++) {
                
                float xorig = ofGetWidth() / drawX * (float)i;
                float yorig = ofGetHeight() / drawY *  (float)j;
                float c = ofMap(ofRandom(-1,1), -1, 1, 1.2, 4);
                
//                cout << c << endl;
                
                
                for (int k = 0; k < size; k= k+3
                     ){
                    
//                    float angle = ofGetElapsedTimef()*size /1.0; //ofGetElapsedTimef() * (1 + k / 100.0);
                    
                    float speed = t*k*0.5 ;
                    float velocity = cos(c *speed);
                    float radious = size*0.45* velocity;
                    float x = xorig + radious * cos(speed);
                    float y = yorig + radious * sin(speed);
 //
                    
                    
                    
                    
                    
                    //float rad = t*k*10 * PI /180 ;
//                    float velocity = cos(c * rad);
//                    float radious = size*0.45* velocity;
//                    float x = xorig + radious * cos(rad);
//                    float y = yorig + radious * sin(rad);
                    
                    ofCircle(x, y, 1);
                }
            }
        }
    }
    float kk = 8.62;
    cout << kk * 100.0 << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
            
        default:
            break;
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
    drawX = (int)ofGetWidth() / size;
    drawY = (int)ofGetHeight() / size;
    
    
}


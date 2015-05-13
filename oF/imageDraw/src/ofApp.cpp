#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    myImg.loadImage("giraffe.jpg"); //giraffe zebra
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetCircleResolution(60);
    
    _2.clear();
    _3.clear();
    _4.clear();
    _5.clear();
    
    for (int i = 0; i < myImg.width; i+=5){
        for (int j = 0; j < myImg.height; j+=5){
            
            ofColor myColor = myImg.getColor(i, j);
            float brightness = myColor.getBrightness();
            int blink = ofMap( brightness, 0, 255, 2, 5 );
            
            if(blink == 2){
                _2.push_back(ofPoint(i,j));
            }else if(blink == 3){
                _3.push_back(ofPoint(i,j));
            }else if(blink == 4){
                _4.push_back(ofPoint(i,j));
            }else if(blink == 5){
                _5.push_back(ofPoint(i,j));
            }
        }
    }
    
    gui.setup("gui");
    gui.add(speed.set("speed", 0.3, 0.1, 2.0));
    gui.setPosition(ofPoint(600, 20));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    ofBackground(0);
    ofSetColor(255);
    //myImg.draw(0,0);
    int radius = 3;
    
    int t = ofGetFrameNum() * speed ;
    
    if(t % 13 == 0){
        for (int i = 0; i < _2.size(); i++) {
            ofCircle(_2[i].x, _2[i].y, radius);
        }
    }
    if(t % 11 == 0){
        for (int i = 0; i < _3.size(); i++) {
            ofCircle(_3[i].x, _3[i].y, radius);
        }
    }
    if(t % 7 == 0){
        for (int i = 0; i < _4.size(); i++) {
            ofCircle(_4[i].x, _4[i].y, radius);
        }
    }
    if(t % 5 == 0 ){
        for (int i = 0; i < _5.size(); i++) {
            ofCircle(_5[i].x, _5[i].y, radius);
        }
    }
    
    gui.draw();
    
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

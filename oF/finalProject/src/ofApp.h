#pragma once

#include "ofMain.h"
//#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxISF.h"

#define HOST "127.0.0.1"
#define PORT 12345

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
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void drawFbo();
    void drawGrabber();
    void sendOsc();
    void drawGui();
    
    ofVideoGrabber grabber;
    ofxOscSender sender;
    ofVideoPlayer video;
    ofTrueTypeFont verlag;
    
    
    bool bDebug;
    bool bVideo;
    bool bBang;
    bool bCam;
    bool bTalking;
    bool bGui;
    
    static const int camWidth = 1280;
    static const int camHeight = 720;
    
    //    static const int port = 12345;
    //    static const string host = 10.0.1.255;
    
    ofImage grayImage;
    int pixelX, pixelY;
    
    vector<int> binary, binary1;
    string out;
    vector<string> _out;
    
    ofVboMesh mesh;
    
    ofFbo mFbo, mFbo1;
//    int fadeAmnt;
    
    ofxISF::Shader isf;
    
    ofxPanel gui;
    
    ofxIntSlider fadeAmnt;
    ofxIntSlider speed;
    ofxFloatSlider shiftY;
    
};

using namespace cv;
using namespace ofxCv;

inline float modf( float X, float Y )
{
    return ( X - int( X / Y ) * Y );
}

//--------------------------------------------------------------
void ofApp::setup(){
    bDebug = false;
    bVideo = true;
    bBang = true;
    bCam = true;
    bTalking = false;
    bGui = false;
    
    
    
    ofBackground(230);
    ofSetWindowPosition(1920/2, 0);
    ofSetWindowShape(1920/2, 1080/2);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    grabber.initGrabber(camWidth, camHeight);
    grayImage.allocate(camWidth, camHeight, OF_IMAGE_GRAYSCALE);
    
    video.loadMovie("fingers.mov");
    video.play();
    
    sender.setup(HOST, PORT);
    
    verlag.loadFont("Verlag-XLight.otf", 40);
    mFbo.allocate(1920, 1200, GL_RGBA32F_ARB);
    mFbo.begin();
    ofClear(255,255,255,0);
    mFbo.end();

//    isf.setup(1280, 720, GL_RGB32F);
//    isf.load("edge.fs");
//    isf.setImage("inputImage", grabber.getTextureReference());

    
    gui.setup();
    gui.add(speed.setup("speed", 1, 0, 100));
    gui.add(fadeAmnt.setup("fade", 0, 0, 100));
    
}

//--------------------------------------------------------------
void ofApp::update(){
//    ofEnableAlphaBlending();
    
    grabber.update();
//    if(grabber.isFrameNew()){
        drawGrabber();
        sendOsc();
//    }
    
    
    mFbo.begin();
    drawFbo();
    mFbo.end();

    
//    isf.update();

}

//--------------------------------------------------------------
void ofApp::drawGrabber(){
    binary.clear();

    convertColor(grabber, grayImage, CV_RGB2GRAY);
    
    grayImage.update();
    for (int i = 0; i < camWidth; i+=40) {
//        for(int j = 0; j < camHeight; j+=100){
            ofColor mColor = grayImage.getColor(i, ofGetHeight()/2);
            int brightness = mColor.getBrightness()+ 100 ;
            brightness = ofMap(brightness, 0, 255, 0, 1);
            //                    brightness = ofMap(brightness, 0, 100, 0, 1);
            
            if (binary.size() < 32) {
                binary.push_back(brightness);
            }
//        }
        //  cout << "sum"<< i << " = " << std::accumulate(binary.begin(), binary.end(), 0) << endl;
        //  cout << "avg "<< i << "= "<<std::accumulate(binary.begin(), binary.end(), 0) / binary.size() << endl;
        //  binary.clear();
        
    }
    
}

//--------------------------------------------------------------
void ofApp::drawFbo(){
    if( ofGetKeyPressed('0') ){
        ofClear(255,255,255, 0);
    }
    
//    fadeAmnt = 1;
//    if(ofGetKeyPressed('1')){
//        fadeAmnt = 1;
//    }else if(ofGetKeyPressed('2')){
//        fadeAmnt = 5;
//    }else if(ofGetKeyPressed('3')){
//        fadeAmnt = 15;
//    }
    
    ofFill();
    ofSetColor(255,255,255, fadeAmnt);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    ofSetColor(255,255,255);
    
    
    int width = ofGetWidth() / 32;
    
    for(int i = 0; i < (int)binary.size(); i++){
        if(binary[i] == 0){
            ofSetColor(30,30,30, 250);
        }else{
            ofSetColor(255,255,255, 250);
        }
//        ofLine(width * i,shiftY, width * (i+1), shiftY);
        ofRect(width*i, shiftY, width, 1);
        
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    //ofSetWindowTitle(ofToString(ofGetFrameRate()));
        shiftY = (ofGetFrameNum()*speed) % 1200;
//        shiftY = (ofGetElapsedTimeMillis() / speed) % 1200;
//    shiftY = modf((ofGetElapsedTimeMillis() / 10),  1200);

    if(bCam){
        grayImage.draw(0, 0,  ofGetWidth(), ofGetHeight());
    }
    
    
    ofSetLineWidth(4);
    ofSetColor(255);
    mFbo.draw(0, 0);
    
    ofSetColor(255);
    ofSetLineWidth(1);
    ofLine(0, shiftY, ofGetWidth(), shiftY);
    
    ofSetColor(255);
    
    
    if(bDebug){
        ofSetColor(255);
        grabber.draw(0, 0, camWidth/2, camHeight/2);
        grayImage.draw(camWidth,0, camWidth/2,camHeight/2);
        //        cout << "x: " << pixelX << "y: "<< pixelY << endl;
        
        ofSetColor(255, 0, 0);
        verlag.drawString(ofToString(ofGetFrameRate()), 0, 100);
        
    }
    
    if(bGui){
        gui.draw();
    }
    
}

//--------------------------------------------------------------
void ofApp::sendOsc(){
    ofxOscMessage m, m1;
    m.setAddress("/pix");
    //            cout << binary.size() << endl;
    
    for (int k = 0 ; k < binary.size(); k++) {
        m.addIntArg(binary[k]);
        //        out = out + ofToString(binary[k]);
        
    }
    //        cout << out << endl;
    //    if(_out.size() > row){
    //        _out.erase(_out.begin());
    //    }
    //    _out.push_back(out);
    
    //        cout << _out[0] << endl;
    
    sender.sendMessage(m);
    
    
    if(ofGetFrameNum() % 130 == 0 || ofGetFrameNum() % 80 == 0 ){
        bTalking = !bTalking;
    }
    
    
    
    m1.setAddress("/bTalking");
    if(bTalking){
        m1.addIntArg(1);
    }else{
        m1.addIntArg(0);
    }
    sender.sendMessage(m1);
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'g':
            bGui = !bGui;
            break;
        case 't':
            bTalking = !bTalking;
            break;
        case 'c':
            bCam = !bCam;
            break;
            
        case 'd':
            bDebug = !bDebug;
            break;
        case 'v':
            bVideo = !bVideo;
            break;
        case 'f':
            ofToggleFullscreen();
            break;
            
        default:
            break;
    }
    if(key == 'a' || key == 'A'){
        ofxOscMessage m;
        m.setAddress("/test");
        m.addIntArg(1);
        m.addFloatArg(3.5f);
        m.addStringArg("hello");
        m.addFloatArg(ofGetElapsedTimef());
        sender.sendMessage(m);
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

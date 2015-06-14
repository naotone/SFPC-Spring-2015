#pragma once

#include "ofMain.h"
//#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxISF.h"
#include "ofxSyphon.h"

#define ME "127.0.0.1"
//#define PROJ "127.0.0.1"
#define YOU "127.0.0.1"
#define PORT 11111
#define PORT_PROJ 11112

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
    ofxOscSender me, you, proj;
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
    
    ofImage grayImage, syphonImage;
    int pixelX, pixelY;
    
    vector<int> binary, binary1;
    string out;
    vector<string> _out;
    
    ofVboMesh mesh;
    
    ofFbo mFbo, mFbo1;
    ofPixels pixels;
    //    int fadeAmnt;
    
    ofxISF::Shader isf;
    
    ofxPanel gui;
    
    ofxIntSlider fadeAmnt;
    ofxIntSlider speed;
    ofxFloatSlider shiftY;
    
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonServerDirectory dir;
    ofxSyphonClient client;
    int dirIdx;
    
    ofxSyphonServer mainOutputSyphonServer;
    
    
    
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
    ofSetWindowShape(1920, 1200);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    grabber.initGrabber(camWidth, camHeight);
    grayImage.allocate(1920, 1200, OF_IMAGE_GRAYSCALE);
    
    video.loadMovie("fingers.mov");
    video.play();
    
    me.setup(ME, PORT);
    you.setup(YOU, PORT);
    proj.setup(ME, PORT_PROJ);
    
    verlag.loadFont("Verlag-XLight.otf", 40);
    mFbo.allocate(1920, 1200, GL_RGBA32F_ARB);
    mFbo.begin();
    ofClear(255,255,255,0);
    mFbo.end();
    
    mFbo1.allocate(1920, 1200, GL_RGB);
    
    //    isf.setup(1280, 720, GL_RGB32F);
    //    isf.load("edge.fs");
    //    isf.setImage("inputImage", grabber.getTextureReference());
    
    
    gui.setup();
    gui.add(speed.setup("speed", 1, 0, 100));
    gui.add(fadeAmnt.setup("fade", 0, 0, 100));
    
    dir.setup();
    client.setup();
    
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    dirIdx = -1;
    
    mainOutputSyphonServer.setName("1");
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    grabber.update();
    drawGrabber();
    sendOsc();
    
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
    for (int i = 0; i < 1920; i+=60) {
        ofColor mColor = grayImage.getColor(i, 1200/2);
        int brightness = mColor.getBrightness() ;
        brightness = ofMap(brightness, 0, 255, 0, 1);
        
        cout << brightness << endl;
        if (binary.size() < 32) {
            binary.push_back(brightness);
        }
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
    ofSetColor(255);
    
    mFbo1.begin();
    ofClear(255,255,255,0);
    client.draw(0, 0);
    mFbo1.end();
    shiftY = (ofGetFrameNum()*speed) % 1200;
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
    
    mainOutputSyphonServer.publishScreen();
    
    
    
    if(bDebug){
        ofSetColor(255);
        //        grabber.draw(0, 0, camWidth/2, camHeight/2);
        grayImage.draw(camWidth,0, camWidth/2,camHeight/2);
        //        cout << "x: " << pixelX << "y: "<< pixelY << endl;
        
        ofSetColor(255, 0, 0);
        verlag.drawString(ofToString(ofGetFrameRate()), 0, 100);
        
    }
    
    if(bGui){
        gui.draw();
    }
    
    
}
///////////////////////////////////////////////////////////////////////////
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

//--------------------------------------------------------------
void ofApp::sendOsc(){
    ofxOscMessage m, m1;
    m.setAddress("/pix");
    
    cout << binary << endl;
    
    for (int k = 0 ; k < binary.size(); k++) {
        m.addIntArg(binary[k]);
    me.sendMessage(m);
    you.sendMessage(m);
    
    
    if(ofGetFrameNum() % 130 == 0 || ofGetFrameNum() % 80 == 0 ){
        bTalking = !bTalking;
    }
    
    
    
    m1.setAddress("/bTalking");
    if(bTalking){
        m1.addIntArg(1);
    }else{
        m1.addIntArg(0);
    }
    me.sendMessage(m1);
    you.sendMessage(m1);
    proj.sendMessage(m1);
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
        me.sendMessage(m);
        you.sendMessage(m);
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
    //press any key to move through all available Syphon servers
    if (dir.size() > 0)
    {
        dirIdx++;
        if(dirIdx > dir.size() - 1)
            dirIdx = 0;
        
        client.set(dir.getDescription(dirIdx));
        string serverName = client.getServerName();
        string appName = client.getApplicationName();
        
        if(serverName == ""){
            serverName = "null";
        }
        if(appName == ""){
            appName = "null";
        }
        ofSetWindowTitle(serverName + ":" + appName);
    }
    else
    {
        ofSetWindowTitle("No Server");
    }
    
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

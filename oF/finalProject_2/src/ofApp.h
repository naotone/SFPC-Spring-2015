#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxSyphon.h"

#define PORT 11111
#define NUM_MSG_STRINGS 20

#define VDMX "127.0.0.1"
#define VDMX_PORT 22222


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
    
    void setupLights();
    void setupObjs();
    void drawObjs();
    void oscReceiver();
    void oscSender();
    
    
    bool bFill;
    bool bWireframe;
    bool bDrawNormals;
    bool bDrawAxes;
    bool bDrawLights;
    bool bInfoText;
    bool bMousePressed;
    bool bSplitFaces;
    
    int mode;
    bool bTalking;
    bool bGui;
    
    float camSpeed;
    float t;
    
    ofSpherePrimitive sphere;
    vector<ofSpherePrimitive> spheres;
    ofSpherePrimitive icoSphere;
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    
    ofLight pointLight4;
    ofLight pointLight5;
    ofLight pointLight6;
    
    ofMaterial material;
    
    // place to store the sides of the box //
    ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    ofVboMesh deformPlane;
    ofVboMesh topCap, bottomCap, body;
    vector<ofMeshFace> triangles;
    
    ofCamera cam;
    //    ofNode node;
    
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    ofxOscSender vdmx;
    int counter;
    
    ofxSyphonServer mainOutputSyphonServer;
    
    
    ofxPanel gui;
    ofxIntSlider size, camz;
    ofxIntSlider bgBrightness1, bgBrightness2;
    
};

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    bTalking = true;
    size = 1;
    camSpeed = 1;
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    ofDisableArbTex();
    cam.setFov(60);
    cam.setFarClip(10000);
    bFill       = true;
    bWireframe  = false;
    bDrawNormals= false;
    bDrawAxes   = false;
    bDrawLights = false;
    bInfoText   = false;
    bMousePressed   = false;
    bSplitFaces = true;
    
    setupObjs();
    
    ofSetSphereResolution(24);
    
    receiver.setup(PORT);
    vdmx.setup(VDMX, VDMX_PORT);
    
    gui.setup();
    gui.add(size.setup("size", 3, 1, 20));
    gui.add(camz.setup("camz", 2500, 0, 4000));
    gui.add(bgBrightness1.setup("bgBrightness1", 233, 0, 255));
    gui.add(bgBrightness2.setup("bgBrightness2", 233, 0, 255));
    
    mainOutputSyphonServer.setName("2");
    
    
}

//--------------------------------------------------------------
void ofApp::setupObjs(){
    float width     = ofGetWidth() * .12;
    
    icoSphere.setRadius( width );
    icoSphere.setResolution(0); // number of subdivides //
    
    mode = 0;
    
    sphere.setRadius( ofGetWidth()*.3 );
    sphere.setResolution(2);
    sphere.setMode( OF_PRIMITIVE_TRIANGLES );
    vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
    sphere.getMesh().setFromTriangles( triangles, true );
    
    for (int i = 0; i < size; i++) {
        spheres.push_back(sphere);
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::setupLights(){
    ofSetSmoothLighting(true);
    
    int t = (ofGetFrameNum()/3) % 255;
    int t2 = (ofGetFrameNum()/4) % 255;
    int t3 = (ofGetFrameNum()/9) % 255;
    
    pointLight.setDiffuseColor( ofColor::fromHsb(t, 200, 80));
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    pointLight2.setDiffuseColor( ofColor::fromHsb(t2, 200, 100));
    pointLight2.setSpecularColor( ofColor(255.f, 255.f, 255.f) );
    
    pointLight3.setDiffuseColor( ofColor::fromHsb(t3, 180, 200));
    pointLight3.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    pointLight4.setDiffuseColor( ofColor::fromHsb(93, 140, 200));
    pointLight4.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    
    pointLight5.setDiffuseColor( ofColor::fromHsb(2, 120, 200));
    pointLight5.setSpecularColor( ofColor(255.f, 255.f, 255.f) );
    
    pointLight6.setDiffuseColor( ofColor::fromHsb(t, 140, 200));
    pointLight6.setSpecularColor( ofColor(255.f, 255.f, 255.f) );
    
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 120);
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
}

//--------------------------------------------------------------
void ofApp::update() {
    oscReceiver();
    oscSender();
    
    cam.setPosition(cos(ofGetElapsedTimef()*.2)*(ofGetWidth()), sin(ofGetElapsedTimef()*.4)*ofGetHeight(), cos(ofGetElapsedTimef()*0.7)*camz);
    
    
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.1), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.1),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()*.1), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    pointLight4.setPosition((ofGetWidth()*.5)+ sin(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 1000);
    pointLight5.setPosition((ofGetWidth()*.5)+ sin(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + cos(ofGetElapsedTimef()*.7)*(ofGetHeight()), -1300);
    
    pointLight6 .setPosition(cos(ofGetElapsedTimef()*.2)*(ofGetWidth()), sin(ofGetElapsedTimef()*.4)*ofGetHeight(), cos(ofGetElapsedTimef()*0.7)*camz);
    
    //ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw() {
    setupLights();
    
    //    cout << cam.getPosition() << endl;
    icoSphere.setResolution(counter/2 + 2);
    icoSphere.setMode( OF_PRIMITIVE_TRIANGLES );
    triangles = icoSphere.getMesh().getUniqueFaces();
    icoSphere.getMesh().setFromTriangles(triangles, true);
    
    int t = (ofGetFrameNum()/2) % 255;
    int t2 = (ofGetFrameNum()/5) % 255;
    
    ofColor color1 = ofColor::fromHsb(t, 142, bgBrightness1);
    ofColor color2 = ofColor::fromHsb(t2, 142, bgBrightness2);
    ofBackgroundGradient(color1, color2, OF_GRADIENT_LINEAR);
    
    ofEnableDepthTest();
    
    cam.lookAt(ofVec3f(0, 0, 0));
    cam.begin();
    
    ofEnableLighting();
    
    if(!bTalking){
        pointLight.enable();
        pointLight2.enable();
        pointLight3.enable();
        pointLight4.enable();
        pointLight5.enable();
        pointLight6.enable();
    }else{
        pointLight.disable();
        pointLight2.enable();
        pointLight3.enable();
        pointLight4.enable();
        pointLight5.enable();
        pointLight6.enable();
        
    }
    
    material.begin();
    drawObjs();
    
    //    ofPopMatrix();
    ofDisableDepthTest();
    cam.end();
    
    ofFill();
    
    if(bInfoText) {
        stringstream ss;
        ss << "Framerate: " << ofToString(ofGetFrameRate(),0) << "\n";
        ss << "(f): Toggle Fullscreen"<<endl<<"(s): Draw Solid Shapes"<<endl<<"(w): Draw Wireframes"<<endl;
        ss <<"(1/2/3/4): Set Resolutions" <<endl<<"(n): Draw Normals"<<"\n(LEFT/RIGHT): Set Mode "<<ofToString(mode,0)<<endl;
        ss <<"(z): Split Faces " <<bSplitFaces<<endl;
        ss <<"(a): Draw Axes"<<endl<<"(l): Render lights"<<endl<<"(t): Info Text"<<endl;
        
        ofDrawBitmapString(ss.str().c_str(), 20, 20);
    }
    
    mainOutputSyphonServer.publishScreen();
    
    if(bGui){
        gui.draw();
    }
    
}

void ofApp::drawObjs(){
    float spinX = sin(ofGetElapsedTimef()*.00035f);
    float spinY = cos(ofGetElapsedTimef()*.00075f);
    
    
    
    
    // Sphere //
    sphere.rotate(spinX, 1.0, 0.0, 0.0);
    sphere.rotate(spinY, 0, 1.0, 0.0);
    
    ofPopMatrix();
    sphere.setPosition(-3000, 0, 0);
    ofPushMatrix();
    
    for (int i = 0; i < 3; i++) {
        ofTranslate(1500, 0, 0);
        if(bFill) {
            ofFill();
            ofSetColor(255);
            sphere.draw();
        }
    }
    ofPopMatrix();
    
    // ICO Sphere //
    icoSphere.rotate(-spinX/2, 1.0, 0.0, 0.0);
    icoSphere.rotate(-spinY/2, 0, 1.0, 0.0);
    
    ofSeedRandom(0);
    for (int i = 0; i <  5 + counter ; i++) {
        icoSphere.setPosition(ofRandom(-3000,3000), ofRandom(-3000,3000), ofRandom(-2000,2000));
        
        if(bFill) {
            ofFill();
            ofSetColor(255);
            icoSphere.draw();
        }
        if(bWireframe) {
            ofNoFill();
            ofSetColor(0, 0, 0);
            if(!bFill) ofSetColor(255);
            icoSphere.setScale(1.01f);
            icoSphere.drawWireframe();
            icoSphere.setScale(1.f);
        }
        
        
    }
    
    material.end();
    ofDisableLighting();
    
    if(bDrawLights) {
        ofFill();
        ofSetColor(pointLight.getDiffuseColor());
        pointLight.draw();
        ofSetColor(pointLight2.getDiffuseColor());
        pointLight2.draw();
        ofSetColor(pointLight3.getDiffuseColor());
        pointLight3.draw();
    }
    
    if(bDrawNormals) {
        ofSetColor(225, 0, 255);
        sphere.drawNormals(20, bSplitFaces);
        icoSphere.drawNormals(20, bSplitFaces);
    }
    if(bDrawAxes) {
        sphere.drawAxes(sphere.getRadius()+30);
        icoSphere.drawAxes(icoSphere.getRadius()+30);
    }
}

//--------------------------------------------------------------
void ofApp::oscSender(){
    
}

//--------------------------------------------------------------
void ofApp::oscReceiver(){
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }
    
    // check for waiting messages
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        // check for mouse moved message
        if(m.getAddress() == "/bTalking"){
            // both the arguments are int32's
            if(m.getArgAsInt32(0)== false){
                bTalking = false;
            }else{
                bTalking = true;
            }
            
        }else if (m.getAddress() == "/pix"){
            counter = 0;
            
            for(int i =0; i < 32; i++){
                //            cout << ofToString(m.getArgAsInt32(i)) << endl;
                if(m.getArgAsInt32(i)== 1){
                    counter++;
                }
            }
            //        cout << ofToString(counter) << endl;
            ofxOscMessage m, m1, m2, m3, m4, m5, m6;
            m.setAddress("/counter");
            m.addIntArg(counter);
            vdmx.sendMessage(m);
            
            m1.setAddress("/5");
            (counter > 5) ? m1.addFloatArg(1.0) : m1.addFloatArg(0.0) ;
            vdmx.sendMessage(m1);
            
            m2.setAddress("/10");
            (counter > 10) ? m2.addFloatArg(1.0) : m2.addFloatArg(0.0) ;
            vdmx.sendMessage(m2);
            
            m3.setAddress("/15");
            (counter > 15) ? m3.addFloatArg(1.0) : m3.addFloatArg(0.0) ;
            vdmx.sendMessage(m3);
            
            m4.setAddress("/20");
            (counter > 20) ? m4.addFloatArg(1.0) : m4.addFloatArg(0.0) ;
            vdmx.sendMessage(m4);
            
            m5.setAddress("/25");
            (counter > 25) ? m5.addFloatArg(1.0) : m5.addFloatArg(0.0) ;
            vdmx.sendMessage(m5);
            
        }
        else{
            // unrecognized message: display on the bottom of the screen
            string msg_string;
            msg_string = m.getAddress();
            msg_string += ": ";
            for(int i = 0; i < m.getNumArgs(); i++){
                // get the argument type
                msg_string += m.getArgTypeName(i);
                msg_string += ":";
                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msg_string += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msg_string += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msg_string += m.getArgAsString(i);
                }
                else{
                    msg_string += "unknown";
                }
            }
            // add to the list of strings to display
            msg_strings[current_msg_string] = msg_string;
            timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
            current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
            // clear the next line
            msg_strings[current_msg_string] = "";
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    switch(key) {
        case '1':
            t+=0.001;
            break;
        case '2':
            t-=0.001;
            break;
            
        case ' ':
            bTalking = !bTalking;
            break;
        case 'g':
            bGui = !bGui;
            break;
            //        case '=':
            //            size++;
            //            break;
            //        case '-':
            //            size--;
            //            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 's':
            bFill = !bFill;
            break;
        case 'w':
            bWireframe = !bWireframe;
            break;
        case 'n':
            bDrawNormals = !bDrawNormals;
            break;
        case 'a':
            bDrawAxes = !bDrawAxes;
            break;
        case 'l':
            bDrawLights = !bDrawLights;
            break;
        case 't':
            bInfoText=!bInfoText;
            break;
        case 'z':
            bSplitFaces = !bSplitFaces;
            
            if(bSplitFaces) {
                sphere.setMode( OF_PRIMITIVE_TRIANGLES );
                vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
                sphere.getMesh().setFromTriangles( triangles, true );
                
                icoSphere.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = icoSphere.getMesh().getUniqueFaces();
                icoSphere.getMesh().setFromTriangles(triangles, true);
                
                
            } else {
                // vertex normals are calculated with creation, set resolution //
                sphere.setResolution( sphere.getResolution() );
                
                icoSphere.setResolution( icoSphere.getResolution() );
                
            }
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
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

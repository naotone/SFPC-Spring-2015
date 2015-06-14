#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxSyphon.h"

#define PORT 11112
#define NUM_MSG_STRINGS 20


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
    
    void oscReceiver();
    
    ofxOscReceiver receiver;
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];

    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
    ofxSyphonServerDirectory dir;
    ofxSyphonClient client1, client2;
    int dirIdx;
  
    //setup our directory
    
    bool bTalking;
    
    ofxSyphonServer server;
    

};


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    dir.setup();
    //setup our client
    client1.setup();
    client2.setup();
    
    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
    
    dirIdx = -1;
    
    receiver.setup(PORT);

    
}

//--------------------------------------------------------------
void ofApp::update(){
    oscReceiver();
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(!bTalking){
        ofClear(255, 255, 255);
        client1.draw(0, 0, ofGetWidth(), ofGetHeight());
    }else{
        ofClear(255, 255, 255);
        client2.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    server.publishScreen();
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
            cout << "talk" << endl;
            // both the arguments are int32's
            if(m.getArgAsInt32(0) == false){
                bTalking = false;
            }else{
                bTalking = true;
            }
            
            
        }
        // check for mouse button message
        //        else if(m.getAddress() == "/mouse/button"){
        //            // the single argument is a string
        //            mouseButtonState = m.getArgAsString(0);
        //        }
        // check for an image being sent (note: the size of the image depends greatly on your network buffer sizes - if an image is too big the message won't come through )
        //        else if(m.getAddress() == "/image" ){
        //            ofBuffer buffer = m.getArgAsBlob(0);
        //            receivedImage.loadImage(buffer);
        //        }
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


//these are our directory's callbacks
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
void ofApp::keyPressed(int key){
    switch (key) {
        case '[':
            //press any key to move through all available Syphon servers
            if (dir.size() > 0)
            {
                dirIdx++;
                if(dirIdx > dir.size() - 1)
                    dirIdx = 0;
                
                client1.set(dir.getDescription(dirIdx));
                string serverName = client1.getServerName();
                string appName = client1.getApplicationName();
                
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
            
            break;
        case ']':
            //press any key to move through all available Syphon servers
            if (dir.size() > 0)
            {
                dirIdx++;
                if(dirIdx > dir.size() - 1)
                    dirIdx = 0;
                
                client2.set(dir.getDescription(dirIdx));
                string serverName = client2.getServerName();
                string appName = client2.getApplicationName();
                
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
            
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key =='f'){
        ofToggleFullscreen();
    }
    
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

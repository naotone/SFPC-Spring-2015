#pragma once

#include "ofMain.h"
#include "ofxAfterEffectsKeyframeParser.h"

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
    
    ofVideoPlayer player;
    ofxAfterEffectsKeyframeParser key;
    ofPolyline line, lineX, lineY;
    
    const ofxAfterEffectsKeyframeParser::Track& track = key.getTrack("Motion Trackers/Tracker #1/Track Point #1/Feature Center");
    int F;
    float x, y;
    float x1, y1;
    float x2, y2;
    
    int playCount;
        
};

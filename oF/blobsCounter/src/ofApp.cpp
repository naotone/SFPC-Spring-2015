#include "ofApp.h"
ofDirectory dir("./");

//--------------------------------------------------------------
void ofApp::setup(){
    bDrawNum = true;
    bDrawBlobs = true;
    bDrawChain = true;
    
    mImgNum = 0;
    dir.allowExt("png");
    dir.listDir();
    dir.sort();
    
    for (int i = 0 ; i < (int)dir.size(); i++) {
        mImg.push_back(ofImage());
        mImg[i].loadImage(dir.getPath(i));
    }
    
    mGrayImg.allocate(400, 300);
    
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
    ofBackground(30);
    ofSetLineWidth(1);
    
    mImg[mImgNum].draw(0, 0);
    
    mClorImg.setFromPixels(mImg[mImgNum].getPixels(),400,300);
    mGrayImg.setFromColorImage(mClorImg);
    mGrayImg.draw(400, 0);
    mGrayImg.draw(0, 300);
    
    switch (mImgNum) {
        case 0:
            cfMin = 10;
            cfMax = (400*300)/3;
            cfNCon = 10;
            break;
        case 1:
            cfMin = 0;
            cfMax = (400*300);
            cfNCon = 10000;
            break;
        case 2:
            cfMin = 100;
            cfMax = (400*300);
            cfNCon = 10;
            break;
        case 3:
            cfMin = 10;
            cfMax = (400*300);
            cfNCon = 10;
            break;
        case 4:
            cfMin = 0;
            cfMax = (400*300);
            cfNCon = 50;
            break;
            
        default:
            cfMin = 10;
            cfMax = (400*300)/3;
            cfNCon = 10;
            
            break;
    }
    
    contourFinder.findContours(mGrayImg, cfMin, cfMax, cfNCon, true);
    
    
    ofDrawBitmapString("Total: " + ofToString(contourFinder.nBlobs), 400, 300);
    
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        
        if(bDrawBlobs){
            contourFinder.blobs[i].draw(400,0);
        }
        
        if(bDrawNum){
            ofSetColor(255,0,0);
            ofDrawBitmapString(ofToString(i),
                               contourFinder.blobs[i].boundingRect.getCenter().x + 400,
                               contourFinder.blobs[i].boundingRect.getCenter().y + 0);
        }
        
        if(bDrawChain){
            ofSetColor(100,100,255);
            mPts.clear();
            int t = ofGetFrameNum();
            for (int j = 0; j < contourFinder.blobs[i].pts.size(); j++) {
                ofPoint pos;
                pos.set(contourFinder.blobs[i].pts[j]);
                mPts.push_back(pos);
                cout << "x: " + ofToString(contourFinder.blobs[i].pts[j].x) + "  y: " + ofToString(contourFinder.blobs[i].pts[j].y )<< endl;
            }
            
            ofBeginShape();
            for (int k = 0; k < mPts.size(); k++){
                ofVertex(mPts[k].x, mPts[k].y+300);
            }
            ofEndShape();
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '=':
            mImgNum < dir.size()-1 ? mImgNum++ : mImgNum;
            break;
        case '-':
            mImgNum > 0 ? mImgNum-- : mImgNum;
            break;
        case '1':
            bDrawBlobs = !bDrawBlobs;
            break;
        case '2':
            bDrawNum = !bDrawNum;
            break;
        case '3':
            bDrawChain = !bDrawChain;
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
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

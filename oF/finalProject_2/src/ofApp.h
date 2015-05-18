#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"

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
    
    
    bool bFill;
    bool bWireframe;
    bool bDrawNormals;
    bool bDrawAxes;
    bool bDrawLights;
    bool bInfoText;
    bool bMousePressed;
    bool bSplitFaces;
    
    int mode;
    
    ofSpherePrimitive sphere;
//    ofSpherePrimitive *sphere = new ofSpherePrimitive();
    vector<ofSpherePrimitive> spheres;
    ofIcoSpherePrimitive icoSphere;
    ofCylinderPrimitive cylinder;
    ofConePrimitive cone;
    ofBoxPrimitive box;
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofMaterial material;
    
    // place to store the sides of the box //
    ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    ofVboMesh deformPlane;
    ofVboMesh topCap, bottomCap, body;
    vector<ofMeshFace> triangles;
    
    ofCamera cam;
    ofNode node;
};

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableSmoothing();

    // GL_REPEAT for texture wrap only works with NON-ARB textures //
    // ofDisableArbTex();
    cam.setFov(60);
    bFill       = true;
    bWireframe  = false;
    bDrawNormals= false;
    bDrawAxes   = false;
    bDrawLights = false;
    bInfoText   = false;
    bMousePressed   = false;
    bSplitFaces = true;
    
    setupObjs();
    setupLights();
    
    ofSetSphereResolution(24);

}

//--------------------------------------------------------------
void ofApp::setupObjs(){
    float width     = ofGetWidth() * .12;
    float height    = ofGetHeight() * .12;
    
    box.set( width*1.25 );
    //    sphere[0].setRadius( width );
    icoSphere.setRadius( width );
    cylinder.set(width*.7, height*2.2);
    cone.set( width*.75, height*2.2 );
    
    //    sphere[0].setResolution(2);
    icoSphere.setResolution(0); // number of subdivides //
    cylinder.setResolution(4,2,1);
    cone.setResolution(4, 1, 1);
    box.setResolution(1);
    
    mode = 0;
    
    for (int i = 0; i < 10; i++) {
        sphere.setRadius( width );
        sphere.setResolution(2);
        sphere.setMode( OF_PRIMITIVE_TRIANGLES );
        vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
        sphere.getMesh().setFromTriangles( triangles, true );
        spheres.push_back(sphere);

    }
    
    icoSphere.setMode( OF_PRIMITIVE_TRIANGLES );
    triangles = icoSphere.getMesh().getUniqueFaces();
    icoSphere.getMesh().setFromTriangles(triangles, true);
    
    cylinder.setMode( OF_PRIMITIVE_TRIANGLES );
    triangles = cylinder.getMesh().getUniqueFaces();
    cylinder.getMesh().setFromTriangles(triangles, true);
    
    cone.setMode( OF_PRIMITIVE_TRIANGLES );
    triangles = cone.getMesh().getUniqueFaces();
    cone.getMesh().setFromTriangles(triangles, true);
    
    box.setMode( OF_PRIMITIVE_TRIANGLES );
    triangles = box.getMesh().getUniqueFaces();
    box.getMesh().setFromTriangles(triangles, true);
}

//--------------------------------------------------------------
void ofApp::setupLights(){
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 128 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));

}

//--------------------------------------------------------------
void ofApp::update() {
    cam.setPosition(cos(ofGetElapsedTimef()*.2)*(ofGetWidth()), sin(ofGetElapsedTimef()*.4)*ofGetHeight(), cos(ofGetElapsedTimef()*0.7)*1000);

    
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
    //ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
}

//--------------------------------------------------------------
void ofApp::draw() {
//    cout << cam.getPosition() << endl;
    
    int t = (ofGetFrameNum()/2) % 255;
    int t2 = (ofGetFrameNum()/5) % 255;
    
//    ofColor color1 = ofColor::fromHsb(191, 56, 91);(360,100,100)
    ofColor color1 = ofColor::fromHsb(t, 142, 233);
    ofColor color2 = ofColor::fromHsb(t2, 142, 233);
    ofBackgroundGradient(color1, color2, OF_GRADIENT_LINEAR);

    ofEnableDepthTest();

    cam.lookAt(ofVec3f(0, 0, 0));
    cam.begin();
//    ofDrawAxis(10000);

    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);

//    node.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
//    //    cam.setPosition(t2, 40, 190);
//    node.draw();

    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    
    material.begin();
    drawObjs();
    
    ofPopMatrix();
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
    
    
}

void ofApp::drawObjs(){
    float spinX = sin(ofGetElapsedTimef()*.00035f);
    float spinY = cos(ofGetElapsedTimef()*.00075f);

    // Box //
    box.setPosition(ofGetWidth()*.5, ofGetHeight()*.25, 0);
    box.rotate(spinX, 1.0, 0.0, 0.0);
    box.rotate(spinY, 0, 1.0, 0.0);
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            box.transformGL();
            for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
                ofPushMatrix();
                ofTranslate( boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50  );
                boxSides[i].draw();
                ofPopMatrix();
            }
            box.restoreTransformGL();
        } else {
//            box.draw();
        }
    }
    

    
    // Sphere //
    ofPushMatrix();
    for (int i = 0; i < 10; i++) {

    spheres[i].setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
        ofTranslate(200, 0, 0);

    spheres[i].rotate(spinX, 1.0, 0.0, 0.0);
    spheres[i].rotate(spinY, 0, 1.0, 0.0);
    
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            float angle = ofGetElapsedTimef()*3.2;
            float strength = (sin( angle+.25 )) * .5f * 5.f;
            ofVec3f faceNormal;
            for(int i = 0; i < triangles.size(); i++ ) {
                // store the face normal here.
                // we change the vertices, which makes the face normal change
                // every time that we call getFaceNormal //
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex( j, triangles[i].getVertex(j) + faceNormal * strength);
                }
            }
            spheres[i].getMesh().setFromTriangles( triangles );
        }

        spheres[i].draw();
        }
    }
    ofPopMatrix();
    
    
    
    // ICO Sphere //
    icoSphere.setPosition(ofGetWidth()*.2, ofGetHeight()*.75, 0);
    icoSphere.rotate(spinX, 1.0, 0.0, 0.0);
    icoSphere.rotate(spinY, 0, 1.0, 0.0);
    
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        
        if(mode == 3) {
            float angle = (ofGetElapsedTimef() * 1.4);
            ofVec3f faceNormal;
            for(int i = 0; i < triangles.size(); i++ ) {
                float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
                faceNormal = triangles[i].getFaceNormal();
                for(int j = 0; j < 3; j++ ) {
                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc );
                }
            }
            icoSphere.getMesh().setFromTriangles( triangles );
        }
        
//        icoSphere.draw();
    }
    
    
    // Cylinder //
    cylinder.setPosition(ofGetWidth()*.5, ofGetHeight()*.75, 0);
    cylinder.rotate(spinX, 1.0, 0.0, 0.0);
    cylinder.rotate(spinY, 0, 1.0, 0.0);
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            cylinder.transformGL();
            ofPushMatrix(); {
                if(topCap.getNumNormals() > 0) {
                    ofTranslate( topCap.getNormal(0) * (cos(ofGetElapsedTimef()*5)+1)*.5f * 100 );
                    topCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                if(bottomCap.getNumNormals() > 0) {
                    ofTranslate( bottomCap.getNormal(0) * (cos(ofGetElapsedTimef()*4)+1)*.5f * 100 );
                    bottomCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                float scale = (cos(ofGetElapsedTimef()*3)+1)*.5f + .2;
                ofScale( scale, scale, scale );
                body.draw();
            } ofPopMatrix();
            cylinder.restoreTransformGL();
        } else {
//            cylinder.draw();
        }
    }
    
    // Cone //
    cone.setPosition(ofGetWidth()*.8, ofGetHeight()*.75, 0);
    cone.rotate(spinX, 1.0, 0.0, 0.0);
    cone.rotate(spinY, 0, 1.0, 0.0);
    
    if(bFill) {
        ofFill();
        ofSetColor(255);
        if(mode == 3) {
            cone.transformGL();
            ofPushMatrix();
            if(bottomCap.getNumNormals() > 0 ) {
                ofTranslate( bottomCap.getNormal(0) * cone.getHeight()*.5 );
                ofRotate( sin(ofGetElapsedTimef()*5) * RAD_TO_DEG, 1, 0, 0);
                bottomCap.draw();
            }
            ofPopMatrix();
            
            ofPushMatrix();
            ofRotate(90, 1, 0, 0);
            ofRotate( (cos(ofGetElapsedTimef()*6) +1)*.5 * 360 , 1, 0, 0 );
            body.draw();
            ofPopMatrix();
            cone.restoreTransformGL();
        } else {
//            cone.draw();
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
        box.drawNormals(20, bSplitFaces);
        sphere.drawNormals(20, bSplitFaces);
        icoSphere.drawNormals(20, bSplitFaces);
        cylinder.drawNormals(20, bSplitFaces);
        cone.drawNormals(20, bSplitFaces);
    }
    if(bDrawAxes) {
        box.drawAxes(box.getWidth()+30);
        sphere.drawAxes(sphere.getRadius()+30);
        icoSphere.drawAxes(icoSphere.getRadius()+30);
        cylinder.drawAxes(cylinder.getHeight()+30);
        cone.drawAxes(cone.getHeight()+30);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    switch(key) {
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
                
                cylinder.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = cylinder.getMesh().getUniqueFaces();
                cylinder.getMesh().setFromTriangles(triangles, true);
                
                cone.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = cone.getMesh().getUniqueFaces();
                cone.getMesh().setFromTriangles(triangles, true);
                
                box.setMode( OF_PRIMITIVE_TRIANGLES );
                triangles = box.getMesh().getUniqueFaces();
                box.getMesh().setFromTriangles(triangles, true);
                
            } else {
                // vertex normals are calculated with creation, set resolution //
                sphere.setResolution( sphere.getResolution() );
                
                icoSphere.setResolution( icoSphere.getResolution() );
                
                cylinder.setResolution( cylinder.getResolutionRadius(), cylinder.getResolutionHeight(), cylinder.getResolutionCap() );
                cone.setResolution( cone.getResolutionRadius(), cone.getResolutionHeight(), cone.getResolutionCap() );
                box.setResolution( box.getResolutionWidth() );
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

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	Tracer.reset(new ofxNodeTracer(&N));
	step = 40.0f;

	//GUI.setName("GUI");
	GUI.setup("GUI");
	GUI.add(Tracer->getParameters());
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	Np.resetTransform();

	for(float s = 0;s<Tracer->getMaxP();s+=step)
	{
		Tracer->poseNodeAtP(&Np,s);
		Np.draw();
	}
	N.draw();
	GUI.draw();
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
	ofVec3f Pos = ofVec3f(x,y,0);
	ofVec3f Scl = ofVec3f(
		ofRandom(0.1f,5.0f),
		ofRandom(0.1f,5.0f),
		ofRandom(0.1f,5.0f));
	ofVec3f Rot = ofVec3f(
		ofRandom(-180.0f,180.0f),
		ofRandom(-180.0f,180.0f),
		ofRandom(-180.0f,180.0f));
	ofQuaternion Q;
	Q.makeRotate(
		Rot.x,ofVec3f(1,0,0),
		Rot.y,ofVec3f(0,1,0),
		Rot.z,ofVec3f(0,0,1));

	ofMatrix4x4 Mat;
	
	//
	Mat.scale(Scl);
	Mat.rotate(Q);	
	Mat.translate(Pos);	

	N.setTransformMatrix(Mat);

	Tracer->trace();

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

void myNode::customDraw()
{
	ofDrawAxis(50);
	ofPushStyle();
	ofSetColor(ofColor::blue);
	ofCircle(ofPoint(0,0),25);
	ofPopStyle();
}

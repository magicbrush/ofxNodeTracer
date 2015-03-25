#pragma once

#include "ofMain.h"
#include "ofxNodeTracer.h"
#include "ofxGui.h"


class myNode:
	public ofNode
{
public:

private:
	virtual void customDraw();
};

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

		float step;
		myNode N;
		ofNode Np;
		ofPtr<ofxNodeTracer> Tracer;

		ofxPanel GUI;
		
};

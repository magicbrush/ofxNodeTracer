#pragma once

#include "ofMain.h"


class ofxNodeTracer
{
public:
	enum traceMode
	{
		LOCAL,
		GLOBAL
	};
	ofxNodeTracer(
		ofNode* n, 
		float wtPos=1.0f, 
		float wtRot=0.0f, 
		float wtScale = 0.0f,
		float wtTime=0.0f,
		traceMode TMode=GLOBAL);

	ofParameterGroup getParameters() const { return parameters; }
	float getMaxP() const;
	
	void setTraceMode(traceMode tmode);
	void linkNode(ofNode* n);
	void trace();

	void clear();
	void poseNodeAtP(ofNode* n, float p);


private:
	



private:
	// trace target
	ofNode* node;
	
	// trace
	traceMode TraceMode;
	struct nodeTrace
	{
		ofPolyline ScaleL;
		ofPolyline PosL;
		deque<ofQuaternion> RotL;
		ofPolyline P;		
	}Trace;

	// temp
	float lastTime;
	ofVec3f lastScale;
	ofVec3f lastPos;
	ofQuaternion lastRot;

	// parameters
	ofParameter<float> WtPos, WtRot, WtScale, WtTime;
	void WtPosChanged(float& value);
	void WtRotChanged(float& value);
	void WtScaleChanged(float& value);
	void WtTimeChanged(float& value);
	
	ofParameterGroup parameters;
	

};


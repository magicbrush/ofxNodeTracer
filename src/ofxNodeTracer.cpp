#include "ofxNodeTracer.h"


ofxNodeTracer::ofxNodeTracer( 
	ofNode* n, 
	float wtPos/*=1.0f*/, 
	float wtRot/*=0.0f*/, 
	float wtScale /*= 0.0f*/, 
	float wtTime/*=0.0f */,
	traceMode TMode /* = GLOBAL */):
node(n),
TraceMode(TMode),
lastTime(0),
lastScale(ofVec3f(0)),
lastPos(ofVec3f(0)),
lastRot(ofQuaternion())
{
	parameters.setName("ofxNodeTracer");
	parameters.add(WtPos.set("WeightOfPosition",wtPos));
	parameters.add(WtRot.set("WeightOfRotaion",wtRot));
	parameters.add(WtScale.set("WeightOfScale",wtScale));
	parameters.add(WtTime.set("WeightOfTime",wtTime));
}

void ofxNodeTracer::linkNode( ofNode* n )
{
	node = n;
	clear();
}

void ofxNodeTracer::trace()
{
	float TimeNow = ofGetElapsedTimef();
	ofVec3f S,SL;
	ofVec3f P,PL;
	ofQuaternion Q,QL;
	if(TraceMode==GLOBAL)
	{
		S = node->getGlobalScale();
		P = node->getGlobalPosition();
		Q = node->getGlobalOrientation();	
	}	
	SL = node->getScale();
	PL = node->getPosition();
	QL = node->getOrientationQuat();
	
	ofVec3f SC,PC;
	ofQuaternion QC;
	if(TraceMode==GLOBAL)
	{
		SC = S;
		PC = P;
		QC = Q;
	}
	else
	{
		SC = SL;
		PC = PL;
		QC = QL;
	}

	Trace.ScaleL.addVertex(SL);
	Trace.PosL.addVertex(PL);
	Trace.RotL.push_back(Q);
	if(Trace.P.size()==0)
	{
		Trace.P.addVertex(0,0);
	}
	else
	{		
		float pp,pr,ps,pt;
		
		pp = WtPos*(PC-lastPos).length();
		
		float ang;
		ofVec3f ax;
		ofQuaternion DQ = QC-lastRot;
		DQ.getRotate(ang,ax);
		pr = WtRot*ang;
		
		ofVec3f ds = SC- lastScale;
		ps = WtScale*(abs(ds.x)+abs(ds.y)+abs(ds.z));
		
		pt = WtTime*(TimeNow-lastTime);	

		float dp = pp+pr+ps+pt;
		float lp = Trace.P[Trace.P.size()-1].x;
		float cp = lp+dp;
		Trace.P.addVertex(cp,0);
	}

	lastTime = TimeNow;
	lastScale = SC;
	lastPos = PC;
	lastRot = QC;	
}

void ofxNodeTracer::clear()
{
	Trace.ScaleL.clear();
	Trace.PosL.clear();
	Trace.RotL.clear();
	Trace.P.clear();
	
	lastTime = 0;
	lastScale = ofVec3f(0);
	lastPos = ofVec3f(0);
	lastRot = ofQuaternion();
}

void ofxNodeTracer::poseNodeAtP( ofNode* n, float p )
{
	int vtNum = Trace.P.size();
	float pmax = Trace.P.getLengthAtIndex(vtNum-1);
	float pc = ofClamp(p,0,pmax);
	float id = Trace.P.getIndexAtLength(pc);

	ofVec3f scl,pos;
	ofQuaternion quat;
	scl = Trace.ScaleL.getPointAtIndexInterpolated(id);
	pos = Trace.PosL.getPointAtIndexInterpolated(id);
	
	float idf,idc;
	idf =  floorf(id);
	idc = floorf(id+1);
	float s = (id-idf)/(idc-idf);
	ofQuaternion Qf,Qc;
	Qf = Trace.RotL.at(idf);
	Qc = Trace.RotL.at(idc);	
	quat.slerp(s,Qf,Qc);

	ofMatrix4x4 Mat;
	Mat.translate(pos);
	Mat.scale(scl);
	Mat.setRotate(quat);

	ofNode* parent = node->getParent();
	n->setParent(*parent);
	n->setTransformMatrix(Mat);
}

void ofxNodeTracer::WtPosChanged( float& value )
{
	clear();
}

void ofxNodeTracer::WtRotChanged( float& value )
{
	clear();
}

void ofxNodeTracer::WtScaleChanged( float& value )
{
	clear();
}

void ofxNodeTracer::WtTimeChanged( float& value )
{
	clear();
}

void ofxNodeTracer::setTraceMode( traceMode tmode )
{
	TraceMode = tmode;
	clear();
}

float ofxNodeTracer::getMaxP() const
{
	if(Trace.P.size()==0)
	{
		return 0;
	}
	float p = Trace.P[Trace.P.size()-1].x;
	return p;
}

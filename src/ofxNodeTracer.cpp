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
	parameters.add(WtPos.set("WeightOfPosition",wtPos,0.0f,10.0f));
	parameters.add(WtRot.set("WeightOfRotaion",wtRot,0.0f,10.0f));
	parameters.add(WtScale.set("WeightOfScale",wtScale,0.0f,10.0f));
	parameters.add(WtTime.set("WeightOfTime",wtTime,0.0f,10.0f));
	parameters.add(SmoothSize.set("SmoothSize",10,0,100));	
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

	if(Trace.ScaleL.size()==0)
	{
		Trace.ScaleL.addVertex(SL);
	}
	else
	{
		Trace.ScaleL.curveTo(SL,SmoothSize);
	}
	
	if(Trace.PosL.size()==0)
	{
		Trace.PosL.addVertex(PL);
	}
	else
	{
		Trace.PosL.curveTo(PL,SmoothSize);
	}	
	

	Trace.RotL.curveTo(Q.asVec4(),SmoothSize);
	
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
		Trace.P.curveTo(ofPoint(cp,0,0),SmoothSize);
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
	/*ofPolyline TP = Trace.P.getSmoothed(SmoothSize,SmoothShape);
	ofPolyline TScl = Trace.ScaleL.getSmoothed(SmoothSize,SmoothShape);
	ofPolyline TPos = Trace.PosL.getSmoothed(SmoothSize,SmoothShape);*/

	ofPolyline& TP = Trace.P;
	ofPolyline& TScl = Trace.ScaleL;
	ofPolyline& TPos = Trace.PosL;
	ofPolyline& TRot = Trace.RotL;

	int vtNum = TP.size();
	float pmax = TP.getLengthAtIndex(vtNum-1);
	float pc = ofClamp(p,0,pmax);
	float id = TP.getIndexAtLength(pc);	

	ofVec3f scl,pos,quatv;
	ofQuaternion quat;
	scl = TScl.getPointAtIndexInterpolated(id);
	pos = TPos.getPointAtIndexInterpolated(id);
	
	float idf,idc;
	idf =  floorf(id);
	idc = floorf(id+1);
	float s = (id-idf)/(idc-idf);
	
	//Qf = Trace.RotL.at(idf);
	//Qc = Trace.RotL.at(idc);	
	quatv = TRot.getPointAtIndexInterpolated(id);
	quat.set(quatv);

	ofMatrix4x4 Mat;	
	Mat.scale(scl);
	Mat.setRotate(quat);
	Mat.translate(pos);

	ofNode* parent = node->getParent();
	n->setParent(*parent);
	//n->setTransformMatrix(Mat);
	n->setScale(scl);
	n->setOrientation(quat);
	n->setPosition(pos);
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

	float p = Trace.P.getLengthAtIndex(Trace.P.size()-1);
	//float p = Trace.P[Trace.P.size()-1].x;
	return p;
}

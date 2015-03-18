#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "cocos2d.h"
#include "GameConst.h"
#include "GameEnum.h"

using namespace cocos2d;


cocos2d::Point  convertA1ToA2(Point centerPoint,cocos2d::Point  paopaoPos, float angle)
{
	float x = paopaoPos.x;
	float y = paopaoPos.y;
	//CCPoint tmpPoint = ccp(x-_centerPoint.x, y-_centerPoint.y);
	float angle1 =0;
	if((x-centerPoint.x) == 0)
	{
		if(y-centerPoint.y>0) 
			angle1 = 90;
		else if(y-centerPoint.y<0){
			angle1 = 270;
		}
	}
	else{
		angle1 = atan((y-centerPoint.y) / (x-centerPoint.x) );
		//float angle2 = angle1 * 180/PI;
		angle1 = CC_RADIANS_TO_DEGREES(angle1);

	}
	if(x-centerPoint.x < 0.0)
	{
		angle1 = angle1 + 180;
	}
	float r = sqrt(pow(y-centerPoint.y, 2) + pow(x-centerPoint.x, 2) );
	float x1 = r * cos( CC_DEGREES_TO_RADIANS(  angle1 + angle)  );
	float x11= x1	+ centerPoint.x;
	float y1 = r * sin( CC_DEGREES_TO_RADIANS( angle1 + angle ) );
	float y11 = y1 + centerPoint.y;
cocos2d::Point  truePoint = Point(x11, y11);
	
	return truePoint;
}

float trianglearea(float x1,float y1,float x2,float y2,float x3,float y3) {//三点求三角形面积
	float temp = (x2*y3-y2*x3+x3*y1-y3*x1+x1*y2-x2*y1);
	return abs(1.0/2*temp);	
}

float dist(float x1, float y1, float x2, float y2){//两点距离
	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
float distance(float x1, float y1,float x2,float y2,float x3, float y3){ //点(x1,y1)到直线(x2,y2,x3,y3)的距离
	float temp1 = trianglearea(x1,y1,x2,y2,x3,y3);
	float temp2 = dist(x2,y2,x3,y3);
	return 2*temp1/temp2;
}
cocos2d::Point chuizu(float x1,float y1,float x2,float y2,float x3,float y3){//点(x1,y1)到直线(x2,y2,x3,y3)的垂足
	if(x2 == x3) {//竖着的一直线
		return Point(x2,y1);
	} else if(y2 == y3){//横着的一直线
		return Point(x1,y2);
	} 
	else 
	{
		float k = (y3-y2)/(x3-x2);
		float xx = (k*k*x2 + k * (y1-y2) + x1)/(k*k+1);
		float yy = k*(xx - x2) + y2;
		return Point(xx,yy);
	}
}


int sign(float x)
{
	if(x>0.0)
		return 1;
	else
		return -1;
}

#endif
#ifndef __AssistLine_H__
#define __AssistLine_H__
#include "cocos2d.h"
using namespace cocos2d;

class LineBubbleSprite;


class AssistLine : public Layer
{
public:
	AssistLine(void);
	virtual ~AssistLine(void);
	virtual bool init(); 
	CREATE_FUNC(AssistLine);


	void showLine(cocos2d::Point start, float angle);

	//scheduleUpdate每帧调用  
	virtual void update(float delta);  

	void stopScheduleUpdate();

private:
	std::vector<LineBubbleSprite *> _bubbleList;

	LineBubbleSprite* newLineBubble();

	float _angle;
	cocos2d::Point _endLinePoint;
	cocos2d::Point _startLinePoint;
	int _loopInterval;
};

#endif // !__AssistLine_H__

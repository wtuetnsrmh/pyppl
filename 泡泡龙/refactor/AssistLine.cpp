#include "AssistLine.h"
#include "Bubble.h"
#include "Global.h"
#include "LineBubbleSprite.h"
AssistLine::AssistLine(void)
{
	
	
}

AssistLine::~AssistLine(void)
{

}

bool AssistLine::init()  
{  
	//////////////////////////////  
	// 1. super init first  
	if ( !Layer::init() )  
	{  
		return false;  
	}  
	
	for(int i=0; i<10; i++){

		LineBubbleSprite *pp = newLineBubble();
		pp->setVisible(false);
	}

	return true;  
} 

void AssistLine::showLine(cocos2d::Point start, float degree)
{
	_loopInterval = 0;
	this->unscheduleUpdate();
	MyRect gf = g_gameLayer->getGameFrame();
	int step = PAOPAO_SIZE * 5 /6;
	
	size_t index = 0;
	if(abs(degree) <  1e-4)
		return;
	float angle = CC_DEGREES_TO_RADIANS(degree);
	_angle = angle;
	bool wallCollision1 = false;
	bool wallCollision2 = false;
	bool wallCollision3 = false;

	float preX = start.x ;
	float preY =  start.y ;
	float nextX = 0;
	float nextY = 0;
	do{
		if(wallCollision1 + wallCollision2 + wallCollision3>= 2)
			break;
		nextX = start.x - sin(angle) * step * (index+1);
		nextY = start.y - abs(cos(angle) * step * (index+1));
		
		cocos2d::Point originalPoint(nextX, nextY);

		if(nextX <= gf.x + PAOPAO_SIZE/2 ){
			nextX = nextX + 2 * (gf.x-nextX) ;
			wallCollision1 = true;
			
		}
		if(nextX >= gf.x + gf.width - PAOPAO_SIZE/2 ){
			nextX = nextX - 2 * (nextX - (gf.x + gf.width) );
			wallCollision2 = true;
			
		}
		if(nextY <= gf.y+PAOPAO_SIZE){
			//nextY = nextY + 2 * (gf.y - nextY);
			wallCollision3 = true;
			break;
		}

	
		if (g_gameLayer->isCollisionBubble(preX, preY, nextX, nextY))
		{
			break;
		}

		if(index<_bubbleList.size()){
			_bubbleList.at(index)->setPosition(cocos2d::Point(nextX, nextY));
			_bubbleList.at(index)->setVisible(true);
			
			_bubbleList.at(index)->setOriginalPoint(originalPoint);
		}
		else
		{
			LineBubbleSprite *pp = newLineBubble();
			pp->setPosition(cocos2d::Point(nextX, nextY ));
			pp->setVisible(true);

			pp->setOriginalPoint(originalPoint);
		}
		index++;

		if(index == 1){
			_startLinePoint.x = nextX;
			_startLinePoint.y = nextY;
		}

		preX = nextX;
		preY = nextY;

	}while(true);

	_endLinePoint.x = nextX;
	_endLinePoint.y = nextY;

	//for (size_t i=index; i<_bubbleList.size(); i++)
	//{
	//	_bubbleList.at(i)->setVisible(false);
	//}

	for (size_t i=_bubbleList.size()-1; i>index; i--)
	{
		LineBubbleSprite * bb = _bubbleList.at(i);
		this->removeChild(bb);
		_bubbleList.erase( _bubbleList.begin() + i);

	}

	this->scheduleUpdate();
}

void AssistLine::stopScheduleUpdate()
{

	this->unscheduleUpdate();
}

void AssistLine::update(float delta)
{
	if(_loopInterval % 3 == 0){
		MyRect gf = g_gameLayer->getGameFrame();
		float maxY = 0;
		float xByMaxY = 0;
		for (size_t i=0; i<_bubbleList.size(); i++)
		{
			LineBubbleSprite *bb = _bubbleList.at(i);
			if(bb->isVisible() == true)
			{
				float nextX = bb->getOriginalPoint().x - sin(_angle) * 5;
				float nextY =  bb->getOriginalPoint().y - abs(cos(_angle) * 5);

				if(maxY < nextY ){
					maxY = nextY ;
					xByMaxY = nextX;
				}
			}
		}

		for (size_t i=0; i<_bubbleList.size(); i++)
		{
			LineBubbleSprite *bb = _bubbleList.at(i);
			if(bb->isVisible() == true)
			{
				float nextX =  bb->getOriginalPoint().x - sin(_angle) * 5;
				float nextY =  bb->getOriginalPoint().y - abs(cos(_angle) * 5);

				bb->setOriginalPoint(cocos2d::Point(nextX, nextY));

				if(nextX <= gf.x + PAOPAO_SIZE/2 ){
					nextX = nextX + 2 * (gf.x-nextX) ;
				}
				if(nextX >= gf.x + gf.width - PAOPAO_SIZE/2 ){
					nextX = nextX - 2 * (nextX - (gf.x + gf.width) );

				}
				if(nextY <= _endLinePoint.y){
					//bb->setPositionX(_startLinePoint.x);
					//bb->setPositionY(_startLinePoint.y);
					float preX = xByMaxY + sin(_angle) * PAOPAO_SIZE * 5 /6;
					float preY =  maxY + abs(cos(_angle) * PAOPAO_SIZE * 5 /6);
					bb->setOriginalPoint( cocos2d::Point(preX, preY) );
					bb->setPositionX(preX);
					bb->setPositionY(preY);
				}
				else{
					bb->setPositionX(nextX);
					bb->setPositionY(nextY);
				}
			}
		}
	}
	_loopInterval++;
}


LineBubbleSprite* AssistLine::newLineBubble()
{
	LineBubbleSprite *pp = LineBubbleSprite::initWithPic("monster/assist_line.png");
	_bubbleList.push_back(pp);

	this->addChild(pp);

	pp->setScale(PIC_RADIO/4);

	//pp->playNormalAction(true);
	return pp;
}
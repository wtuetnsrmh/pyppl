#include "LineBubbleSprite.h"

LineBubbleSprite::LineBubbleSprite()
{
	_xRefraction = _yRefraction = 0;
}

LineBubbleSprite::~LineBubbleSprite()
{
}

LineBubbleSprite * LineBubbleSprite::initWithPic(std::string pic)
{  
	LineBubbleSprite *pRet = LineBubbleSprite::create();  
	if (pRet && pRet->initWithFile(pic)) //初始化判断  
	{  
		
	}  
	else  
	{  
		CC_SAFE_RELEASE(pRet);  
		pRet = NULL;  
	}  
	return pRet;  
} 

void LineBubbleSprite::setOriginalPointY(float y)
{
	this->_originalPoint.y = y;
}
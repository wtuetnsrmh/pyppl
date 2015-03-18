#include "EffectSprite.h"

EffectSprite::EffectSprite()
{
	_type = 0;
}


EffectSprite::~EffectSprite()
{
}

EffectSprite * EffectSprite::initWithType(int type) //根据传过来的类型来生成泡泡  
{  
	EffectSprite *pRet = EffectSprite::create();  
	if (pRet && pRet->initWithFile(pRet->getStringByType(type))) //初始化判断  
	{  
		pRet->setType(type);
	}  
	else  
	{  
		CC_SAFE_RELEASE(pRet);  
		pRet = NULL;  
	}  
	return pRet;  
}

std::string EffectSprite::getStringByType(int type) //不同类型获取不同的图片资源  
{  
    std::string pRet = "";  
	char s[32];
	
	sprintf(s, "effect/extra/%d/1.png", type);
	pRet = s;

    return pRet;  
}

void EffectSprite::playEffectOnce(float delay)
{
	int picCount = 4;
	if(_type == 3)
		picCount = 11;
	auto ea = Animation::create();
	for(int i=1; i<=picCount; i++){
		char s1[64];
		sprintf(s1,"effect/extra/%d/%d.png", _type, i);

		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	this->runAction( Animate::create(ea) );
}

void EffectSprite::playEffectForEver(float delay)
{
	int picCount = 4;
	if(_type == 3)
		picCount = 16;
	auto ea = Animation::create();
	for(int i=1; i<=picCount; i++){
		char s1[64];
		sprintf(s1,"effect/extra/%d/%d.png", _type, i);

		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	this->runAction( RepeatForever::create(Animate::create(ea) ) );
}
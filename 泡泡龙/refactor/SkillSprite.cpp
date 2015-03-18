#include "SkillSprite.h"

SkillSprite::SkillSprite()
{

}

SkillSprite::~SkillSprite()
{

}

SkillSprite * SkillSprite::initWithType(int type)
{
	char s[64];
	sprintf(s, "skill/%d_01.png", type);
	SkillSprite *pRet = SkillSprite::create();
	if (pRet && pRet->initWithFile(s)) //初始化判断  
	{  
		pRet->setSkillType(type);
		return pRet;
	}
	else  
	{  
		CC_SAFE_RELEASE(pRet);  
		pRet = NULL;  
	}  

	return pRet;

}

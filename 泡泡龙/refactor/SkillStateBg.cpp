#include "SkillStateBg.h"

SkillStateBg::SkillStateBg()
{

}

SkillStateBg::~SkillStateBg()
{

}

SkillStateBg * SkillStateBg::initWithState(int state)
{
	char s[64];
	if (state == 0)
	{
		//sprintf(s, "skill/lock.png");
        sprintf(s, "skill/lock.png");
	}
	else{
		//sprintf(s, "skill/bg.png");
        sprintf(s, "skill/bg.png");
	}
	SkillStateBg *pRet = SkillStateBg::create();
	if (pRet && pRet->initWithFile(s)) 
	{  
		pRet->setBgState(state);
		return pRet;
	}
	else  
	{  
		CC_SAFE_RELEASE(pRet);  
		pRet = NULL;  
	}  

	return pRet;
}

void SkillStateBg::changeState(int state)
{
	if(state != this->getBgState()){
		char s[64];
		if (state == 0)
		{
			sprintf(s, "skill/lock.png");
		}
		else{
			sprintf(s, "skill/bg.png");
		}
		Texture2D *textture;  
		Sprite * temp_obj = Sprite::create(s);  
		textture = temp_obj->getTexture();  
		this->stopAllActions();  

		this->setTexture(textture); 

		this->setBgState(state);
	}
}
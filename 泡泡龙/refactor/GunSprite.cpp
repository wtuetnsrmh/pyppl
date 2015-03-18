#include "GunSprite.h"
#include "Global.h"

GunSprite::GunSprite(void)
{
	_normalAnimation = Animation::create();
}


GunSprite::~GunSprite(void)
{
}


GunSprite * GunSprite::initWithPic(std::string pic ) 
{  
    GunSprite *pRet = GunSprite::create();  
    if (pRet && pRet->initWithFile(pic)) //初始化判断  
    {  
		//pRet->startAction();
    }  
    else  
    {  
        CC_SAFE_RELEASE(pRet);  
        pRet = NULL;  
    }  
    return pRet;  
}  


void GunSprite::startAction()
{
	if(this->_normalAnimation->getFrames().size() == 0)
	{
		for(int i=1; i<=8; i++){
			char s1[64];
			sprintf(s1,"gun/%d.png",i);
			this->_normalAnimation->addSpriteFrameWithFile(s1);
		}
		this->_normalAnimation->setDelayPerUnit(0.2f);
	}

	this->runAction(CCRepeatForever::create( CCAnimate::create(_normalAnimation) ));
}
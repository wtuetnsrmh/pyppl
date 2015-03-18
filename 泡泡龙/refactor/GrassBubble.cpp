#include "GrassBubble.h"
#include "GameConst.h"
#include "Global.h"

GrassBubble::GrassBubble(void)
{
}

GrassBubble::~GrassBubble(void)
{
}

GrassBubble * GrassBubble::initWithType(GrassBubble_Type type) //根据传过来的类型来生成泡泡  
{  
    GrassBubble *pRet = GrassBubble::create();  
    if (pRet && pRet->initWithFile(getStringByType(type))) //初始化判断  
    {  
        
		pRet->setBubbleType(type); 
		
		cocos2d::Size  sz = pRet->getContentSize();
		pRet->setScale(PAOPAO_SIZE/sz.width);
		pRet->ignoreAnchorPointForPosition(false);
		
    }  
    else  
    {  
        CC_SAFE_RELEASE(pRet);  
        pRet = NULL;  
    }  

    return pRet;  
}
std::string GrassBubble::getStringByType(GrassBubble_Type type) //不同类型获取不同的图片资源  
{  
    std::string pRet = "";  
	char s[32];
	
	sprintf(s, "grass/%d-01.png", static_cast<int>(type));
	pRet = s;

    return pRet;  
}
void GrassBubble::playEffect(int type, float delay)
{
    char s1[64];
	sprintf(s1,"effect/bubble/%d/%d.png", type, 1);
	_effectSprite = Sprite::create(s1);
	this->addChild(_effectSprite,0);
	auto ea = Animation::create();
	for(int i=1; i<=16; i++){
		sprintf(s1,"effect/bubble/%d/%d.png", type, i);
        
		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	_effectSprite->runAction( RepeatForever::create(Animate::create(ea) ) );
	
	_effectSprite->setPosition(this->getContentSize().width/2,  this->getContentSize().height/2);
}
void GrassBubble::playEffectOnce(int type, float delay)
{
	char s1[64];
	sprintf(s1,"effect/bubble/%d/%d.png", type, 1);
	_effectSprite = Sprite::create(s1);
	this->addChild(_effectSprite,0);
	auto ea = Animation::create();
	for(int i=1; i<=16; i++){
		sprintf(s1,"effect/bubble/%d/%d.png", type, i);
        
		ea->addSpriteFrameWithFile(s1);
	}
	ea->setDelayPerUnit(delay);
	_effectSprite->runAction(Animate::create(ea));
	
	_effectSprite->setPosition(this->getContentSize().width/2,  this->getContentSize().height/2);
}
void GrassBubble::setBubbleType(GrassBubble_Type type)
{
	this->_bubbleType = type;
}

GrassBubble_Type GrassBubble::getBubbleType()
{
	return _bubbleType;
}
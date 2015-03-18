#include "HelpSprite.h"

HelpSprite::HelpSprite(void)
{
}
HelpSprite::~HelpSprite(void)
{
}
void HelpSprite::initWithType(int type)  
{  
	char s[64];
	sprintf(s, "help/%d.png", type);
	_sp = Sprite::create(s);

	this->setHelpType(type);
	this->addChild(_sp); 
	this->setContentSize(cocos2d::Size(1416,843));
}
void HelpSprite::changeType(int type)
{
	this->setHelpType(type);
	char sn[32];
	sprintf(sn, "help/%d.png", type);

	Texture2D *textture;  

	Sprite * temp_obj = Sprite::create(sn);  

	textture = temp_obj->getTexture();  

	this->stopAllActions();  

	_sp->setTexture(textture); 
}
#include "AlertDialog.h"

#include "Global.h"
using namespace cocos2d::extension;
AlertDialog::AlertDialog(void)
{
}


AlertDialog::~AlertDialog(void)
{
}

bool AlertDialog::init()  
{  
	//////////////////////////////  
	// 1. super init first  
	if ( !Layer::init() )  
	{  
		return false;  
	}
cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	_background = Sprite::create("ui/0-01.png");
cocos2d::Size sz = _background->getContentSize();
	//_background->setScale(visibleSize.width/sz.width, visibleSize.height/sz.height);
	_background->setAnchorPoint(cocos2d::Point(0,0));
	_background->setPosition(cocos2d::Point(origin.x, origin.y));
	this->addChild(_background, 0);

	return true;  
}


void AlertDialog::setText(int type)
{
	//Label *label = Label::create(text,"font/Marker Felt.ttf",20);
	//_bossHp = Label::create();
	//label->setColor(Color3B(255,255,255));
	//_background->addChild(label);
//cocos2d::Size sz = _background->getContentSize();
	//label->setPosition(cocos2d::Point( (sz.width-label->getContentSize().width)/2, (sz.height-label->getContentSize().height)/2 ) );

	char s[64];
	sprintf(s,"ui/%d-01.png",type);
	Texture2D *textture;  
	Sprite *temp_obj= Sprite::create(s); 
	textture = temp_obj->getTexture(); 
	_background->setTexture(textture);

cocos2d::Point pt = this->getPosition();
auto move = MoveTo::create(0.3f, cocos2d::Point(pt.x, pt.y+100));

	auto move_bounce = EaseOut::create(move, 2.0f);
	

	auto seq = Sequence::create(move, DelayTime::create(2.0f), CallFuncN::create(this, callfuncN_selector(AlertDialog::removeMe)), NULL);

	this->runAction(seq);
}

void AlertDialog::removeMe(Node *obj)
{
	if (obj != NULL)
	{
		g_miscLayer->removeChild(obj, true);
	}
}
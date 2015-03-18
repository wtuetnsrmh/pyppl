#include "GameBgLayer.h"
#include "extensions/cocos-ext.h"
#include "Global.h"
using namespace cocos2d::extension;
GameBgLayer::GameBgLayer(void)
{
}


GameBgLayer::~GameBgLayer(void)
{
}

bool GameBgLayer::init()  
{  
	//////////////////////////////  
	// 1. super init first  
	if ( !Layer::init() )  
	{  
		return false;  
	}
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	Sprite *background = Sprite::create("game_bg.jpg");
cocos2d::Point  anch =background->getAnchorPoint();
cocos2d::Size  sz = background->getContentSize();
	background->setScale(visibleSize.width/sz.width, visibleSize.height/sz.height);
	background->setPosition(cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	this->addChild(background, 0);

	g_miscLayer->playBackgroundMusic("bg.wav" );
	
	return true;  
}  

void GameBgLayer::addGameFrame(int x, int y, int width, int height)
{
	auto sprite = Scale9Sprite::create("game_outframe.png");
	sprite->setPreferredSize(cocos2d::Size(width, height));
	sprite->setContentSize(cocos2d::Size(width, height));
	this->addChild(sprite);
	sprite->setPosition(cocos2d::Point(x+width/2, y+height/2));
}

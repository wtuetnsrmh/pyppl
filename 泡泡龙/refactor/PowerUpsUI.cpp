#include "PowerUpsUI.h"
#include "Global.h"
#include "PowerUpsView.h"
#include <iostream>
using namespace std;
PowerUpsUI::PowerUpsUI(void)
{
}


PowerUpsUI::~PowerUpsUI(void)
{
}

bool PowerUpsUI::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
    LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));
    
	this->addChild(lc, 0);
	//±≥æ∞
	Sprite *bg = Sprite::create("ui/powerups/bg.png");
cocos2d::Size bgSz = bg->getContentSize();
	cocos2d::Point center = cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
	bg->setPosition(center);
	this->addChild(bg);

	//Sprite *iconBg = Sprite::create("ui/powerups/iconBg.png");
	//cocos2d::Size iconbgSz = iconBg->getContentSize();
	
	//iconBg->setPosition(ipo);
	//this->addChild(iconBg);

	//_nowIcon=Sprite::create("power/2-01.png");
    //cocos2d::Point ipo = cocos2d::Point(center.x-bgSz.width/2+64+_nowIcon->getContentSize().width/2,center.y+bgSz.height/2-_nowIcon->getContentSize().height);
	//_nowIcon->setPosition(ipo);
	//this->addChild(_nowIcon);

	//_nowName=Label::create("","font/Marker Felt.ttf",60);
	//_nowName->setColor(Color3B(255,255,0));
	//_nowName->enableOutline(Color4B(0,0,0,255),2);
	//_nowName->setPosition(ipo.x+_nowIcon->getContentSize().width,ipo.y+60);

	//this->addChild(_nowName);

	//_nowDes=Label::create("","font/Marker Felt.ttf",60);
	//_nowDes->setColor(Color3B(255,255,255));
	//_nowDes->enableOutline(Color4B(0,0,0,255),2);
	//_nowDes->setPosition(ipo.x+_nowIcon->getContentSize().width,ipo.y-60);
	//this->addChild(_nowDes);

	_tableView = PowerUpsView::create();
	this->addChild(_tableView);
	
	_tableView->setPosition(center.x-bgSz.width/2+64,center.y-bgSz.height/2+365);

	//back
	Button *back = Button::create();
	back->setTouchEnabled(true);
	back->setTag(UP_BACKBTN);
	back->loadTextures("ui/powerups/play1.png","ui/powerups/play2.png","");
	this->addChild(back);
	back->setPosition(cocos2d::Point(center.x,center.y-bgSz.height/2+back->getContentSize().height));
	back->addTouchEventListener(this, toucheventselector(PowerUpsUI::touchEvent));

    if(visibleSize.width<IPADWIDTH)
    {
        this->setScale( visibleSize.width / IPADWIDTH ,visibleSize.height / IPADHEIGHT);
        
    }

	this->updateNowPower(TT_AssistLine,"Assistant Ray","Help you to aim precisely", 0);
    this->setNowPower(TT_AssistLine);
	return true;
}
void PowerUpsUI::updateNowPower(Talent_Type type,std::string name,std::string des, int itemIndex)
{
	int t =type;
	//Texture2D *textture;
	//char s[64];
	//sprintf(s,"power/%d-01.png",t);
	//Sprite * temp_obj = Sprite::create(s);
	//temp_obj->setPosition(20,20);
	//textture = temp_obj->getTexture();
	//_nowIcon->setTexture(textture);
	//_nowName->setString(name);
	//_nowDes->setString(des);

	_tableView->updateItems(itemIndex);

}
void PowerUpsUI::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		g_miscLayer->playWav("btn.wav");
		g_miscLayer->removeChild(this);
		g_gameLayer->gameStart();
		break;
	}
}
void PowerUpsUI::setNowPower(Talent_Type type)
{
	_nowPower = type;
	g_gameLayer->setNowPowerType(_nowPower);
}
void PowerUpsUI::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PowerUpsUI::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(PowerUpsUI::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(PowerUpsUI::onTouchEnded,this);
	listener->setSwallowTouches(true);//≤ªœÚœ¬¥´µ›¥•√˛
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
}

void PowerUpsUI::onExit()
{
	Layer::onExit();
}

bool PowerUpsUI::onTouchBegan (Touch *touch, Event *unused_event)
{
    return true;
}
void PowerUpsUI::onTouchMoved (Touch *touch, Event *unused_event)
{  
}
void PowerUpsUI::onTouchEnded (Touch *touch, Event *unused_event)
{ 
}
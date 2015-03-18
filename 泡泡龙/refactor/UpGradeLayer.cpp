#include "UpGradeLayer.h"
#include "Global.h"
#include "TalentView.h"
#include "MyButton.h"
#include "CoinShopUI.h"
#include <iostream>
UpGradeLayer::UpGradeLayer(void)
{
}


UpGradeLayer::~UpGradeLayer(void)
{
}

bool UpGradeLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
    LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));
    
	this->addChild(lc, 0);
	//背景
	Sprite *bg = Sprite::create("ui/store/storeBg.png");
	cocos2d::Size bgSz = bg->getContentSize();
	cocos2d::Point center = cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
	bg->setPosition(center);
	this->addChild(bg);
    Sprite *name = Sprite::create("ui/store/UPGRADE.png");
    name->setPosition(center.x,center.y+bgSz.height/2-name->getContentSize().height/2-40);
    this->addChild(name);

	_talentView = TalentView::create();
	this->addChild(_talentView);
	
	_talentView->setPosition(center.x-bgSz.width/2+64,center.y-bgSz.height/2+374);


	cocos2d::Point bgPo = cocos2d::Point(center.x-470,center.y+bgSz.height/2-350);

	_goldLabel = Label::create("","Copperplate Gothic Bold.ttf",80);
	_goldLabel->setColor(Color3B(255,255,255));
    //_goldLabel->enableOutline(Color4B(0,0,0,255),5);
     _goldLabel->setAnchorPoint(cocos2d::Point(0,0));
	_goldLabel->setPosition(bgPo.x,bgPo.y);
	this->addChild(_goldLabel);
	this->updateGold(g_goodsData->gold);

	//加金币按钮
	//ui::Button *button= ui::Button::create();
	//button->setTouchEnabled(true);
	//button->loadTextures("shop/jia1.png", "shop/jia2.png", "");
	//button->setTag(UP_BUYCOINS);
	//button->addTouchEventListener(this, toucheventselector(UpGradeLayer::touchEvent));
	//this->addChild(button);
	//button->setPosition(cocos2d::Point(bgPo.x+sz.width/2-10,bgPo.y));


	//宝石
	//Sprite *gemBg=Sprite::create("upgrade/gem.png");
	//this->addChild(gemBg);
	//sz = gemBg->getContentSize();
	//gemBg->setPosition(center.x+sz.width/2,center.y+bgSz.height/2-93);

	bgPo = cocos2d::Point(center.x+180,center.y+bgSz.height/2-350);
	

	_gemLabel = Label::create("","Copperplate Gothic Bold.ttf",80);
	_gemLabel->setColor(Color3B(255,255,255));
   // _gemLabel->enableOutline(Color4B(0,0,0,255),5);
    _gemLabel->setAnchorPoint(cocos2d::Point(0,0));
	_gemLabel->setPosition(bgPo.x,bgPo.y);
	this->addChild(_gemLabel);
	this->updateGem(g_goodsData->gem);


	//get more coins
	ui::Button *buyCoins = ui::Button::create();
	buyCoins->setTouchEnabled(true);
	buyCoins->setTag(UP_GETCOINSBTN);
	buyCoins->loadTextures("ui/store/GET COINS1.png","ui/store/GET COINS2.png","");
	this->addChild(buyCoins);
	buyCoins->setPosition(cocos2d::Point(center.x-buyCoins->getContentSize().width/2-140,center.y-bgSz.height/2+buyCoins->getContentSize().height));
	buyCoins->addTouchEventListener(this, toucheventselector(UpGradeLayer::touchEvent));

	//back
	MyBtton *back = MyBtton::create();
	back->setTouchEnabled(true);
	back->setTag(UP_BACKBTN);
	back->loadTextures("ui/store/BACK1.png","ui/store/BACK2.png","");
	this->addChild(back);
	back->setPosition(cocos2d::Point(center.x+back->getContentSize().width/2+150,center.y-bgSz.height/2+back->getContentSize().height));
	back->addTouchEventListener(this, toucheventselector(UpGradeLayer::touchEvent));
    
    
    //clear
//	Button *clear = Button::create();
//	clear->setTouchEnabled(true);
//	clear->setTag(UP_Clear);
//	clear->loadTextures("ui/restart-1.png","ui/restart-2.png","");
//	this->addChild(clear);
//	clear->setPosition(cocos2d::Point(center.x,center.y-bgSz.height/2+150));
//	clear->addTouchEventListener(this, toucheventselector(UpGradeLayer::touchEvent));
    
    
    if(visibleSize.width<IPADWIDTH)
    {
        this->setScale( visibleSize.width / IPADWIDTH ,visibleSize.height / IPADHEIGHT);
        
    }
    
	return true;
}
void UpGradeLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		//std::cout << "UpGradeLayer touchEvent begin" << std::endl;
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		//std::cout << "UpGradeLayer touchEvent end" << std::endl;
		g_miscLayer->playWav("btn.wav");
		switch (tag)
		{
			case UP_BACKBTN:
				g_miscLayer->removeChild(this);
				break;
           // case UP_Clear:
            //    _talentView->updateItems();
                break;
		}

		break;
	}
	
	if(tag==UP_BUYCOINS)
	{
		if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
		{
			g_coinShopUI = CoinShopUI::create();
			g_miscLayer->addChild(g_coinShopUI);
			g_coinShopUI->setTag(COINHELPUI);
		}
	}else if(tag==UP_GETCOINSBTN)
	{
		if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
		{
			g_coinShopUI = CoinShopUI::create();
			g_miscLayer->addChild(g_coinShopUI);
			g_coinShopUI->setTag(COINHELPUI);
		}
	}
}
void UpGradeLayer::updateGold(int gold)
{
	char si[64];
	sprintf(si, "%d", gold);
	_goldLabel->setString(si);

}
void UpGradeLayer::updateGem(int gem)
{
	char si[64];
	sprintf(si, "%d", gem);
	_gemLabel->setString(si);
}
void UpGradeLayer::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(UpGradeLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(UpGradeLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(UpGradeLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}

void UpGradeLayer::onExit()
{
	Layer::onExit();
	
}

bool UpGradeLayer::onTouchBegan (Touch *touch, Event *unused_event)
{
    return true;
}
void UpGradeLayer::onTouchMoved (Touch *touch, Event *unused_event)
{
    
}
void UpGradeLayer::onTouchEnded (Touch *touch, Event *unused_event)
{
    
}

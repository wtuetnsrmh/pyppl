#include "ShopLayer.h"
#include "Global.h"
#include "ShopGoodsSprite.h"
#include "CoinShopUI.h"

ShopLayer::ShopLayer(void)
{
}


ShopLayer::~ShopLayer(void)
{
}

bool ShopLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	} 

	//Director::getInstance()->getEventDispatcher()->
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	
	LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));

	this->addChild(lc, 0);

	//lc->getEventDispatcher()->

	//背景
	Sprite *bg = Sprite::create("shop/shopBg.png");
cocos2d::Size  bgSz = bg->getContentSize();
cocos2d::Point  center = cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
	bg->setPosition(center);
	this->addChild(bg);

	

	//金币
	Sprite *goldBg=Sprite::create("shop/goldBg.png");
	this->addChild(goldBg);
	goldBg->setPosition(center.x,center.y+280);
cocos2d::Size  sz = goldBg->getContentSize();
cocos2d::Point  bgPo = goldBg->getPosition();

	_goldLabel = Label::create("","font/Marker Felt.ttf",30);
	_goldLabel->setColor(Color3B(255,255,255));
    _goldLabel->enableOutline(Color4B(0,0,0,255),5);
	_goldLabel->setPosition(bgPo.x-40,bgPo.y);

	this->addChild(_goldLabel);
	this->updateGold(g_goodsData->gold);

	//加金币按钮
	ui::Button *button= ui::Button::create();  
	button->setTouchEnabled(true);
	button->loadTextures("shop/jia1.png", "shop/jia2.png", "");
	button->setTag(BUYCOINS);
	button->addTouchEventListener(this, toucheventselector(ShopLayer::touchButtonEvent));
	this->addChild(button);
	button->setPosition(cocos2d::Point(bgPo.x+sz.width/2-10,bgPo.y));


	//物品
	_index = GT_Earthquake;
	_goodsImg = ShopGoodsSprite::create();
	_goodsImg->initWithType(GT_Earthquake);
	this->addChild(_goodsImg);
	_goodsImg->setPosition(center.x,center.y);


	//buy
	ui::Button *buy = ui::Button::create();
	buy->setTouchEnabled(true);
	buy->setTag(BUYGOODSBTN);
	buy->loadTextures("shop/buy1.png","shop/buy2.png","");
	this->addChild(buy);
	buy->setPosition(cocos2d::Point(center.x,center.y-130));
	buy->addTouchEventListener(this, toucheventselector(ShopLayer::touchButtonEvent));

	//get more coins
	ui::Button *buyCoins = ui::Button::create();
	buyCoins->setTouchEnabled(true);
	buyCoins->setTag(GETCOINSBTN);
	buyCoins->loadTextures("shop/buyCoins1.png","shop/buyCoins2.png","");
	this->addChild(buyCoins);
	buyCoins->setPosition(cocos2d::Point(center.x-bgSz.width/2+165,center.y-bgSz.height/2+100));
	buyCoins->addTouchEventListener(this, toucheventselector(ShopLayer::touchButtonEvent));

	//back
	ui::Button *back = ui::Button::create();
	back->setTouchEnabled(true);
	back->setTag(BACKBTN);
	back->loadTextures("shop/back1.png","shop/back2.png","");
	this->addChild(back);
	back->setPosition(cocos2d::Point(center.x+bgSz.width/2-120,center.y-bgSz.height/2+100));
	back->addTouchEventListener(this, toucheventselector(ShopLayer::touchButtonEvent));

	//左右箭头
	ui::Button *leftBtn = ui::Button::create();
	leftBtn->loadTextures("shop/leftBtn.png","shop/leftBtn.png","");
	this->addChild(leftBtn);
	leftBtn->setTag(LEFTBTN);
	leftBtn->setPosition(cocos2d::Point(center.x-bgSz.width/2+70,center.y));
	//leftBtn->setVisible(false);
	leftBtn->setTouchEnabled(true);
	leftBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchButtonEvent));

	ui::Button *rightBtn = ui::Button::create();
	rightBtn->setTouchEnabled(true);
	rightBtn->setTag(RIGHTBTN);
	rightBtn->loadTextures("shop/rightBtn.png","shop/rightBtn.png","");
	this->addChild(rightBtn);
	rightBtn->setPosition(cocos2d::Point(center.x+bgSz.width/2-70,center.y));
	rightBtn->addTouchEventListener(this, toucheventselector(ShopLayer::touchButtonEvent));

	//this->setTouchEnabled(true);
	//this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);  

    if(visibleSize.width> UIWIDTH + 20)
    {
        this->setScale( visibleSize.width / UIWIDTH ,visibleSize.height / UIHEIGHT);
        
    }
    
	
	return true;
}
void ShopLayer::touchButtonEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		g_miscLayer->playWav("btn.wav");
		switch (tag)
		{
			case BUYGOODSBTN://买道具
				g_shopMgr->buyGoods(_index);
				break;
			case LEFTBTN://左
				if (_index>1)
				{
					ChangePage(--_index);
				}
				break;
			case RIGHTBTN://右
				if (_index<4)
				{
					ChangePage(++_index);
				}
				break;
			case BACKBTN://返回
				g_miscLayer->removeChild(this);
				break;
		}

		break;
	}

	if(tag==BUYCOINS)
	{
		if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
		{
			auto coinShopUI = CoinShopUI::create();
			g_miscLayer->addChild(coinShopUI);
			coinShopUI->setTag(COINHELPUI);
		}
	}else if(tag==GETCOINSBTN)
	{
		if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
		{
			auto coinShopUI = CoinShopUI::create();
			g_miscLayer->addChild(coinShopUI);
			coinShopUI->setTag(COINHELPUI);
		}
	}



}
void ShopLayer::ChangePage(int index)
{
	_goodsImg->changeType(index);
}
void ShopLayer::updateGold(int gold)
{
	char si[32];
	sprintf(si, "%d", gold);
	_goldLabel->setString(si);

}

void ShopLayer::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ShopLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(ShopLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(ShopLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//不向下传递触摸
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}
void ShopLayer::onExit()
{
	Layer::onExit();
	
}

 bool ShopLayer::onTouchBegan (Touch *touch, Event *unused_event)
 {
	 _touchBeganPoint = touch->getLocation();
	 return true;
 }
 void ShopLayer::onTouchMoved (Touch *touch, Event *unused_event)
 {
	 _touchMovePoint = touch->getLocation();
 }
 void ShopLayer::onTouchEnded (Touch *touch, Event *unused_event)
 {
	 _touchEndPoint = touch->getLocation();
	 if(_touchEndPoint.x>_touchBeganPoint.x+50)
	 {
		 if (_index>1)
		 {
			 ChangePage(--_index);
		 }

	 }else if(_touchEndPoint.x<_touchBeganPoint.x+50)
	 {
		 if (_index<4)
		 {
			 ChangePage(++_index);
		 }
	 }
 }

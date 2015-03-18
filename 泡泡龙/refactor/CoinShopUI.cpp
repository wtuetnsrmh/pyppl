#include "CoinShopUI.h"
#include "Global.h"
#include "CoinShopItem.h"
#include "CoinShopView.h"

CoinShopUI::CoinShopUI(void)
{
}


CoinShopUI::~CoinShopUI(void)
{
}

bool CoinShopUI::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	} 
	cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	
	LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));

	this->addChild(lc, 0);

    
   // SpriteFrameCache *cache = SpriteFrameCache::getInstance();
   // cache->addSpriteFramesWithFile("ui/store/store.plist","ui/store/store.png");
    
    
	Sprite *bg = Sprite::create("ui/store/storeBg.png");
	cocos2d::Size  bgSz = bg->getContentSize();
	cocos2d::Point  center = cocos2d::Point(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2);
	bg->setPosition(center);
	this->addChild(bg);
    Sprite *name = Sprite::create("ui/store/STORE.png");
    name->setPosition(center.x,center.y+bgSz.height/2-name->getContentSize().height/2-40);
    this->addChild(name);



	//Ω±“
	//Sprite *goldBg=Sprite::create("ui/coinShop/goldBg.png");
	//this->addChild(goldBg);
	//cocos2d::Size sz = goldBg->getContentSize();
	//goldBg->setPosition(center.x-sz.width/2,center.y+bgSz.height/2-93);
	cocos2d::Point bgPo =cocos2d::Point( center.x-470,center.y+bgSz.height/2-350);

	_goldLabel = Label::create("","Copperplate Gothic Bold.ttf",80);
	_goldLabel->setColor(Color3B(255,255,255));
    
    //_goldLabel->enableOutline(Color4B(0,0,0,255),5);
	_goldLabel->setPosition(bgPo.x,bgPo.y);
     _goldLabel->setAnchorPoint(cocos2d::Point(0,0));
	this->addChild(_goldLabel);
	this->updateGold(g_goodsData->gold);


	//±¶ Ø
	//Sprite *gemBg=Sprite::create("ui/coinShop/gemBg.png");
	//this->addChild(gemBg);
	//sz = gemBg->getContentSize();
	//gemBg->setPosition(center.x+sz.width/2,center.y+bgSz.height/2-93);

	bgPo = cocos2d::Point(center.x+200,center.y+bgSz.height/2-350);
	

	_gemLabel = Label::create("","Copperplate Gothic Bold.ttf",80);
	_gemLabel->setColor(Color3B(255,255,255));
    //_gemLabel->enableOutline(Color4B(0,0,0,255),5);
	_gemLabel->setPosition(bgPo.x,bgPo.y);
     _gemLabel->setAnchorPoint(cocos2d::Point(0,0));
	this->addChild(_gemLabel);
	this->updateGem(g_goodsData->gem);

	//back
	ui::Button *back = ui::Button::create();
	back->setTouchEnabled(true);
	back->setTag(BACKBTN);
	back->loadTextures("ui/store/BACK1.png","ui/store/BACK2.png","");
	this->addChild(back);
	back->setPosition(cocos2d::Point(center.x,center.y-bgSz.height/2+back->getContentSize().height));
	back->addTouchEventListener(this, toucheventselector(CoinShopUI::touchButtonEvent));


	//coinItem

	g_coinShopMgr=new CoinShopMgr;
	g_coinShopMgr->initCost();

	CoinShopView *coinShop = CoinShopView::create();
	this->addChild(coinShop);
	coinShop->setPosition(center.x-bgSz.width/2+64,center.y-bgSz.height/2+374);
	//for(int i=0;i<5;i++)
	//{
		//auto item = CoinShopItem::create();
		//this->addChild(item);
		//item->setCoin(i*10+10,i*10+1);
		//item->setPosition(itemPo);
		//cocos2d::Size itemSz = item->getContentSize();
		//itemPo.y-=itemSz.height;
	//}

	

	if(visibleSize.width<IPADWIDTH)
    {
        this->setScale( visibleSize.width / IPADWIDTH ,visibleSize.height / IPADHEIGHT);
        
    }
//	 if(visibleSize.width< UIWIDTH)
//    {
//        this->setScale( visibleSize.width / UIWIDTH ,visibleSize.height / UIHEIGHT);
//        
//    }
	return  true;
}
void CoinShopUI::touchButtonEvent(Sprite* pSender,TouchEventType type)
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
			case BACKBTN://∑µªÿ
				g_miscLayer->removeChild(this);
                if(g_miscLayer->getChildByTag(MOREGEM)!=NULL)
                {
                    g_miscLayer->removeChild(g_miscLayer->getChildByTag(MOREGEM));
                }
				break;
		}

		break;
	}

}
void CoinShopUI::updateGold(int gold)
{
	char si[32];
	sprintf(si, "%d", gold);
	_goldLabel->setString(si);

}
void CoinShopUI::updateGem(int gem)
{
	char s[64];
	sprintf(s,"%d",gem);
	_gemLabel->setString(s);
	int a=0;
}
void CoinShopUI::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CoinShopUI::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(CoinShopUI::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(CoinShopUI::onTouchEnded,this);
	listener->setSwallowTouches(true);//≤ªœÚœ¬¥´µ›¥•√˛
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}
void CoinShopUI::onExit()
{
	Layer::onExit();
	
}

 bool CoinShopUI::onTouchBegan (Touch *touch, Event *unused_event)
 {
	 return true;
 }
 void CoinShopUI::onTouchMoved (Touch *touch, Event *unused_event)
 {
 }
 void CoinShopUI::onTouchEnded (Touch *touch, Event *unused_event)
 {
 }
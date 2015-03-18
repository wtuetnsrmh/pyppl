#include "BottomMenuLayer.h"
#include "Global.h"
#include "GoodsShopLayer.h"
#include "GoodsShopItem.h"
#include "LoseUI.h"
#include "MaskLayer.h"
#include "MoreGemLayer.h"
#include <iostream>

GoodsShopLayer::GoodsShopLayer(void)
{

}
GoodsShopLayer::~GoodsShopLayer(void)
{


}
bool GoodsShopLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
	cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	MyRect gameFrame = g_gameLayer->getGameFrame();
	cocos2d::Point center = cocos2d::Point(gameFrame.x+gameFrame.width/2,gameFrame.y+gameFrame.height/2);
    lc = LayerColor::create(Color4B(0, 0, 0, 110));
    
	g_miscLayer->addChild(lc, 0);
    
	Sprite *gemBg = Sprite::create("goodsShop/bg.png");
	this->addChild(gemBg);
	gemBg->setPosition(center.x,center.y);
	Sprite *gem = Sprite::create("goodsShop/gem.png");
	this->addChild(gem);
	gem->setPosition(gemBg->getPosition().x+gemBg->getContentSize().width/2-gem->getContentSize().width*2,gemBg->getPosition().y+gemBg->getContentSize().height/2-gem->getContentSize().height);

	_gemLabel=Label::create("","Copperplate Gothic Bold.ttf",110);
	_gemLabel->setColor(Color3B(255,255,255));
	//_gemLabel->enableOutline(Color4B(0,0,0,255),2);
	//_gemLabel->setAlignment(TextHAlignment::RIGHT);
	this->addChild(_gemLabel);
	cocos2d::Point gpo=cocos2d::Point(center.x,gem->getPosition().y);
	_gemLabel->setPosition(gpo);
     _gemLabel->setString("0");
	this->updateGem(g_goodsData->gem);
   


	Button *back = Button::create();
	back->setTouchEnabled(true);
	back->loadTextures("goodsShop/cancel1.png","goodsShop/cancel2.png","");
	this->addChild(back);
	back->setPosition(cocos2d::Point( center.x,center.y-gemBg->getContentSize().height/2+back->getContentSize().height));
	back->addTouchEventListener(this, toucheventselector(GoodsShopLayer::touchEvent));
    back->setTag(BACK);
    

    if(visibleSize.width<IPADWIDTH)
    {
        this->setScale(visibleSize.width / IPADWIDTH , visibleSize.height / IPADHEIGHT);
    }
//    if(visibleSize.width< UIWIDTH)
//	{
//       this->setScale(visibleSize.width / UIWIDTH , visibleSize.height / UIHEIGHT);
//	}
	return true;
    
    
}
void GoodsShopLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
    int tag =pSender->getTag();
		switch(type){
		case TOUCH_EVENT_BEGAN:

			break;
		case TOUCH_EVENT_MOVED:

			break;
		case  TOUCH_EVENT_ENDED:
                g_miscLayer->playWav("btn.wav");
            switch (tag) {
                case BACK:
                    g_miscLayer->removeChild(this);
                    g_miscLayer->removeChild(lc);
                    if (g_miscLayer->getChildByTag(LOSEUI)==NULL&&g_gameLayer->getGameState()==G_OVER)
                    {
                        auto loseUI = LoseUI::create();
                        g_miscLayer->addChild(loseUI);
                        loseUI->setTag(LOSEUI);
                        g_gameLayer->gameRealOver();
                    }
                break;
                default:
                    break;
            }
			
			break;
		}

	
}
void GoodsShopLayer::updateGem(int gem)
{
        char s[64];
        sprintf(s,"%d",gem);
        _gemLabel->setString(s);	
}
void GoodsShopLayer::initItem(bool disabel)
{
	MyRect gameFrame = g_gameLayer->getGameFrame();
	cocos2d::Point center = cocos2d::Point(gameFrame.x+gameFrame.width/2,gameFrame.y+gameFrame.height/2);
	cocos2d::Point ipo=cocos2d::Point(center.x-673,center.y+90);
	int index=0;
	if(disabel==true)
	{
		for(int i=0;i<4;i++)
		{
			GoodsShopItem *item = GoodsShopItem::create();
			this->addChild(item);
			if(index==2)
			{
				ipo.y=ipo.y-410;
				ipo.x=center.x-673;
			}
			item->initWithType(i);
			item->setPosition(ipo);
			
			ipo.x+=item->getContentSize().width+30;
			index++;
			if(i<=2)
			{
				item->disabelItem(true);
			}else
			{
				item->disabelItem(false);
			}

		}
	}else
	{
		for(int i=0;i<4;i++)
		{
			GoodsShopItem *item = GoodsShopItem::create();
			this->addChild(item);
			if(index==2)
			{
				ipo.y=ipo.y-410;
				ipo.x=center.x-673;
			}
			item->initWithType(i);
			item->setPosition(ipo);
			ipo.x+=item->getContentSize().width+30;
			index++;
	}
	}
	
}
void GoodsShopLayer::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GoodsShopLayer::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(GoodsShopLayer::onTouchMoved,this);
	listener->onTouchEnded = CC_CALLBACK_2(GoodsShopLayer::onTouchEnded,this);
	listener->setSwallowTouches(true);//≤ªœÚœ¬¥´µ›¥•√˛
	dispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	
}

void GoodsShopLayer::onExit()
{
	Layer::onExit();
	
}

bool GoodsShopLayer::onTouchBegan (Touch *touch, Event *unused_event)
{
    return true;
}
void GoodsShopLayer::onTouchMoved (Touch *touch, Event *unused_event)
{
    
}
void GoodsShopLayer::onTouchEnded (Touch *touch, Event *unused_event)
{
    
}
void GoodsShopLayer::moreGem()
{
    more = MoreGemLayer::create();
    this->addChild(more);
}

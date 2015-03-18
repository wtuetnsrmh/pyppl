#include "BottomMenuLayer.h"
#include "Global.h"
#include "BuyGoodsLayer.h"
#include "GoodsShopItem.h"


BuyGoodsLayer::BuyGoodsLayer(void)
{

}
BuyGoodsLayer::~BuyGoodsLayer(void)
{


}
bool BuyGoodsLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
	cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	MyRect gameFrame = g_gameLayer->getGameFrame();
	cocos2d::Point center = cocos2d::Point(gameFrame.x+gameFrame.width/2,gameFrame.y+gameFrame.height/2);
   // LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));
    
	//this->addChild(lc, 0);
    
	Sprite *gemBg = Sprite::create("goodsShop/gemBg.png");
	this->addChild(gemBg);
	gemBg->setPosition(center.x,center.y+390);
	Sprite *gem = Sprite::create("goodsShop/gem.png");
	this->addChild(gem);
	gem->setPosition(gemBg->getPosition().x+gemBg->getContentSize().width/2-gem->getContentSize().width/2,gemBg->getPosition().y);

	_gemLabel=Label::create("","font/Marker Felt.ttf",30);
	_gemLabel->setColor(Color3B(255,255,255));
	_gemLabel->enableOutline(Color4B(0,0,0,255),2);
	_gemLabel->setAlignment(TextHAlignment::RIGHT);
	_gemLabel->setString("123");
	this->addChild(_gemLabel);
	cocos2d::Point gpo=cocos2d::Point(gemBg->getPosition().x,gemBg->getPosition().y);
	_gemLabel->setPosition(gpo);


	cocos2d::Point ipo=cocos2d::Point(center.x-370,center.y+100);
	int index=0;
	for(int i=0;i<4;i++)
	{
		GoodsShopItem *item = GoodsShopItem::create();
		this->addChild(item);
		if(index==2)
		{
			ipo.y=ipo.y-360;
			ipo.x=center.x-350;
		}
		item->initWithType(i);
		item->setPosition(ipo);
		ipo.x+=430;
		index++;
	}


	Button *back = Button::create();
	back->setTouchEnabled(true);
	back->loadTextures("goodsShop/cancel1.png","goodsShop/cancel2.png","");
	this->addChild(back);
	back->setPosition(cocos2d::Point( center.x,center.y-390));
	back->addTouchEventListener(this, toucheventselector(BuyGoodsLayer::touchEvent));


    if(visibleSize.width> UIWIDTH + 20)
    {
        this->setScale(visibleSize.width / UIWIDTH , visibleSize.height / UIHEIGHT);
    }
    if(visibleSize.width< UIWIDTH)
	{
       this->setScale(visibleSize.width / UIWIDTH , visibleSize.height / UIHEIGHT);
	}
	return true;
    
    
}
void BuyGoodsLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
		switch(type){
		case TOUCH_EVENT_BEGAN:

			break;
		case TOUCH_EVENT_MOVED:

			break;
		case  TOUCH_EVENT_ENDED:
			g_miscLayer->removeChild(this);
			break;
		}

	
}
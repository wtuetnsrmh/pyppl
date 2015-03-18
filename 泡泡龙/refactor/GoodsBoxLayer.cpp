#include "GoodsBoxLayer.h"
#include "GoodsSprite.h"
#include "Global.h"
#include "GameEnum.h"
#include "ShopLayer.h"
#include "GoodsShopLayer.h"

GoodsBoxLayer::GoodsBoxLayer(void)
{
}


GoodsBoxLayer::~GoodsBoxLayer(void)
{
}

bool GoodsBoxLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
    MyRect gf = g_gameLayer->getGameFrame();
    cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
    
    //this->setContentSize(cocos2d::Size(240,48));
    //this->setAnchorPoint(cocos2d::Point(0,0));
    //this->setPosition(gf.x+23, gf.y+25);

	//cocos2d::Point  startPt = cocos2d::Point(0,0);
    
	//GoodsSprite* goodsList[4];
//	for (int i=0;i<4;i++)
//	{
//		GoodsSprite *sp = GoodsSprite::create();
//		sp->initWithType(i);
//		sp->setPosition(startPt);
//		this->addChild(sp);
//		startPt.x += 48;
//
//		_goodsMap[i] = sp;
//    }
	//ui::Button *buyButton = ui::Button::create();
	//buyButton->loadTextures("goods/buy1.png","goods/buy2.png","");
	//this->addChild(buyButton);
	//buyButton->setTag(SHOPBTN);
	//buyButton->setPosition(startPt);
	//buyButton->addTouchEventListener(this, toucheventselector(GoodsBoxLayer::touchEvent));
//	cocos2d::Size  oldSz = this->getContentSize();
//	cocos2d::Point  oldPo = this->getPosition();
//    if(visibleSize.width> UIWIDTH + 20)
//    {
//        float radioX = visibleSize.width / UIWIDTH;
//        float radioY = visibleSize.height / UIHEIGHT ;
//        this->setScale(radioX , radioY);
//		cocos2d::Size  newSz = cocos2d::Size(oldSz.width*radioX, oldSz.height*radioY);
//		cocos2d::Point  diff = cocos2d::Point(gf.x+50, gf.y+newSz.height/2+10);
//        this->setPosition(diff);
//    }
    
    this->setPosition(cocos2d::Point(0,0));
    this->setAnchorPoint(cocos2d::Point(0,0));
    this->setContentSize(cocos2d::Size( 1536,262));
    cocos2d::Point po = this->getPosition();
    cocos2d::Size sz = this->getContentSize();
    
    
//    Sprite *bg=Sprite::create("ipad/bottomBg.png");
//    bg->setPosition(sz.width/2,sz.height/2);
//    this->addChild(bg);
    
    Button *btn = Button::create();
    btn->loadTextures("ui/bottom/goodsBtn1.png","ui/bottom/goodsBtn2.png", "");
    btn->setTouchEnabled(true);
    this->addChild(btn);
    cocos2d::Point btnPO = cocos2d::Point(sz.width/2-btn->getContentSize().width/2+20,btn->getContentSize().height/2+20);
    btn->setPosition(btnPO);
    btn->addTouchEventListener(this, toucheventselector(GoodsBoxLayer::touchEvent));
    
    if(visibleSize.width<IPADWIDTH)
    {
        float radioX = visibleSize.width / IPADWIDTH;
        float radioY = visibleSize.height / IPADHEIGHT ;
        this->setScale(radioX , radioY);
        //cocos2d::Size  newSz = cocos2d::Size(sz.width*radioX, sz.height*radioY);
//        cocos2d::Point  diff = cocos2d::Point(visibleSize.width/2,visibleSize.height/2);
//        this->setPosition(diff);
    }
	return true;
}

void GoodsBoxLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
    case  TOUCH_EVENT_ENDED:
        g_miscLayer->playWav("btn.wav" );
        if(g_miscLayer->getChildByTag(GOODSHOP)==NULL)
        {
            g_goodsShopLayer = GoodsShopLayer::create();
            g_miscLayer->addChild(g_goodsShopLayer);
            g_goodsShopLayer->setTag(GOODSHOP);
            g_goodsShopLayer->initItem(false);
        }
			//case SHOPBTN:
			//if (g_miscLayer->getChildByTag(SHOPUI)==NULL)
			//{
			//	auto ShopUI = ShopLayer::create();
			//	g_miscLayer->addChild(ShopUI);
			//	ShopUI->setTag(SHOPUI);
			//}
            
		
		break;
	}

}
/*void GoodsBoxLayer::updateGoods(int type, int changeCount)
{
	int curCount = _goodsMap[type]->getGoodsCount();
	_goodsMap[type]->setGoodsCount( curCount + changeCount );
	_goodsMap[type]->updateCountShow(curCount + changeCount);
}*/
/*
void GoodsBoxLayer::onEnter()
{
	Layer::onEnter();
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(false);

	listener1->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<GoodsSprite*>(event->getCurrentTarget());
		target->setScale(1.1f);
		return true;
	};

	listener1->onTouchMoved = [](Touch* touch, Event* event){
		
	};

	listener1->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<GoodsSprite*>(event->getCurrentTarget());
		target->setScale(1.0f);
		switch (target->getGoodsType() )
		{
		case GT_AssistLine:
			g_shotLayer->setLienShow(true);
			break;
		case GT_Earthquake:
			g_gameLayer->earthquake();
			break;
		case GT_ResetHeart:
			g_shotLayer->resetHeart();
			break;
		case GT_ChangeShottingBubbleType:
			g_shotLayer->changeShottingBubbleByRandom();
			break;
		default:
			break;
		}
		
	
	};

	std::vector<GoodsSprite*>::iterator iter;
	for (iter = _goodsSpriteList.begin(); iter!=_goodsSpriteList.end(); ++iter)
	{
		this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1->clone(), *iter);
	}
	

}*/
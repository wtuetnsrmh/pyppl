#include "GoodsShopItem.h"
#include "GameEnum.h"
#include "Global.h"
#include "MaskLayer.h"
GoodsShopItem::GoodsShopItem(void)
{
}
GoodsShopItem::~GoodsShopItem(void)
{
}
bool GoodsShopItem::init()
{
	if(!Layer::init())
	{
		return false;
	}
	
	return true;
}
void GoodsShopItem::initWithType(int type)
{
	this->setGoodsType(type);

	btn=Button::create();
	btn->setTouchEnabled(true);
	btn->loadTextures("goodsShop/bigBtn1.png","goodsShop/bigBtn2.png","");
	this->addChild(btn);
	cocos2d::Point centerPo=cocos2d::Point(btn->getContentSize().width/2,btn->getContentSize().height/2);
	this->setContentSize(btn->getContentSize());
	btn->setPosition(centerPo);
	btn->addTouchEventListener(this, toucheventselector(GoodsShopItem::touchEvent));

	char s[64];
	sprintf(s,"goodsShop/%d-01.png",type+1);
	_goodsName=Sprite::create(s);
	this->addChild(_goodsName);
	_goodsName->setPosition(centerPo.x,centerPo.y+_goodsName->getContentSize().height/2);

	Sprite *gem = Sprite::create("goodsShop/gem.png");
	this->addChild(gem);
	gem->setPosition(btn->getContentSize().width-gem->getContentSize().width/2-10,centerPo.y-gem->getContentSize().height/2-15);

	this->initGem(type);

}
void GoodsShopItem::initGem(int type)
{
	switch (type)
			{
			case GT_Earthquake:
				this->changeGemSp(g_gameLayer->_EarthquakeCount,false);
				break;
			case GT_EnergyInjection: //能量翻倍
				this->changeGemSp(g_gameLayer->_EnergyInjectionCount,false);
				break;
			case GT_Laser: //激光
				this->changeGemSp(g_gameLayer->_LaserCount,false);
				break;
			case GT_Timelaspe: //时光倒流
				this->changeGemSp(g_gameLayer->_TimelaspeCount,false);
				break;
			default:
				break;
			}
}
void GoodsShopItem::changeGemSp(int count,bool isChange)
{
	cocos2d::Point centerPo=cocos2d::Point(this->getContentSize().width/2,this->getContentSize().height/2-74);
	char s[64];
	int posx = centerPo.x;
	int checknum = count;


	if(isChange==true)
	{
		if(_gemSp1!=NULL)
		{
			this->removeChild(_gemSp1);
		}
		while ((checknum=checknum / 10) > 0)
		{
			sprintf(s,"goodsShop/%d.png",checknum);
			_gemSp1=Sprite::create(s);
			this->addChild(_gemSp1);
			_gemSp1->setPosition(posx-_gemSp1->getContentSize().width,centerPo.y);
		}
		sprintf(s,"goodsShop/%d.png",count%10);
		_gemSp2->setTexture(s);

	}else
	{
		while ((checknum=checknum / 10) > 0)
		{
			sprintf(s,"goodsShop/%d.png",checknum);
			_gemSp1=Sprite::create(s);
			this->addChild(_gemSp1);
			_gemSp1->setPosition(posx-_gemSp1->getContentSize().width,centerPo.y);
		
		}
		sprintf(s,"goodsShop/%d.png",count%10);
		_gemSp2=Sprite::create(s);
		this->addChild(_gemSp2);
		_gemSp2->setPosition(posx,centerPo.y);
	}

	
}
void GoodsShopItem::touchEvent(Sprite* pSender,TouchEventType type)
{
	GoodsShopItem * gs = dynamic_cast<GoodsShopItem*>(pSender->getParent());
		switch(type){
		case TOUCH_EVENT_BEGAN:

			break;
		case TOUCH_EVENT_MOVED:

			break;
		case  TOUCH_EVENT_ENDED:
			switch (gs->getGoodsType())
			{
			case GT_Earthquake:
				_currentType=GT_Earthquake;
                if(g_goodsData->gem>= g_gameLayer->_EarthquakeCount)
                {
                    this->runAction(Sequence::create(
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::doAction)),
                                    DelayTime::create(0.02f),
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::removeThis)),
                                    NULL));
                }else
                {
                    g_shopMgr->buyGoods(g_gameLayer->_EarthquakeCount);
                    g_goodsShopLayer->moreGem();
                }
				break;
			case GT_EnergyInjection: //能量翻倍
				_currentType=GT_EnergyInjection;
                if(g_goodsData->gem>=g_gameLayer->_EnergyInjectionCount)
                {
                    this->runAction(Sequence::create(
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::doAction)),
                                    DelayTime::create(0.02f),
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::removeThis)),
                                   
                                   
                                    NULL));
                }else{
                    g_shopMgr->buyGoods(g_gameLayer->_EnergyInjectionCount);
                    g_goodsShopLayer->moreGem();
                }
				break;
			case GT_Laser: //激光
				_currentType=GT_Laser;
                if(g_goodsData->gem>=g_gameLayer->_LaserCount)
                {
                    this->runAction(Sequence::create(
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::doAction)),
                                    DelayTime::create(0.02f),
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::removeThis)),
                                   
                                    NULL));
                }else
                {
                    g_shopMgr->buyGoods(g_gameLayer->_LaserCount);
                    g_goodsShopLayer->moreGem();
                }
				break;
			case GT_Timelaspe: //时光倒流
				_currentType=GT_Timelaspe;
                if(g_goodsData->gem>=g_gameLayer->_TimelaspeCount)
                {
                    this->runAction(Sequence::create(
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::addShizhong)),
                                    DelayTime::create(1.6f),
                                    CallFuncN::create(this,callfuncN_selector(GoodsShopItem::restoreBarrier)),
                                    NULL));
                    g_shopMgr->buyGoods(g_gameLayer->_TimelaspeCount);
                    g_gameLayer->_TimelaspeCount++;
                    this->changeGemSp(g_gameLayer->_TimelaspeCount,true);
                }else
                {
                    g_shopMgr->buyGoods(g_gameLayer->_TimelaspeCount);
                    g_goodsShopLayer->moreGem();
                    
                }
				break;
			default:
				break;
			}

			break;
		}
	
}
void GoodsShopItem::removeThis(Node*node)
{
	g_miscLayer->removeChild(g_goodsShopLayer);
    g_miscLayer->removeChild(g_goodsShopLayer->lc);
}
void GoodsShopItem::doAction(Node*node)
{
	switch (_currentType)
			{
			case GT_Earthquake:
                    g_shopMgr->buyGoods(g_gameLayer->_EarthquakeCount);
                    g_gameLayer->_EarthquakeCount++;
                    this->changeGemSp(g_gameLayer->_EarthquakeCount,true);
                    g_gameLayer->earthquake();
				break;
			case GT_EnergyInjection: //能量翻倍
                    g_shopMgr->buyGoods(g_gameLayer->_EnergyInjectionCount);
                    g_gameLayer->_EnergyInjectionCount++;
                    g_gameLayer->doublePower();
                    this->changeGemSp(g_gameLayer->_EnergyInjectionCount,true);
				break;
			case GT_Laser: //激光
                    g_shopMgr->buyGoods(g_gameLayer->_LaserCount);
                    g_gameLayer->useLaser();
                    g_gameLayer->_LaserCount++;
                    this->changeGemSp(g_gameLayer->_LaserCount,true);
				break;
			default:
				break;
			}
}
void GoodsShopItem::addShizhong(Node *node)
{
    if(g_miscLayer->getChildByTag(MASKLAYER)==NULL)
    {
        auto mask = MaskLayer::create();
        g_miscLayer->addChild(mask);
        mask->setTag(MASKLAYER);
    }
	g_miscLayer->addShiZhong(true);
}
void GoodsShopItem::restoreBarrier(Node *node)
{
	g_miscLayer->addShiZhong(false);
	g_gameLayer->restoreCurrentBarrier();
	g_miscLayer->removeChild(g_goodsShopLayer);
    g_miscLayer->removeChild(g_goodsShopLayer->lc);
    if(g_miscLayer->getChildByTag(MASKLAYER)!=NULL)
    {
        g_miscLayer->removeChild(g_miscLayer->getChildByTag(MASKLAYER));
    }
}
void GoodsShopItem::disabelItem(bool disabel)
{
	if(disabel==true)
	{
		gray = Sprite::create("goodsShop/gray.png");
		gray->setAnchorPoint(cocos2d::Point(0,0));
		this->addChild(gray,this->getChildrenCount()-1);
		gray->setContentSize(cocos2d::Size(291,144));
		btn->setTouchEnabled(false);
	}else
	{
		if(gray)
		{
			this->removeChild(gray);
			btn->setTouchEnabled(true);
		}
	}
}
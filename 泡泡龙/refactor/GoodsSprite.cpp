#include "GoodsSprite.h"
#include "GameEnum.h"
#include "Global.h"
#include "GoodsShopLayer.h"
GoodsSprite::GoodsSprite()
{

}

GoodsSprite::~GoodsSprite()
{

}

bool GoodsSprite::init()  
{  
	//////////////////////////////  
	// 1. super init first  
	if ( !Layer::init() )  
	{  
		return false;  
	}  
	return true;
}

void GoodsSprite::initWithType(int type)
{
	
	ui::Button *button= ui::Button::create();  
	button->setTouchEnabled(true);
	char s1[64];
	sprintf(s1, "goods/%d-01.png", type+1);
	char s2[64];
	sprintf(s2, "goods/%d-02.png", type+1);

	//char s3[64];
	//sprintf(s3, "goods/%d-03.png", type+1);

	button->loadTextures(s1, s2, ""); //Button的bug， disable状态没法显示

	button->addTouchEventListener(this, toucheventselector(GoodsSprite::touchEvent));
	this->addChild(button);

	_enableButton = button;
	
	this->setGoodsType(type);

	/*switch (type)
	{
	
	case GT_EnergyInjection:
		this->setGoodsCount(g_goodsData->gEnergy);
		break;
	
	case GT_Laser:
		this->setGoodsCount(g_goodsData->gLaser);
		break;
	case GT_Timelaspe:
		this->setGoodsCount(g_goodsData->gTimelaspe);
		break;
		
	case GT_Earthquake:
		this->setGoodsCount(g_goodsData->gEarthQuake);
		break;
	
	default:
		break;
	}*/

	//_disableButton = Sprite::create(s3);

	//this->addChild(_disableButton);

	/*if(this->getGoodsCount() == 0){
		//button->setEnabled(false);
		//_disableButton->setVisible(true);
		_enableButton->setVisible(false);
	}
	else{
		//_disableButton->setVisible(false);
		_enableButton->setVisible(true);
	}

    _countLabel = Label::create("","font/Marker Felt.ttf",16);
	this->addChild(_countLabel);
    _countLabel->enableOutline(Color4B(0,0,0,255),3);

	char sCount[32];
	_countLabel->setAlignment(TextHAlignment::RIGHT);
	_countLabel->setPosition(cocos2d::Point(0, -20));
	_countLabel->setColor(Color3B(255, 255, 255));
	sprintf(sCount, "%d", this->getGoodsCount());
	_countLabel->setString(sCount);*/

}

/*void GoodsSprite::updateCountShow(int count)
{
	char sCount[32];
	sprintf(sCount, "%d", count);
	_countLabel->setString(sCount);

	if(this->getGoodsCount() == 0){
		disableButton();
	}
	else{
		enableButton();
	}
}*/

/*void GoodsSprite::enableButton()
{
	//_disableButton->setVisible(false);
	_enableButton->setVisible(true);
}

void GoodsSprite::disableButton()
{
	//_disableButton->setVisible(true);
	_enableButton->setVisible(false);
}*/

void GoodsSprite::touchEvent(Sprite* pSender,TouchEventType type)
{
	GoodsSprite * gs = dynamic_cast<GoodsSprite*>(pSender->getParent());
		switch(type){
		case TOUCH_EVENT_BEGAN:

			break;
		case TOUCH_EVENT_MOVED:

			break;
		case  TOUCH_EVENT_ENDED:
                if(g_miscLayer->getChildByTag(GOODSHOP)==NULL)
                {
                    g_goodsShopLayer = GoodsShopLayer::create();
                    g_miscLayer->addChild(g_goodsShopLayer);
                    g_goodsShopLayer->setTag(GOODSHOP);
					g_goodsShopLayer->initItem(false);
                }
			
			break;
		}
	
}
/*int GoodsSprite::getGoodsCount()
{
	return this->_goodsCount;
}
void GoodsSprite::setGoodsCount(int count)
{
	this->_goodsCount = count;
	//this->upateCountShow(count);
}*/



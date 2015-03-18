#include "CoinShopItem.h"
#include "Global.h"
#include "Purchase.h"

CoinShopItem::CoinShopItem(void)
{
	
}
CoinShopItem::~CoinShopItem(void)
{
}
bool CoinShopItem::init()
{
	if ( !Layer::init() )  
	{  
		return false;  
	} 
	this->setContentSize(cocos2d::Size(1398,300));
	this->setPosition(cocos2d::Point(0,0));
	
	return  true ;
}
void CoinShopItem::initWithType(int type)
{
	this->setCoinType(type);
	cocos2d::Size  sz = this->getContentSize();
	cocos2d::Point  po = this->getPosition();
	cocos2d::Point  centerPo = cocos2d::Point(po.x+sz.width/2,po.y+sz.height/2);

	char s1[64];
	sprintf(s1, "ui/coinShop/%d-01.png", type+1);
	_iconImg = Sprite::create(s1);
	this->addChild(_iconImg);
	_iconImg->setPosition(po.x+_iconImg->getContentSize().width/2,centerPo.y);
	cocos2d::Point basePt  = cocos2d::Point(po.x+_iconImg->getContentSize().width,centerPo.y+40);
	
    _coinsLabel = Label::create("","Copperplate Gothic Bold.ttf",70);
    this->addChild(_coinsLabel);
    _coinsLabel->setAnchorPoint(cocos2d::Point(0,0));
    _coinsLabel->setPosition(basePt);
    _coinsLabel->setString("1");
    
    
	//_coinPo = cocos2d::Point(basePt.x+200,basePt.y);
//	if(type==0)
//	{
//		_coinImg=Sprite::create("ui/coinShop/double.png");
//	}else if(type>0&&type<5)
//	{
//		_coinImg=Sprite::create("ui/coinShop/COINS.png");
//	}else
//	{
//		_coinImg=Sprite::create("ui/coinShop/gem.png");
//	}
	
	//_coinImg->setPosition(_coinPo);
	//this->addChild(_coinImg);

//	for(int i=0; i<COIN_NUMBER; i++){
//		
//		_coinNumbers[i] = Sprite::create("ui/coinShop/w.png");
//		Sprite *sp = _coinNumbers[i];
//		this->addChild(sp);
//		sp->setPosition(basePt.x+90,basePt.y);
//		basePt.x -= 34;
//	}

	Sprite *tiao = Sprite::create("ui/store/tiao.png");
	this->addChild(tiao);
	sz = tiao->getContentSize();
	tiao->setPosition(po.x+sz.width/2,po.y);

	buyBtn =Button::create();
	buyBtn->loadTextures("ui/store/buy1.png","ui/store/buy2.png","");
	buyBtn->setTouchEnabled(true);
	this->addChild(buyBtn);
	buyBtn->setTouchEnabled(true);
	cocos2d::Size btnSz = buyBtn->getContentSize();
	buyBtn->setPosition(cocos2d::Point(sz.width-btnSz.width/2,po.y+btnSz.height/2+5));
	buyBtn->addTouchEventListener(this,toucheventselector(CoinShopItem::touchButtonEvent));

    
    _moneyLabel = Label::create("","Copperplate Gothic Bold.ttf",70);
    this->addChild(_moneyLabel);
    _moneyLabel->setColor(Color3B(244,236,32));
	_moneyLabel->setPosition(sz.width/2,_moneyLabel->getContentSize().height);
    _moneyLabel->setAnchorPoint(cocos2d::Point(0,0));
    
    if(type==0)
    {
        _coinsLabel->setString("COINS DOUBLER");
    	
    }
    
	this->initCost(type);
	

}
void  CoinShopItem::updateItem(int type)
{
	this->setCoinType(type);

	char s1[64];
	sprintf(s1, "ui/coinShop/%d-01.png", type+1);
	Texture2D *textture;  
	Sprite * temp_obj = Sprite::create(s1);  
	textture = temp_obj->getTexture();   
	_iconImg->setTexture(textture);

    if(type==0)
    {
        _coinsLabel->setString("COINS DOUBLER");
    	
    }
    
	this->initCost(type);
}
void  CoinShopItem::touchButtonEvent(Sprite* pSender,TouchEventType type)
{
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
    case  TOUCH_EVENT_ENDED:
        g_miscLayer->playWav("btn.wav" );
        Purchase * purchase = [[Purchase alloc]init];
        [purchase startPurchase:(this->getCoinType())];
		break;
	}
}
void CoinShopItem::initCost(int type)
{
	int coin=g_coinShopMgr->getCoin(type);
	float cost=g_coinShopMgr->getCost(type);
	this->setCoin(coin,cost);
}
void CoinShopItem::setCoin(int coin,float money)
{
	char s[64];
	sprintf(s,"$%.2f",money);

	_moneyLabel->setString(s);

	this->addCoin(coin);

}
//void CoinShopItem::changeNumber(Sprite *sp, int num)
//{
//	
//	char sn[32];
//	sprintf(sn, "ui/coinShop/%d.png", num);
//
//	Texture2D *textture;  
//
//	Sprite * temp_obj = Sprite::create(sn);  
//
//	textture = temp_obj->getTexture();   
//
//	sp->setTexture(textture);  
//}

void CoinShopItem::addCoin(int coin)
{
	if(coin==0)return;
    int type =  this->getCoinType();
    char s[64];
	if(type>=1&&type<5)
    {
       
        sprintf(s,"%d  COINS",coin);
        _coinsLabel->setString(s);
    		
    }else
    {
        sprintf(s,"%d  GEM PACK",coin);
    	_coinsLabel->setString(s);
    }
}

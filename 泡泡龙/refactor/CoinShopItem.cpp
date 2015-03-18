#include "CoinShopItem.h"
#include "Global.h"


CoinShopItem::CoinShopItem(void)
{
	_coinNumbers = new Sprite*[COIN_NUMBER];
	for(int i=0; i<COIN_NUMBER; i++){
		_coinNumbers[i] = NULL;
	}
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
	this->setContentSize(cocos2d::Size(681,186));
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
	cocos2d::Point basePt  = cocos2d::Point(po.x+COIN_NUMBER*34,centerPo.y+40);
	
	_coinPo = cocos2d::Point(basePt.x+200,basePt.y);
	if(type==0)
	{
		_coinImg=Sprite::create("ui/coinShop/double.png");
	}else if(type>0&&type<5)
	{
		_coinImg=Sprite::create("ui/coinShop/COINS.png");
	}else
	{
		_coinImg=Sprite::create("ui/coinShop/gem.png");
	}
	
	_coinImg->setPosition(_coinPo);
	this->addChild(_coinImg);

	for(int i=0; i<COIN_NUMBER; i++){
		
		_coinNumbers[i] = Sprite::create("ui/coinShop/w.png");
		Sprite *sp = _coinNumbers[i];
		this->addChild(sp);
		sp->setPosition(basePt.x+90,basePt.y);
		basePt.x -= 34;
	}

	Sprite *tiao = Sprite::create("ui/coinShop/tiao.png");
	this->addChild(tiao);
	sz = tiao->getContentSize();
	tiao->setPosition(po.x+sz.width/2+13,po.y);

	buyBtn =Button::create();
	buyBtn->loadTextures("ui/coinShop/money/btn1.png","ui/coinShop/money/btn2.png","");
	buyBtn->setTouchEnabled(true);
	this->addChild(buyBtn);
	buyBtn->setTouchEnabled(true);
	buyBtn->setTitleText("$9.9");
	buyBtn->setTitleColor(Color3B(255,255,255));
	buyBtn->setTitleFontSize(40);
	cocos2d::Size btnSz = buyBtn->getContentSize();
	buyBtn->setPosition(cocos2d::Point(sz.width-btnSz.width/2,po.y+btnSz.height/2+5));
	buyBtn->addTouchEventListener(this,toucheventselector(CoinShopItem::touchButtonEvent));

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

	cocos2d::Size  sz = this->getContentSize();
	cocos2d::Point  po = this->getPosition();
	cocos2d::Point  centerPo = cocos2d::Point(po.x+sz.width/2,po.y+sz.height/2);
	cocos2d::Point basePt  = cocos2d::Point(po.x+COIN_NUMBER*34,centerPo.y+40);
	for(int i=0; i<COIN_NUMBER; i++){

		Sprite *sp = _coinNumbers[i];
		Texture2D *textture1;  
		Sprite * temp_obj1 = Sprite::create("ui/coinShop/w.png");  
		textture1 = temp_obj1->getTexture();   
		sp->setTexture(textture1); 
		//this->addChild(sp);
		sp->setPosition(basePt.x+90,basePt.y);
		basePt.x -= 34;
		_coinNumbers[i] = sp;
	}

	if(type==0)
	{
		Texture2D *textture1;  
		Sprite * temp_obj1 = Sprite::create("ui/coinShop/double.png");  
		textture1 = temp_obj1->getTexture();   
		_coinImg->setTexture(textture1); 
	}else if(type>0&&type<5)
	{
		Texture2D *textture2;  
		Sprite * temp_obj2 = Sprite::create("ui/coinShop/COINS.png");  
		textture2 = temp_obj2->getTexture();   
		_coinImg->setTexture(textture2); 
	}else
	{
		Texture2D *textture3;  
		Sprite * temp_obj3 = Sprite::create("ui/coinShop/gem.png");  
		textture3 = temp_obj3->getTexture();   
		_coinImg->setTexture(textture3); 
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
		if(this->getCoinType()>=0&&this->getCoinType()<5)
		{
			addGold();
		}else
		{
			addGem();
		}

		break;
	}
}
void CoinShopItem::addGold()
{
	int gold=g_coinShopMgr->getCoin(this->getCoinType());
	g_goodsData->gold+=gold;
    g_gameLayer->saveGold2File(g_goodsData->gold);
	g_coinShopUI->updateGold(g_goodsData->gold);
    
    
    if(g_miscLayer->getChildByTag(UPGRADE))
    g_upGradeLayer->updateGold(g_goodsData->gold);

}
void CoinShopItem::addGem()
{
	int gem=g_coinShopMgr->getCoin(this->getCoinType());
	g_goodsData->gem+=gem;
    g_gameLayer->saveGold2File(g_goodsData->gem);
	g_coinShopUI->updateGem(g_goodsData->gem);
    if (g_miscLayer->getChildByTag(GOODSHOP)) {
        g_goodsShopLayer->updateGem(g_goodsData->gem);
    }
    
    if(g_miscLayer->getChildByTag(UPGRADE))
    g_upGradeLayer->updateGem(g_goodsData->gem);
}
void CoinShopItem::initCost(int type)
{
	int coin=g_coinShopMgr->getCoin(type);
	int cost=g_coinShopMgr->getCost(type);
	this->setCoin(coin,cost);
}
void CoinShopItem::setCoin(int coin,int money)
{
	char s[64];
	sprintf(s,"$%d",money);

	buyBtn->setTitleText(s);

	this->addCoin(coin);

}
void CoinShopItem::changeNumber(Sprite *sp, int num)
{
	
	char sn[32];
	sprintf(sn, "ui/coinShop/%d.png", num);

	Texture2D *textture;  

	Sprite * temp_obj = Sprite::create(sn);  

	textture = temp_obj->getTexture();   

	sp->setTexture(textture);  
}

void CoinShopItem::addCoin(int coin)
{
	if(coin==0)return;
	int index = 0;
	while(coin>=10){
		if (index>=COIN_NUMBER)
		{
			break;
		}
		this->changeNumber(_coinNumbers[index], coin  % 10);
		coin /= 10;
		index++;
	}
	if(coin != 0 && index < COIN_NUMBER){
		
		this->changeNumber(_coinNumbers[index], coin);
	}
}

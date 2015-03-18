#include "GrassSprite.h"
#include "GrassBubble.h"
#include "Global.h"
GrassSprite::GrassSprite(void)
{
	_bubble = NULL;
}

GrassSprite::~GrassSprite(void)
{
}


GrassSprite * GrassSprite::initWithType(int type) //∏˘æ›¥´π˝¿¥µƒ¿‡–Õ¿¥…˙≥…≈›≈�? 
{  
	GrassSprite *pRet = GrassSprite::create();  
	if (pRet && pRet->initWithFile(getStringByType(type))) //≥ı ºªØ≈–∂œ  
	{  
		pRet->setGrassType(type);
		pRet->addABubble();

	}  
	else  
	{  
		CC_SAFE_RELEASE(pRet);  
		pRet = NULL;  
	}  
	return pRet;  
}
std::string GrassSprite::getStringByType(int type) //≤ªÕ¨¿‡–ÕªÒ»°≤ªÕ¨µƒÕº∆¨◊ ‘�? 
{  
    std::string pRet = "";  
	char s[32];
	
	sprintf(s, "grass/g_0%d.png", type);
	pRet = s;

    return pRet;  
}
void GrassSprite::addABubble()
{
	cocos2d::Point  ptList[5] = 
	{
		cocos2d::Point(163, 77), cocos2d::Point(137,119), cocos2d::Point(51,118), 
		cocos2d::Point(48,101),
		cocos2d::Point(90,79)
	};

	GrassBubble_Type type =static_cast<GrassBubble_Type>(rand()%17+1);
	_bubble = GrassBubble::initWithType(type);
    float s =_bubble->getScale();
    _bubble->setScale(0.1);
    _bubble->runAction(ScaleTo::create(1.5f,s));
	_bubble->setPosition(ptList[this->getGrassType()-1]);
    this->addChild(_bubble);
	_bubble->playEffect(300);
    
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    if(visibleSize.width> UIWIDTH)
    {
        cocos2d::Size  newSz =_bubble->getContentSize();
        cocos2d::Point oldPo = _bubble->getPosition();
        
        cocos2d::Point  diff = cocos2d::Point(oldPo.x,oldPo.y+(newSz.height-11)/2-21);
        _bubble->setPosition(diff);
    }
	
	
}
void GrassSprite::removeABubble()
{
    if(_bubble==NULL)return;
	_bubble->runAction(Sequence::create(
        DelayTime::create(0.1f),
		CallFuncN::create(this,callfuncN_selector(GrassSprite::removeBubbleAction)),
		DelayTime::create(0.7f),
		CallFuncN::create(this,callfuncN_selector(GrassSprite::removeBubble)),
		NULL));
}
void GrassSprite::removeBubble(Node*node)
{
	this->removeChild(_bubble,true);
	_bubble->stopAllActions();
	_bubble = NULL;
}
void GrassSprite::removeBubbleAction(Node*node)
{
	_bubble->playEffectOnce(301,0.07f);
}
GrassBubble* GrassSprite::getBubble()
{
	return _bubble;
}
void GrassSprite::setGrassType(int type)
{
	_grassType = type;
}
int GrassSprite::getGrassType()
{
	return _grassType;
}
void GrassSprite::showEffect(GrassBubble_Type type)
{
	int count = 0;
	switch (type)
	{
	case GBT_AssistLine:
		g_shotLayer->setLienShow(true);
		break;
	case GBT_HeartFreeze:
		g_scoreLayer->iceSprite->setVisible(true);
		count = g_talentMgr->getCurrentEffectValue(TT_HeartFreeze);
		g_shotLayer->setHeartFreeze(count);
		break;
	case GBT_Electric:
		count = g_talentMgr->getCurrentEffectValue(TT_Electric);
		g_shotLayer->setElectricBubble(count);
		break;
	case GBT_FireShot:
		g_miscLayer->addFireShot(true);
		g_shotLayer->setIsShotting(true);
		count = g_talentMgr->getCurrentEffectValue(TT_FireShot);
		_hpCount=count;
		this->runAction(Sequence::create(
			DelayTime::create(4.0f),
			CallFuncN::create(this,callfuncN_selector(GrassSprite::changeHp)),
			NULL));
		break;
	case GBT_Earthquake:
		g_gameLayer->earthquake();
		break;
	case GBT_EnergyInjection:
		g_gameLayer->doublePower();
		break;
	case GBT_Laser:
		g_gameLayer->useLaser();
		break;
	case GBT_Timelaspe:
		this->runAction(Sequence::create(
					CallFuncN::create(this,callfuncN_selector(GrassSprite::addShizhong)),
					DelayTime::create(1.6f),
					CallFuncN::create(this,callfuncN_selector(GrassSprite::restoreBarrier)),
					NULL));
		break;
	case GBT_OneGem:
		this->addGem(1);
		break;
	case GBT_TwoGem:
		this->addGem(2);
		break;
	case GBT_OneHundredGold:
		this->addGold(100);
		break;
	case GBT_TwoHundredGold:
		this->addGold(200);
		break;
	case GBT_Pass_Type1:
		g_shotLayer->changeShottingBubble(PAOPAO_TYPE1);
		g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		break;
	case GBT_Pass_Type2:
		g_shotLayer->changeShottingBubble(PAOPAO_TYPE2);
		g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		break;
	case GBT_Pass_Type3:
		g_shotLayer->changeShottingBubble(PAOPAO_TYPE3);
		g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		break;
	case GBT_Pass_Type4:
		g_shotLayer->changeShottingBubble(PAOPAO_TYPE4);
		g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		break;
	case GBT_Pass_Type5:
		g_shotLayer->changeShottingBubble(PAOPAO_TYPE5);
		g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		break;
	case GBT_Pass_Type6:
		g_shotLayer->changeShottingBubble(PAOPAO_TYPE6);
		g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		break;
	default:
		break;
	}
}
void GrassSprite::changeHp(Node* node)
{
	g_miscLayer->playWav("fire.wav");
	g_gameLayer->changeEnemyBossHp(_hpCount);
	g_miscLayer->addFireShot(false);
	g_shotLayer->setIsShotting(false);
}
void GrassSprite::addShizhong(Node *node)
{
	g_miscLayer->addShiZhong(true);
}
void GrassSprite::restoreBarrier(Node *node)
{
	g_miscLayer->addShiZhong(false);
	g_gameLayer->restoreCurrentBarrier();
	g_miscLayer->removeChild(g_goodsShopLayer);
}
void GrassSprite::addGem(int count)
{
	g_goodsData->gem+=count;
}
void GrassSprite::addGold(int count)
{
	g_goodsData->gold+=count;
    g_gameLayer->addtotalCoin(count);
}

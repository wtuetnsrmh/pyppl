#include "ScoreLayer.h"
#include "GameConst.h"
#include "Bubble.h"
#include "Global.h"
#include "BarrierData.h"
#include "CoinShopUI.h"
#include <iostream>
ScoreLayer::ScoreLayer(void)
{
	_score = 0;
    _runGold=0;
	_scoreNumbers = new Sprite*[SCORE_NUMBER];
	for(int i=0; i<SCORE_NUMBER; i++){
		_scoreNumbers[i] = NULL;
	}
	

	_progressBubble = new Bubble*[PROGRESS_PAOPAO_NUMBER];

	for(int i=0; i<PROGRESS_PAOPAO_NUMBER; i++){
		_progressBubble[i] = NULL;
	}
	_resetHeartRound = 0;
}

ScoreLayer::~ScoreLayer(void)
{
	if(_scoreNumbers != NULL)
		delete[] _scoreNumbers;
}


bool ScoreLayer::init()  
{  
    //////////////////////////////  
    // 1. super init first  
    if ( !Layer::init() )  
    {  
        return false;  
    }  

    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	MyRect gameFrame = g_gameLayer->getGameFrame();

    cocos2d::Point  gunPo =g_shotLayer->getGunPosition();
	cocos2d::Point po =cocos2d::Point(gameFrame.x,gunPo.y);
    
    Sprite *stageBg = Sprite::create("ui/top/stage.png");
    this->addChild(stageBg);
    cocos2d::Point  stagePo = cocos2d::Point(gameFrame.x + stageBg->getContentSize().width/2 ,visibleSize.height-stageBg->getContentSize().height/2);
    stageBg->setPosition(stagePo);
    
    _stageLabel = Label::create("","Copperplate Gothic Bold.ttf",80);
    _stageLabel->setColor(Color3B(255,255,255));
	_stageLabel->setString("Stage 1");
	this->addChild(_stageLabel);
	_stageLabel->setPosition(stagePo.x,stagePo.y);
    this->setStage(g_gameLayer->getCurrentBarrierData()->id);
    
    

    Sprite *score = Sprite::create("ui/top/scoreBg.png");
    this->addChild(score);
    score->setPosition(gameFrame.x+gameFrame.width/2-70,visibleSize.height-score->getContentSize().height/2);
    
    _scoreLabel =Label::create("","Copperplate Gothic Bold.ttf",80);
	_scoreLabel->setColor(Color3B(255,255,255));
	_scoreLabel->setString("0");
    _scoreLabel->setAnchorPoint(cocos2d::Point(0,0.5));
	this->addChild(_scoreLabel);
    cocos2d::Point spo=score->getPosition();
	_scoreLabel->setPosition(spo.x-110,spo.y);
    
    Sprite *gold = Sprite::create("ui/top/goldBg.png");
    this->addChild(gold);

    gold->setPosition(visibleSize.width-gold->getContentSize().width/2,visibleSize.height-gold->getContentSize().height/2);
   	_goldLabel = Label::create("","Copperplate Gothic Bold.ttf",80);
    _goldLabel->setAnchorPoint(cocos2d::Point(0,0.5));
    _goldLabel->setColor(Color3B(255,255,255));
    //_goldLabel->enableOutline(Color4B(0,0,0,255),2);
    _goldLabel->setPosition(gold->getPosition().x-110,gold->getPosition().y);
   
    this->addChild(_goldLabel);
    this->updateGold(_runGold);
    
   
	cocos2d::Point uiPoint =cocos2d::Point(gameFrame.x+gameFrame.width-40,gameFrame.y+gameFrame.height-30);
	for(int i=0; i<PROGRESS_PAOPAO_NUMBER; i++){
		Sprite *bg = Sprite::create("monster/heartBg.png");
		bg->setPosition(uiPoint.x,uiPoint.y);
		this->addChild(bg);
		_progressBubble[i] = Bubble::initWithType(PAOPAO_HEART);
		Bubble *pp = _progressBubble[i];
		this->addChild(pp);
		pp->setPosition(uiPoint.x,uiPoint.y);
		
		uiPoint.x -= pp->getContentSize().width+1;
	}
	iceSprite = Sprite::create("power/ice.png");
	this->addChild(iceSprite);
	cocos2d::Size iceSz = iceSprite->getContentSize();
	iceSprite->setVisible(false);
	iceSprite->setPosition(uiPoint.x+iceSz.width/2,uiPoint.y);


	//Sprite *hp = Sprite::create("score/hp.png");
    Sprite *hp = Sprite::create("ui/top/hp.png");

	_blood = ProgressTimer::create(hp);
	_blood->setType(ProgressTimer::Type::BAR);
	_blood->setMidpoint(cocos2d::Point(0,0));
	_blood->setBarChangeRate(cocos2d::Point(1, 0));
	_blood->setPosition(cocos2d::Point(111, 30));
	_blood->setAnchorPoint(cocos2d::Point(0,0));
	_blood->setPercentage(100);

	//uiPoint = bg->getPosition();
	//Sprite *boss = Sprite::create("ui/scoreUI/BOSS.png");
	//this->addChild(boss);
    //Size bsz = boss->getContentSize();
	//boss->setPosition(po.x+30,po.y+bsz.height/2-10);
	//Sprite *bloodBg = Sprite::create("score/hpBg.png");
    Sprite *bloodBg = Sprite::create("ui/top/hpBg.png");
    Size bsz = bloodBg->getContentSize();
	bloodBg->addChild(_blood,2);
    bloodBg->setAnchorPoint(cocos2d::Point(0,0));
	bloodBg->setPosition(gameFrame.x,stagePo.y-stageBg->getContentSize().height/2-bsz.height);
	this->addChild(bloodBg);

//    if(visibleSize.width> UIWIDTH + 20)
//    {
//        bloodBg->setScale( visibleSize.width / UIWIDTH,visibleSize.height/UIHEIGHT);
//        
//    }
   
	

	_bossHp = Label::create("","Copperplate Gothic Bold.ttf",30);
	_bossHp->setColor(Color3B(255,255,255));
	//_bossHp->enableOutline(Color4B(0,0,0,255),1);

	_bossHp->setString("HP: ");

	this->addChild(_bossHp);
	_bossHp->setPosition(gameFrame.x+bloodBg->getContentSize().width/2+40,stagePo.y-stageBg->getContentSize().height/2-bsz.height/2);
   

	this->setBossHp(g_gameLayer->getCurrentBarrierData()->bossHp,g_gameLayer->getCurrentBarrierData()->bossHp);
    
    
    
    
    
    if(visibleSize.width<IPADWIDTH)
    {
        float radioX = visibleSize.width / IPADWIDTH;
        float radioY = visibleSize.height / IPADHEIGHT ;
        bloodBg->setScale(radioX,radioY);
        bloodBg->setPosition(gameFrame.x,stagePo.y-(bsz.height*radioY));
        
        score->setScale(radioX,radioY);
        score->setPosition(gameFrame.x+gameFrame.width/2-25,visibleSize.height-(score->getContentSize().height*radioY)/2);
#ifndef _IOS_
#define _IOS_
         _scoreLabel->setSystemFontSize(40);
		  _goldLabel->setSystemFontSize(40);
        _stageLabel->setSystemFontSize(40);
		  _bossHp->setSystemFontSize(20);
#endif
    
        _scoreLabel->setPosition(score->getPosition().x-50,visibleSize.height-(score->getContentSize().height*radioY)/2);
        
        
        gold->setScale(radioX,radioY);
        gold->setPosition(visibleSize.width-(gold->getContentSize().width*radioX)/2,visibleSize.height-(gold->getContentSize().height*radioY)/2);

        _goldLabel->setPosition(gold->getPosition().x-50,visibleSize.height-(gold->getContentSize().height*radioY)/2);
        
        _bossHp->setPosition(gameFrame.x+(bloodBg->getContentSize().width*radioX)/2+15,stagePo.y-(bsz.height*radioY)/2);
        
        
        
        stageBg->setScale(radioX,radioY);
        stagePo = cocos2d::Point(gameFrame.x + (stageBg->getContentSize().width*radioX)/2 ,visibleSize.height-(stageBg->getContentSize().height*radioY)/2);
        stageBg->setPosition(stagePo);
        _stageLabel->setPosition(stagePo.x,stagePo.y);
        
        _scoreLabel->setSystemFontName("Copperplate Gothic Bold.ttf");
        _goldLabel->setSystemFontName("Copperplate Gothic Bold.ttf");
        _stageLabel->setSystemFontName("Copperplate Gothic Bold.ttf");
        _bossHp->setSystemFontName("Copperplate Gothic Bold.ttf");
        
    }
    
    return true;
}  
void ScoreLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
		{
			auto coinShopUI = CoinShopUI::create();
			g_miscLayer->addChild(coinShopUI);
			coinShopUI->setTag(COINHELPUI);
		}
		break;
	}

}
void ScoreLayer::updateGold(int gold)
{
	char si[32];
	sprintf(si, "%d", gold);
	_goldLabel->setString(si);
	
}
void ScoreLayer::setStage(int stage)
{
    char s[64];
    sprintf(s, "Stage %d",stage);
    _stageLabel->setString(s);
}
void ScoreLayer::setProgressLeftPaoPao(int dieNumber, int resetHeartRound)
{
	int totalLife = PROGRESS_PAOPAO_NUMBER-_resetHeartRound;
	int currentIndex = dieNumber-1 + _resetHeartRound;

	_resetHeartRound = resetHeartRound;
	_dieBubbleCount = dieNumber;
	if(dieNumber==totalLife)
		_dieBubbleCount = 0;
   
	if(dieNumber==0){
		this->resetProgress(this);
		return;
	}

	if(dieNumber==totalLife){
		_progressBubble[currentIndex]->runAction(
			Sequence::create(
			FadeOut::create(0.5f),
			CallFuncN::create(this, callfuncN_selector(ScoreLayer::resetProgress)),
			NULL
			)
			);
	}
	if(dieNumber<totalLife){
		_progressBubble[currentIndex]->runAction(
			Sequence::create(
			FadeOut::create(0.8f),
			CallFuncN::create(this, callfuncN_selector(ScoreLayer::resetProgress)),
			NULL
			)
			);
	}
	
	/*for (int i=dieNumber+resetHeartRound; i<PROGRESS_PAOPAO_NUMBER; i++)
	{
		//_progressPaoPao[i]->set
		_progressBubble[i]->setOpacity(255);
		_progressBubble[i]->setVisible(true);
	}*/
	
}



void ScoreLayer::changeNumber(Sprite *sp, int num)
{
	
	char sn[32];
	sprintf(sn, "score/%d.png", num);

	Texture2D *textture;  

	Sprite * temp_obj = Sprite::create(sn);  

	textture = temp_obj->getTexture();  


	//∏ƒ±‰my_heroµƒÕº∆¨  
	sp->stopAllActions();  

	sp->setTexture(textture);  
}

void ScoreLayer::changeHpNumber(Sprite *sp, int num)
{
	char sn[32];
	sprintf(sn, "score/hp/%d.png", num);

	Texture2D *textture;  

	Sprite * temp_obj = Sprite::create(sn);  

	textture = temp_obj->getTexture();  


	sp->stopAllActions();  

	sp->setTexture(textture);  
}
void ScoreLayer::addScore(int n)
{
	_score += n;
	int score = _score;
    char s[64];
	sprintf(s, "%d", score);
    _scoreLabel->setString(s);
}

void ScoreLayer::showLog(const char * pStr)
{
	//_scoreLabel->setString(pStr);
}

void ScoreLayer::clearScore()
{
	_score = 0;
    _runGold=0;
	addScore(0);
}

int ScoreLayer::getScore()
{
	return _score ;
}

void ScoreLayer::reset()
{
	_resetHeartRound = 0;
	resetProgress(NULL);
	_blood->setPercentage(100);
	this->setBossHp(g_gameLayer->getCurrentBarrierData()->bossHp,g_gameLayer->getCurrentBarrierData()->bossHp);
    this->updateGold(0);
}

void ScoreLayer::resetBossHpShow()
{
	_blood->setPercentage(100);
	this->setBossHp(g_gameLayer->getCurrentBarrierData()->bossHp,g_gameLayer->getCurrentBarrierData()->bossHp);
}

void ScoreLayer::resetProgress(Node *node)
{
	for (int i=0; i<_dieBubbleCount+_resetHeartRound; i++)
	{
		_progressBubble[i]->setVisible(false);
	}
	for (int i=_dieBubbleCount+_resetHeartRound; i<PROGRESS_PAOPAO_NUMBER; i++)
	{
		//_progressPaoPao[i]->set
		_progressBubble[i]->setOpacity(255);
		_progressBubble[i]->setVisible(true);
	}
}


void ScoreLayer::setBossHp(int hp,int bossHp)
{
	char s[64];
	sprintf(s, "HP: %d/%d", hp,bossHp);
	_bossHp->setString(s);


	
	int progressFrom = _blood->getPercentage();

	int progressTo =(static_cast<float>(hp) / static_cast<float>(bossHp))*100;

	ProgressFromTo *to2 = ProgressFromTo::create(0.2f, progressFrom, progressTo);
	_blood->runAction(to2);
}


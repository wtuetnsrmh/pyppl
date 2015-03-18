#include "ShotLayer.h"
#include "GameConst.h"
#include "GunSprite.h"
#include "Global.h"
#include "SimpleAudioEngine.h"
#include "Bubble.h"
#include "AssistLine.h"
#include "BarrierData.h"
#include "SkillSprite.h"
#include "TalentData.h"
#include "GrassSprite.h"
#include "GrassBubble.h"
using namespace CocosDenshion;  

ShotLayer::ShotLayer(void)
{
	reset();
}


ShotLayer::~ShotLayer(void)
{
}
bool ShotLayer::init()  
{
    if ( !CCLayer::init() )  
    {  
        return false;  
    }  
    srand(time(NULL));  
	
    return true;  
} 

void ShotLayer::initGun()
{
	cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Size  visibleSize = Director::getInstance()->getWinSize();  

	//_gunBg = Sprite::create("gun/gun_bg.png");
    //_gunBg = Sprite::create("ipad/gun_bg.png");
	//this->addChild(_gunBg);
	MyRect gameFrame = g_gameLayer->getGameFrame();
	
	//_gunBg->setScale(PIC_RADIO);
	//cocos2d::Size  bgSz = _gunBg->getContentSize();
    
    //if(bgSz.height==0)
    //{
    //    _gunBg->setContentSize(cocos2d::Size(gameFrame.width,500));
    //}

    
	int diffy = 0 ;// (gunBg->getContentSize().height - 100)/2 *PIC_RADIO;
	//_gunBg->setPosition(cocos2d::Point(gameFrame.x + gameFrame.width/2 , gameFrame.y+gameFrame.height+diffy));

    _gunPos = cocos2d::Point(gameFrame.x + gameFrame.width/2 , gameFrame.y+gameFrame.height+diffy);
    
	_gun = GunSprite::initWithPic("gun/1.png");
	
	_gun->setPosition(_gunPos);
	_gun->setLocalZOrder(1);
	this->addChild(_gun,0);
	_gun->setScale(PIC_RADIO*1.3);
    
    Sprite *_nextBubbleBg = Sprite::create("gun/next.png");
    this->addChild(_nextBubbleBg);
    _nextBubbleBg->setScale(PIC_RADIO/1.2);
    int dif = 0;
    if (visibleSize.width<IPADWIDTH) {
        dif = 10;
    }else
    {
        dif =30;
    }
    _nextBubblePo = cocos2d::Point(_gunPos.x-((_nextBubbleBg->getContentSize().width)*(PIC_RADIO/1.2)/2)-dif,_gunPos.y+20);
    _nextBubbleBg->setPosition(_nextBubblePo);
    
    

	this->genShotPaoPao();


	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
}
Bubble * ShotLayer::randomBubble4Shot(PAOPAO_TYPE type)
{  
	int typeCount = g_gameLayer->getCurrentBarrierData()->bubbleTypeCount;
	typeCount = std::min(typeCount, PAOPAO_TOTAL_TYPE_COUNT);
	Bubble *pRet = NULL;  
	if(type == PAOPAO_TYPE_UNKNOW)
		type = static_cast<PAOPAO_TYPE>(rand() % typeCount + 1);  
	pRet =	Bubble::initWithType(type);
	return pRet;  
} 

Bubble *  ShotLayer::randomBubble4Noraml()
{
	Bubble *pRet = NULL;  
	PAOPAO_TYPE type = static_cast<PAOPAO_TYPE>(rand() % PAOPAO_NORMAL_TYPE_COUNT + 1);  
	pRet =	Bubble::initWithType(type);
	return pRet;  
}

Bubble* ShotLayer::genSpecialBubble4RandomAdd(PAOPAO_TYPE type)
{
	_specialBubbleShowMap[type] = 1;
	Bubble *pRet =	Bubble::initWithType(type);
	
	return pRet;  
}

void ShotLayer::genShotPaoPao(PAOPAO_TYPE type)
{
	cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Size  visibleSize = Director::getInstance()->getWinSize();

	_currentBubble = randomBubble4Shot(type); //gamerestat µƒ ±∫Ú£¨±£¡Ù…œ“ªæ‰µƒtype
	this->addChild(_currentBubble);
	
	_shotPaoPaoPos = _gunPos;//ccp(origin.x + visibleSize.width/2, origin.y+visibleSize.height);
	_currentBubble->setPosition(_shotPaoPaoPos.x, _shotPaoPaoPos.y );

	this->_nextBubble = randomBubble4Shot();
	this->addChild(_nextBubble);
    
	_nextBubble->setPosition(_nextBubblePo);
	_nextBubble->setScale(PIC_RADIO/1.3);
	_currentBubble->setLocalZOrder(2);
	
	//if(_currentBubble->getColorType()>=PAOPAO_TYPE1 && _currentBubble->getColorType()<=PAOPAO_TYPE6)
		//_currentBubble->playNormalAction(true);
}


bool ShotLayer::onTouchBegan (Touch *touch, Event *unused_event)
{
	MyRect gameFrame = g_gameLayer->getGameFrame();
    
	cocos2d::Rect rect = cocos2d::Rect(gameFrame.x,gameFrame.y,gameFrame.width,gameFrame.height);
	cocos2d::Point touchPo=touch->getLocation();
    
    if (rect.containsPoint(touchPo))//≈–∂œ¥•√˛µ„ «∑Ò‘⁄ƒø±Íµƒ∑∂Œßƒ⁄
        return true;
	else
        return false;
}
void ShotLayer::onTouchMoved (Touch *touch, Event *unused_event)
{
cocos2d::Point  pt = touch->getLocation();
	float angleDegrees = getRotationByPos(pt);
	_gun->setRotation(angleDegrees);
	
	if(_currentBubble){
		_currentBubble->setRotation(angleDegrees);
	}


	showAssistLine(_gun->getPosition(), angleDegrees);

}
void ShotLayer::onTouchEnded (Touch *touch, Event *unused_event)
{
	
	if(_isShotting == true||_enableShotting==true)
		return;
	//‚àë¬¢‚Ä¶‚Ä?
cocos2d::Point  pt = touch->getLocation();
	if(pt.y >= _gunPos.y ){
		return;
	}
	float angleDegrees = getRotationByPos(pt);
	_gun->setRotation(angleDegrees);

	if(_currentBubble){
		_currentBubble->setRotation(angleDegrees);
	}

	float offRealY = pt.y - _gunPos.y ;
	float offRealX = pt.x - _gunPos.x;
	float totalSpeed = sqrt( pow(offRealY, 2) + pow(offRealX, 2) );
	if(totalSpeed == 0)
		totalSpeed = 1;

	float fps = Director::getInstance()->getAnimationInterval();
	_speedX = offRealX / totalSpeed * PAOPAO_SPEED * fps;
	_speedY = offRealY / totalSpeed * PAOPAO_SPEED * fps;

	if(_currentBubble && _currentBubble->getColorType() == PAOPAO_PASS){
		_speedX *= 2;
		_speedY *= 2;
	}

    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    if(visibleSize.width>UIWIDTH)
    {
        _speedX *= 2;
		_speedY *= 2;
    }
	shotBubble();

	showAssistLine(_gun->getPosition(), angleDegrees);

	_currentAngle = angleDegrees;

	g_gameLayer->addShotPower();
	this->_currentShottingType = PAOPAO_TYPE_UNKNOW;
}

void ShotLayer::updateLoop(float fDelta)
{
	if(_isShotting==true)
		this->bubbleMove(_currentBubble,fDelta, true);

	if(_isOver == false)
	{
		for(size_t i=0; i<_randomBubbleList.size(); ++i){ 
			this->bubbleMove(_randomBubbleList[i], fDelta, false);
		}
		for(size_t i=0; i<_specialBubbleList.size(); ++i){
			this->bubbleMove(_specialBubbleList[i], fDelta, false);
		}
	}
	else{
		for(size_t i=0; i<_randomBubbleList.size(); ++i){ 
			this->removeChild(_randomBubbleList[i]);
		}
		_randomBubbleList.clear();
	}
}

void ShotLayer::bubbleMove(Bubble *pp, float fDelta, bool isShotPaoPao=false)
{
	if(g_gameLayer == NULL || pp->getBubbleState() == Bubble_S_Dirty)
	{
		return;
	}
	
	float deltaX = pp->getSpeedX() ;
	float deltaY =  pp->getSpeedY() ;
	MyRect gameFrame = g_gameLayer->getGameFrame();
cocos2d::Point  currentPt = pp->getPosition();
	currentPt.x += deltaX;
	currentPt.y += deltaY;
	pp->setPosition(currentPt);

	int x = pp->getPosition().x;  
	int y = pp->getPosition().y; 
	//if(true == processSpecialBubbleInShotLayer(pp))
		//return;
	if(isShotPaoPao == true)
	{
		for(size_t i=0; i<_grassList.size(); i++)
		{
			GrassSprite* grass = _grassList.at(i);
			if(grass->isVisible() == true)
			{
				if(grass->getBubble() != NULL)
				{
					cocos2d::Point bubblePos = grass->getBubble()->convertToWorldSpaceAR(cocos2d::Point(0, 0));
					if(this->isCollision(pp->getPosition(), bubblePos, PAOPAO_SIZE/2, PAOPAO_SIZE/2) == true)
					{
						this->removeChild(pp);
						_currentBubble = NULL;
						genNextPaoPao();
						_isShotting = false;
						grass->showEffect(grass->getBubble()->getBubbleType());
						grass->removeABubble();
    
						if(g_gameLayer->getGameState()!=G_OVER&&_heartFreeze==0){
							_shotNullCount++;
							if(_shotNullCount < PROGRESS_PAOPAO_NUMBER-_resetHeartRound)
								g_scoreLayer->setProgressLeftPaoPao(_shotNullCount, _resetHeartRound);
							else{
								_resetHeartRound++;
			
								_resetHeartRound = _resetHeartRound % PROGRESS_PAOPAO_NUMBER;
								g_scoreLayer->setProgressLeftPaoPao(_shotNullCount, _resetHeartRound);
								_shotNullCount = 0;
								this->addBubbleDifficulty();
							}
						}

						return;
					}
				}
			}
		}
	}
	if(pp->getColorType() == PAOPAO_PASS)
	{
		if(  (x>=gameFrame.width+gameFrame.x - PAOPAO_SIZE/2)
			|| (x<=gameFrame.x + PAOPAO_SIZE/2)  
			|| (y<=gameFrame.y+PAOPAO_SIZE/2)  
			)
		{
			std::vector<RowCol> passList = g_gameLayer->checkPass(static_cast<PAOPAO_TYPE>(pp->getOldColorType()), _gun->getPosition().x, _gun->getPosition().y, x, y);
			this->removeChild(pp);
			_currentBubble = NULL;
			genNextPaoPao();
			_isShotting = false;
		}
		return;
	}

	if(x>=gameFrame.width+gameFrame.x - PAOPAO_SIZE/2 && pp->getSpeedX() > 0)  
	{  
		float nextX = x - 2 * (x - (gameFrame.x + gameFrame.width) );
		//pp->setPosition(cocos2d::Point(gameFrame.x +PAOPAO_SIZE/2,y));  
		pp->setPosition(cocos2d::Point(nextX, y));  
		pp->setSpeedX(	abs(pp->getSpeedX()) * -1 );  
		pp->setCollisionCount(pp->getCollisionCount()+1);
	}
	else if (x<=gameFrame.x + PAOPAO_SIZE/2 && pp->getSpeedX() <0)  
	{  
		float nextX = x +  2 * (gameFrame.x-x) ;
		//pp->setPosition(cocos2d::Point(gameFrame.x +PAOPAO_SIZE/2,y));  
		pp->setPosition(cocos2d::Point(nextX, y));
		pp->setSpeedX(	abs(pp->getSpeedX()) );  
		pp->setCollisionCount(pp->getCollisionCount()+1);
	}
	else if (y<=gameFrame.y+PAOPAO_SIZE/2)  
	{  
		pp->setPosition(cocos2d::Point(x,gameFrame.y+PAOPAO_SIZE/2));  
		pp->setSpeedY(pp->getSpeedY() * -1);
		pp->setCollisionCount(pp->getCollisionCount()+1);
	}
	else if(pp->getCollisionCount()>0 && y>=gameFrame.height+gameFrame.y-PAOPAO_SIZE/2){
		pp->setPosition(cocos2d::Point(x,gameFrame.height+gameFrame.y-PAOPAO_SIZE/2));  
		pp->setSpeedY(pp->getSpeedY() * -1);  
		pp->setCollisionCount(pp->getCollisionCount()+1);
	}

	if(pp->getCollisionCount() >= 6 && isShotPaoPao==true){
		this->removeChild(pp);
		_currentBubble = NULL;
		this->stopShot(true, false);
	}
	else{
		if(isShotPaoPao==true){
			bool findPp = g_gameLayer->stickAGrid(pp, this);
		}
		else{
				bool findPP = g_gameLayer->stickAGrid2(pp, this);
		}
	}

}
/*
bool ShotLayer::processSpecialBubbleInShotLayer(Bubble *pBubble)
{
	MyRect gf = g_gameLayer->getGameFrame();
	int y = pBubble->getPosition().y;
	if(pBubble->getColorType() == PAOPAO_MOZHI){
		if (y<=gf.y+PAOPAO_SIZE/2)  
		{ 
			removeSpecialBubbleInShotLayer(pBubble);
			return true;
		}
	}
	return false;
}

void ShotLayer::processSpecialBubbleWhenStick(Bubble *pBubble)
{
	if(pBubble->getColorType() == PAOPAO_MOZHI)
	{
		Bubble * stickPp = pBubble->getStickPp();
		if(stickPp->getColorType() == PAOPAO_TYPE_CENTER){
			this->removeSpecialBubbleInShotLayer(pBubble);
			return;
		}
		std::vector<Bubble *>::iterator itList;
		for( itList = _specialBubbleList.begin(); itList != _specialBubbleList.end(); )
		{
			if( pBubble == (*itList) )
			{
				itList = _specialBubbleList.erase( itList);
				break;
			}
			else
				itList++;
		}
cocos2d::Point  oldMozhiPoint = pBubble->getPosition();
cocos2d::Point  mozhiPoint = pBubble->convertToWorldSpaceAR(Point(0,0));
cocos2d::Point  stickPoint =	stickPp->convertToNodeSpaceAR(Point(0, 0));

		float diffx = stickPoint.x - mozhiPoint.x ;
		float diffy = stickPoint.y - mozhiPoint.y ;


		pBubble->runAction(Sequence::create(
			MoveTo::create(0.1f,Point(oldMozhiPoint.x+diffx, oldMozhiPoint.y+ diffy)),
			ScaleTo::create(0.3f,0.5,0.5),
			CallFuncN::create(this, callfuncN_selector(ShotLayer::removeSpriteInShotLayer)),
			DelayTime::create(0.3f),
			NULL));


		stickPp->surroundMozhi();
	}
}*/

void ShotLayer::removeSpriteInShotLayer(Node *node)
{
	this->removeChild(node, true);
}

void ShotLayer::removeSpecialBubbleInShotLayer(Bubble *pBubble)
{
	std::vector<Bubble *>::iterator itList;
	for( itList = _specialBubbleList.begin(); itList != _specialBubbleList.end(); )
	{
		if( pBubble == (*itList) )
		{
			itList = _specialBubbleList.erase( itList);
			break;
		}
		else
			itList++;
	}
	this->removeChild(pBubble, true);
}

void ShotLayer::genNextPaoPao()
{
	if(_currentBubble != NULL && _currentBubble->getParent() == this)
	{
		return;
	}

	if (_electricBubble > 0)
	{
		_currentBubble = randomBubble4Shot();
		this->addChild(_currentBubble);
		_currentBubble->setPosition(this->_shotPaoPaoPos.x, this->_shotPaoPaoPos.y );
		changeShottingBubble(PAOPAO_BOMB);
		_electricBubble--;
	}

	else
	{
		_currentBubble = _nextBubble;

		_currentBubble->setPosition(this->_shotPaoPaoPos.x, this->_shotPaoPaoPos.y );
		this->_nextBubble = randomBubble4Shot();
		this->addChild(_nextBubble);
		_nextBubble->setPosition(_nextBubblePo);
		_nextBubble->setScale(PIC_RADIO/1.3);

		_currentBubble->setLocalZOrder(2);
		_currentBubble->setScale(PIC_RADIO);

		//_currentBubble->playNormalAction(true);
	}
	
	
}

void ShotLayer::restoreCurrentBubblePosition()
{
	if(_currentBubble != NULL){
		_currentBubble->setPosition(this->_shotPaoPaoPos.x, this->_shotPaoPaoPos.y );
	}
}

void ShotLayer::gameOver()
{
	_isShotting = true;//‚â§¬™∆í‚Äπ‚Äò≈∏‚àë¬¢‚Ä¶‚Ä?
	if(_currentBubble && _currentBubble->getParent()==this){
		_currentBubble->setSpeedXY(0, 0);
		this->restoreCurrentBubblePosition();
	}
	_isOver = true;
	//_resetHeartRound = 0;
	/*std::vector<Bubble *>::iterator itList;
	for( itList = _randomBubbleList.begin(); itList != _randomBubbleList.end(); )
	{
		Bubble * bb = (*itList);
		this->removeChild(bb);
	}
	_randomBubbleList.clear();*/
	//this->removeChild(_currentBubble);
	//this->removeChild(_nextBubble);
	//_currentBubble=_nextBubble = NULL;
}

void ShotLayer::gameRestart()
{
	_isShotting = false;
    _enableShotting =false;
	_isOver = false;
	_specialBubbleShowMap.clear();
	this->removeChild(_currentBubble);
	this->removeChild(_nextBubble);
	genShotPaoPao(_currentShottingType);
	genNextPaoPao();
	this->restoreCurrentBubblePosition();
	_currentShottingType = PAOPAO_TYPE_UNKNOW;
	//resetHeartKeepResetRound();
}

void ShotLayer::reset()
{
	_lineShowCount = 0;
	_isShotting = false;
    _enableShotting =false;
	_shotNullCount = 0;
	_randomBubbleList.clear();
	_specialBubbleList.clear();
	_lastBossShowtime = time(NULL);
	_resetHeartRound = 0;
	_currentAngle = 0;
	_bShowLine = false;

	_heartFreeze = 0;

	_electricBubble = 0;

	_isOver = false;

	 _currentShottingType = PAOPAO_TYPE_UNKNOW;

	 _shotCount = 0;
	 _grassShowCount = -1;
}

void ShotLayer::stopShot(bool bGenNextPaoPao, bool incNullBubble)
{
	_isShotting = false;

	if(bGenNextPaoPao == true){
		genNextPaoPao();
	}
	else{
		_currentBubble = NULL;
	}
	if (_heartFreeze > 0)
	{
		_heartFreeze--;
		if(_heartFreeze==0)
		{
			g_scoreLayer->iceSprite->setVisible(false);
		}
		return;
	}

	if(incNullBubble == true && g_gameLayer->getGameState()!=G_OVER){	
		_shotNullCount++;
		if(_shotNullCount < PROGRESS_PAOPAO_NUMBER-_resetHeartRound)
			g_scoreLayer->setProgressLeftPaoPao(_shotNullCount, _resetHeartRound);
		else{
			_resetHeartRound++;
			
			_resetHeartRound = _resetHeartRound % PROGRESS_PAOPAO_NUMBER;
			g_scoreLayer->setProgressLeftPaoPao(_shotNullCount, _resetHeartRound);
			_shotNullCount = 0;
			this->addBubbleDifficulty();
		}
	}

	int index = this->genGrass();
	if(index != -1){
		this->showAGrass(index);
	}
	else{
		this->checkGrassDie();
	}
}

/**********************************private ********************************************/
void ShotLayer::shotBubble()
{
	if(_currentBubble && _currentBubble->getEnableShot() && _isShotting==false){
		_currentBubble->setSpeedX(_speedX);
		_currentBubble->setSpeedY(_speedY);
		_isShotting = true;
		g_miscLayer->playWav("shot.wav" );

		//showBoss();
		//g_gameLayer->earthquake();

		//g_gameLayer->genInkBubble();
		this->setLienShow(false);

		if(_currentBubble->getColorType() == PAOPAO_PASS){
			_currentBubble->changeImage("effect/bubble/pass.png");
		}

		_shotCount++;
		
	}

}

float ShotLayer::getRotationByPos(Point mouseCurser)
{
cocos2d::Point  pt = mouseCurser;
	float offRealY = pt.y - _gunPos.y ;
	if(offRealY>0)
		offRealY = -0.0001f;
	float offRealX = pt.x - _gunPos.x;
	float angleRadians = atanf((float)offRealX / (float)offRealY);

	float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);

	return angleDegrees;
}

void ShotLayer::testButton()
{
	return;
	ui::Button *button= ui::Button::create();  
	button->setTouchEnabled(true);
	
	char s1[64];
	sprintf(s1,"ui/restart-1.png");
	char s2[64];
	sprintf(s2,"ui/restart-2.png");
	button->loadTextures(s1,s2,"");
	button->setTag(1);
	button->addTouchEventListener(this, toucheventselector(ShotLayer::testTouchEvent));
	this->addChild(button, 2);
	button->setPosition( cocos2d::Point(50, 50) );
}

void ShotLayer::testTouchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(tag){
	case 1:
		switch(type){
		case TOUCH_EVENT_BEGAN:

			break;
		case TOUCH_EVENT_MOVED:

			break;
		case  TOUCH_EVENT_ENDED:
			g_gameLayer->gameRestart();
			break;
		}
		break;
	case 8:

		break;
	}

}
cocos2d::Point  ShotLayer::generateSpeed(Point start,
cocos2d::Point  target)
{
	float offRealY = target.y - start.y ;
	float offRealX = target.x - start.x;
	float totalSpeed = sqrt( pow(offRealY, 2) + pow(offRealX, 2) );
	if(totalSpeed == 0)
		totalSpeed = 1;
	float fps = Director::getInstance()->getAnimationInterval();
	float speedX = offRealX / totalSpeed * PAOPAO_SPEED * fps;
	float speedY = offRealY / totalSpeed * PAOPAO_SPEED * fps;

	return cocos2d::Point(speedX, speedY);
}

//‚ÄòÀÜ¬∫‚Äù∆í‚Äî‚àÇ¬ª
void ShotLayer::addBubbleDifficulty()
{
	g_miscLayer->playWav("suiji.wav");
	//g_gameLayer->genSpecailBubbleInRule();

	MyRect gameFrame = g_gameLayer->getGameFrame();
cocos2d::Point  ptList[8] = 
	{
		cocos2d::Point(gameFrame.x, gameFrame.y), cocos2d::Point( gameFrame.width+gameFrame.x, gameFrame.y), cocos2d::Point(gameFrame.x, gameFrame.height+gameFrame.y), cocos2d::Point(gameFrame.width+gameFrame.x, gameFrame.height+gameFrame.y),
		cocos2d::Point(gameFrame.x + gameFrame.width/2, gameFrame.y), cocos2d::Point(gameFrame.x + gameFrame.width/2, gameFrame.y+gameFrame.height),
		cocos2d::Point(gameFrame.x , gameFrame.y+gameFrame.height/2), cocos2d::Point(gameFrame.x + gameFrame.width, gameFrame.y+gameFrame.height/2)
	};

	//TalentData *talent = g_talentMgr->getTalent(TT_Add_Random_Count);
	

	int addCount = rand() % 3 + 6;

	BarrierData *curBarrier = g_gameLayer->getCurrentBarrierData();

	if(curBarrier->suiji==0)
		return;

	for(int i=0; i<addCount; i++){
		
		Bubble *pp = NULL;

		
		PAOPAO_TYPE type = whichSepcialType(curBarrier, i);
		
		if( (type == PAOPAO_INK || type == PAOPAO_PLAGUE)
			&& g_gameLayer->hasBubbleWithSameType(type)==false)
		{	
			pp = genSpecialBubble4RandomAdd(type);
		}
		else if(type == PAOPAO_STONE)
		{
			pp = genSpecialBubble4RandomAdd(type);
		}
		else if(type == PAOPAO_GUARD && g_gameLayer->bubbleWithSameType(type) < 3)
		{
			pp = genSpecialBubble4RandomAdd(type);
		}
		

		if(pp == NULL)
			pp  = randomBubble4Shot();
		pp->setPosition( ptList[i] );
		cocos2d::Point  speedxy = generateSpeed(ptList[i], g_gameLayer->getCenterPoint());
		pp->setSpeedXY(speedxy.x, speedxy.y);
		this->addChild(pp);
		_randomBubbleList.push_back(pp);
	}
	

}

//‚Äù¬∂‚àè‚àö‚â•ÀÜ≈ì√∑∆í∆í¬ø‚Ä°¬´√?
PAOPAO_TYPE ShotLayer::whichSepcialType(BarrierData *curBarrier, int pos)
{
	if(pos == 0)
	{
		if(curBarrier->mozhi==1 && curBarrier->wenyi==1){
			if (_specialBubbleShowMap.find(PAOPAO_INK) != _specialBubbleShowMap.end()
				&& _specialBubbleShowMap.find(PAOPAO_PLAGUE) == _specialBubbleShowMap.end()
				)
			{
				return PAOPAO_PLAGUE;
			}
			else if (
				_specialBubbleShowMap.find(PAOPAO_INK) == _specialBubbleShowMap.end()
				&& _specialBubbleShowMap.find(PAOPAO_PLAGUE) != _specialBubbleShowMap.end()
				)
			{
				return PAOPAO_INK;
			}
			else{
				if(rand() %2==0)
					return PAOPAO_INK;
				else
					return PAOPAO_PLAGUE;
			}
		}
		else{
			if(curBarrier->mozhi==1)
				return PAOPAO_INK;
			if (curBarrier->wenyi==1)
				return PAOPAO_PLAGUE;
		}

	}
	if(pos ==1)
	{
		if(curBarrier->shitou==1 && curBarrier->huwei==1){
			if (_specialBubbleShowMap.find(PAOPAO_STONE) != _specialBubbleShowMap.end()
				&& _specialBubbleShowMap.find(PAOPAO_GUARD) == _specialBubbleShowMap.end()
				)
			{
				return PAOPAO_GUARD;
			}
			else if (
				_specialBubbleShowMap.find(PAOPAO_STONE) == _specialBubbleShowMap.end()
				&& _specialBubbleShowMap.find(PAOPAO_GUARD) != _specialBubbleShowMap.end()
				)
			{
				return PAOPAO_STONE;
			}
			else{
				if(rand() %2==0)
					return PAOPAO_GUARD;
				else
					return PAOPAO_STONE;
			}
		}
		else{
			if(curBarrier->shitou==1)
				return PAOPAO_STONE;
			if(curBarrier->huwei==1)
				return PAOPAO_GUARD;
		}
	}
	return PAOPAO_TYPE_UNKNOW;
}

void ShotLayer::stopRandomBubble(Bubble *bb)
{
	
	std::vector<Bubble *>::iterator itList;
	for( itList = _randomBubbleList.begin(); itList != _randomBubbleList.end(); )
	{
		if( bb == (*itList) )
		{
			itList = _randomBubbleList.erase( itList);
			break;
		}
		else
			itList++;
	}
}



/*void ShotLayer::addMozhiBubble(Point pos, float delayTime)
{
	float fps = Director::getInstance()->getAnimationInterval();
	Bubble * pp = Bubble::initWithPic("monster/mozhi.png");
	pp->setColorType(PAOPAO_MOZHI);
	pp->setPosition(pos.x-9,pos.y-10);
	pp->setSpeedX(0);
	pp->setSpeedY(-PAOPAO_SPEED / 6 * fps);
	this->addChild(pp);
	pp->setVisible(false);
	pp->runAction(Sequence::create(DelayTime::create(delayTime), CallFuncN::create(this, callfuncN_selector(ShotLayer::addBubble2SpecialList)), NULL
		));
}*/

void ShotLayer::addBubble2SpecialList(Node *pp)
{
	pp->setVisible(true);
	_specialBubbleList.push_back( dynamic_cast<Bubble*>(pp) );	
	
}

void ShotLayer::showAssistLine(Point start, float angle)
{

	AssistLine* line = dynamic_cast<AssistLine*>( this->getChildByTag(TAG_ASSIST_LINE) );
	if(line == NULL){
		line = AssistLine::create();
		this->addChild(line, 0, TAG_ASSIST_LINE);
	}
	line->setVisible(_bShowLine);
	if(_bShowLine == true){
		line->showLine(start, angle);
	}
	else{
		line->stopScheduleUpdate();
	}
}

void ShotLayer::setLienShow(bool show)
{
	
	if(show == true){
		_lineShowCount = g_talentMgr->getCurrentEffectValue(TT_AssistLine);
		this->_bShowLine = show;
	}
	else{
		_lineShowCount--;
		if (_lineShowCount<=0)
		{
			this->_bShowLine = show;
		}
	}
	showAssistLine(_gun->getPosition(), _currentAngle);
}


void ShotLayer::changeShottingBubble(PAOPAO_TYPE type)
{
	_currentShottingType = type;
	if(_currentBubble){
		_currentBubble->changeType(type);
	}
}


void ShotLayer::changeShottingBubbleByRandom()
{

	int typeCount = g_gameLayer->getCurrentBarrierData()->bubbleTypeCount;
	typeCount = std::min(typeCount, PAOPAO_TOTAL_TYPE_COUNT);
	Bubble *pRet = NULL;  
	PAOPAO_TYPE type = static_cast<PAOPAO_TYPE>(rand() % typeCount + 1);  
	if(_currentBubble){
		_currentBubble->changeType(type);
	}

}

//√∑√ø√∑‚àö‚Ä¶Àô‚àö¬∏
void ShotLayer::resetHeart()
{
	_shotNullCount = 0;
	_resetHeartRound = 0;
	g_scoreLayer->setProgressLeftPaoPao(PROGRESS_PAOPAO_NUMBER, _resetHeartRound);
}

void ShotLayer::resetHeartKeepResetRound()
{
	_shotNullCount = 0;
	g_scoreLayer->setProgressLeftPaoPao(_shotNullCount, _resetHeartRound);
}


bool ShotLayer::hasSpecialBubbleOccur(PAOPAO_TYPE type)
{
	return _specialBubbleShowMap.find(type) != _specialBubbleShowMap.end();
}

void ShotLayer::deleteSpecialBubbleOccur(PAOPAO_TYPE type)
{
	_specialBubbleShowMap.erase(static_cast<int>(type));
}

void ShotLayer::setHeartFreeze(int count)
{
	this->_heartFreeze = count;
}


void ShotLayer::setElectricBubble(int count)
{
	this->_electricBubble = count;
	if (_electricBubble > 0)
	{
		if(_currentBubble != NULL && _currentBubble->getParent() == this){
			_currentBubble->setPosition(this->_shotPaoPaoPos.x, this->_shotPaoPaoPos.y );
			changeShottingBubble(PAOPAO_BOMB);
			_electricBubble--;
		}
	}
}
void ShotLayer::setIsShotting(bool isShotting)
{
	_enableShotting=isShotting;
}


cocos2d::Point ShotLayer::getGunPosition()
{
	return this->_gun->getPosition();
}

void ShotLayer::showAGrass(int index)
{
	MyRect gameFrame = g_gameLayer->getGameFrame();
cocos2d::Point  ptList[5] = 
	{
		cocos2d::Point(gameFrame.x+gameFrame.width/2-100, gameFrame.y-6), cocos2d::Point( gameFrame.x+12, gameFrame.y-6), cocos2d::Point(gameFrame.x+gameFrame.width-113, gameFrame.y+90),
		cocos2d::Point(gameFrame.width+gameFrame.x-166, gameFrame.height/2+gameFrame.y+120),
		cocos2d::Point(gameFrame.x-9, gameFrame.y+gameFrame.height/2+180)
	};

	cocos2d::Node *grass = this->getChildByTag(GRASS_TAG_BASE + index);
	if(grass == NULL)
	{
		grass = GrassSprite::initWithType(index);
		grass->setAnchorPoint(cocos2d::Point(0,0));
		this->addChild(grass, 0, GRASS_TAG_BASE + index);
		grass->setPosition(ptList[index-1]);
    
    
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    
    cocos2d::Size oldSz = grass->getContentSize();
    cocos2d::Point oldPo = grass->getPosition();
    if(visibleSize.width> UIWIDTH)
    {
        
        GrassBubble *_bubble=dynamic_cast<GrassSprite *>(grass)->getBubble();

        _bubble->setScale(_bubble->getScaleX()/grass->getScaleX(),_bubble->getScaleY()/grass->getScaleY());
        cocos2d::Size  newSz2 =_bubble->getContentSize();
        cocos2d::Point oldPo2 = _bubble->getPosition();
        
        cocos2d::Point  diff2 = cocos2d::Point(oldPo2.x,oldPo2.y+(newSz2.height-30)/2-_nextDiff);
        _bubble->setPosition(diff2);
    }
    }
    else{
            dynamic_cast<GrassSprite *>(grass)->addABubble();
        }
	
	_grassShowCount = 3;

	_grassList.push_back(dynamic_cast<GrassSprite *>(grass) );
}

void ShotLayer::hideAGrass(int index)
{
	cocos2d::Node *grass = this->getChildByTag(GRASS_TAG_BASE + index);
	if(grass != NULL)
		grass->setVisible(false);
}

bool ShotLayer::isCollision(cocos2d::Point pos1, cocos2d::Point pos2, float r1, float r2)
{
	float dist = sqrt(pow(pos1.x-pos2.x, 2) + pow(pos1.y-pos2.y, 2));
	return (dist < r1 + r2);
}

int ShotLayer::genGrass()
{
	if(_currentBubble&&_currentBubble->getColorType() == PAOPAO_PASS)
	{
		return -1;
	}
	for(size_t i=0; i<_grassList.size(); i++)
	{
		GrassSprite* grass = _grassList.at(i);
		if(grass->getBubble())
		{
			return -1;
		}
	}

	int prob[10] = {0, 0, 0, 0, 5, 5, 10, 15, 20, 30};

	int probAdd = g_talentMgr->getCurrentEffectValue(TT_GrassBubbleSpawn);

	if(rand() % 100	< prob[_shotCount>=10?9:_shotCount% 10] + probAdd / 100)
	{
		int grassSize = g_talentMgr->getCurrentEffectValue(TT_GrassBubble);
		
		_shotCount = 0;

		return rand() % grassSize + 1;
	}

	return -1;
}

void ShotLayer::checkGrassDie()
{
	if(_grassShowCount > 0)
	{
		_grassShowCount--;
		for(size_t i=0; i<_grassList.size(); i++)
		{
			GrassSprite* grass = _grassList.at(i);
			GrassBubble* bubble = grass->getBubble();
			if(grass->isVisible()&&bubble)
			{
				bubble->runAction(Sequence::create(
					ScaleTo::create(0.5f,bubble->getScale()*1.1),
					CallFuncN::create(this,callfuncN_selector(ShotLayer::changeBubblePosition)),
					NULL));
			}
			
		}
	}

	if(_grassShowCount == 0)
	{
		_grassShowCount = -1;

		for(size_t i=0; i<_grassList.size(); i++)
		{
			GrassSprite* grass = _grassList.at(i);
			grass->removeABubble();
		}
	}
}
void ShotLayer::changeBubblePosition(Node *node)
{
	GrassBubble*bubble=dynamic_cast<GrassBubble*>(node);
	cocos2d::Size  oldSz = bubble->getContentSize()/1.1;
	cocos2d::Size  newSz =bubble->getContentSize();
	cocos2d::Point oldPo = bubble->getPosition();

	cocos2d::Point  diff = cocos2d::Point(oldPo.x,oldPo.y);
    bubble->setPosition(diff);
}
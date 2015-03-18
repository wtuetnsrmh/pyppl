
#include "GameLayer.h"
#include "Global.h"
#include "Bubble.h"
#include "ShotLayer.h"
#include "Utility.h"
#include "LoseUI.h"
#include "Shake.h"
#include "FileUtil.h"
#include "BarrierData.h"
#include "NextUI.h"
#include "SkillSprite.h"
#include "TalentMgr.h"
#include "TalentData.h"
#include "PowerDef.h"
#include "FireLayer.h"
#include "HelpLayer.h"
#include "MaskLayer.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h> 
GameLayer::GameLayer(void)
{
	 srand((unsigned)time(NULL)); 
	_rorateDegeree = 0;
	_bumbingTime = 0;
	_enemyBossHp = 0;
	_oldEnemyBossHp = 0;
	_currentBarrierData = NULL;
	resetValue();

	_statData.reset();
	_totalCoinInRound = 0;
	//_winCoinInOneRound = 0;

	_power = 0;

	_enableTalentUpdate = false;

	_EarthquakeCount=1;
	_EnergyInjectionCount=1;
	_LaserCount=1;
	_TimelaspeCount=1;
}


GameLayer::~GameLayer(void)
{

}

void GameLayer::resetValue()
{
	//_seqClearCount = 0;
	//_seqSameClearCount = 0;
	//_lastClearColor = PAOPAO_TYPE_UNKNOW;

	_plagueStep = 0;
	_inkStep = 0;

	_normalBubbleActionTime = 0;

	//_power = 0;

	_bubbleOperationList.clear();

}

// on "init" you need to initialize your instance  
bool GameLayer::init()  
{  
	//////////////////////////////  
	// 1. super init first  
	if ( !Layer::init() )  
	{  
		return false;  
	}  

	calOffsetXY();

	FileUtil::readBarrierProgress(_currrentBarrierProgress);
	if(_currrentBarrierProgress-1 < FileUtil::barrierList.size())
		_currentBarrierData = FileUtil::barrierList.at(_currrentBarrierProgress-1);
	else
	{
		_currentBarrierData = FileUtil::barrierList.at(0);
	}


	return true;  
} 


void GameLayer::gameStart()
{
	_enableTalentUpdate = false;
	this->initTheBoard();
	this->initBarrier(_currrentBarrierProgress,false);
	this->scheduleUpdate();
	this->setGameState(G_START);
	//g_goodsData->gem+=10;
    
    FileUtil::readGuide();
    if(g_guide==0)
    {
        if (g_miscLayer->getChildByTag(HELPUI)==NULL)
        {
            auto helpUI = HelpLayer::create();
            g_miscLayer->addChild(helpUI);
            helpUI->setTag(HELPUI);
        }
    }
	_rorateDegeree = std::max(80, rand() % 180);
}
void GameLayer::gameOver()
{
	if (this->getGameState() == G_OVER)
	{
		return;
	}
    _statData.coin =  _totalCoinInRound;
	_statData.baseScore = g_scoreLayer->getScore();
	_statData.stage = _currrentBarrierProgress+g_talentMgr->getCurrentEffectValue(TT_ScoreMulti);
    
    _statData.score = g_scoreLayer->getScore() * _statData.stage;
	
	FileUtil::writeNewRecord(_statData.score);
	this->setGameState(G_OVER);
	g_shotLayer->gameOver();
    this->unscheduleUpdate();
	if(g_miscLayer->getChildByTag(GOODSHOP)==NULL)
    {
		g_goodsShopLayer = GoodsShopLayer::create();
		g_miscLayer->addChild(g_goodsShopLayer);
		g_goodsShopLayer->setTag(GOODSHOP);
		g_goodsShopLayer->initItem(true);
     }
}
void GameLayer::gameRealOver()
{
	_statData.reset();
	_totalCoinInRound = 0;
	//_winCoinInOneRound = 0;
    this->unscheduleUpdate();
	
	_enableTalentUpdate = true;
}
void GameLayer::gameWin()
{
	//ªÒµ√Ω±“

	if (this->getGameState() == G_OVER)
	{
		return;
	}
	this->setGameState(G_OVER);
	g_shotLayer->gameOver();
	this->unscheduleUpdate();
	

	this->runAction(Sequence::create(
		CallFuncN::create(this, callfuncN_selector(GameLayer::bossDie)),
        CallFuncN::create(this, callfuncN_selector(GameLayer::dropAllLeftBubbleWhenWin)),
		DelayTime::create(0.7f),
		CallFuncN::create(this, callfuncN_selector(GameLayer::removeBoss)),
		DelayTime::create(1.0f),
		CallFuncN::create(this, callfuncN_selector(GameLayer::addNextUI)),
		NULL));
	
	FileUtil::writeNewRecord(g_scoreLayer->getScore());
	_enableTalentUpdate = true;
    
    
}
void GameLayer::bossDie(Node* node)
{
    if(g_miscLayer->getChildByTag(GOODSHOP)!=NULL)
    {
        g_miscLayer->removeChild(g_goodsShopLayer);
    }
	g_miscLayer->playMp3("clear1.mp3");
	centerPp->playEffectOnce(B_Bumb_Broken);
}
void GameLayer::removeBoss(Node* node)
{
    if(g_miscLayer->getChildByTag(GOODSHOP)!=NULL)
    {
        g_miscLayer->removeChild(g_goodsShopLayer);
    }
	_board[centerPp->getRow()][centerPp->getCol()] = NULL;
	this->removeChild(centerPp);
}
void GameLayer::addNextUI(Node* node)
{
    if(g_miscLayer->getChildByTag(GOODSHOP)!=NULL)
    {
        g_miscLayer->removeChild(g_goodsShopLayer);
    }
    if(g_miscLayer->getChildByTag(LOSEUI)!=NULL)
    {
        g_miscLayer->removeChild(g_miscLayer->getChildByTag(LOSEUI));
    }
	if (g_miscLayer->getChildByTag(NEXTUI)==NULL)
	{
		auto nextUI = NextUI::create();
		g_miscLayer->addChild(nextUI);
		nextUI->setTag(NEXTUI);
	}
}
void GameLayer::nextBarrier()
{
	_currrentBarrierProgress++;
	//FileUtil::writeBarrierProgress(1);
	g_scoreLayer->setStage(_currrentBarrierProgress);
	gameRestart();
}

void GameLayer::dropAllLeftBubbleWhenWin(Node *node)
{
    if(g_miscLayer->getChildByTag(GOODSHOP)!=NULL)
    {
        g_miscLayer->removeChild(g_goodsShopLayer);
    }
	ROWCOL_LIST list;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL&&_board[i][j]->getColorType()!=PAOPAO_TYPE_CENTER)
				list.push_back(RowCol(_board[i][j]->getRow(), _board[i][j]->getCol()));
		}
	}
	this->dropBubble(list);
}


void GameLayer::gameRestart()
{
	if(this->getGameState() == G_OVER){
		//_seqClearCount = 0;
		//_seqSameClearCount = 0;
		this->resetValue();
		this->removeAllChildren();
		gameStart();

		g_shotLayer->gameRestart();
		g_scoreLayer->resetBossHpShow();
	}
}
void GameLayer::gameRestartFormOne()
{
        FileUtil::readNewRecord();
		this->removeAllChildren();
		//_seqClearCount = 0;
		//_seqSameClearCount = 0;
		this->resetValue();
    
    
        _currentBarrierData = FileUtil::barrierList.at(0);
    
		_currrentBarrierProgress=1;
        g_shotLayer->resetCurrentShottingType2UnKnow();
		g_scoreLayer->setStage(_currrentBarrierProgress);


//		this->initTheBoard();
//		this->initBarrier(1,false);
//		this->scheduleUpdate();
//		this->setGameState(G_START);
		
		g_shotLayer->gameRestart();
		

		g_scoreLayer->reset();
		g_scoreLayer->clearScore();

        g_powerLayer->reset();
		g_powerupsUI = PowerUpsUI::create();
	    g_miscLayer->addChild(g_powerupsUI);
        g_shotLayer->resetHeart();
		
		_enableTalentUpdate = false;
		_EarthquakeCount=1;
		_EnergyInjectionCount=1;
		_LaserCount=1;
		_TimelaspeCount=1;
		
}

void GameLayer::saveGold2File(int gold)
{
	//g_scoreLayer->updateGold(gold);
	FileUtil::writeGameGoods();
}

void GameLayer::addGold(int addGold)
{
    this->saveGold2File(g_goodsData->gold+addGold);
}

Bubble * GameLayer::randomBubble()
{  
	Bubble *pRet = NULL;  
	int typeCount = PAOPAO_TOTAL_TYPE_COUNT;
	if(_currentBarrierData!=NULL)
		typeCount = std::min(_currentBarrierData->bubbleTypeCount, PAOPAO_TOTAL_TYPE_COUNT);
	
	PAOPAO_TYPE type = static_cast<PAOPAO_TYPE>(rand() % typeCount + 1);  
	pRet = Bubble::initWithType(type);  
	return pRet;  
} 

void GameLayer::calOffsetXY()
{
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
cocos2d::Size  visibleSize = Director::getInstance()->getWinSize();  
	
	int width = visibleSize.width - origin.x;
	int height = visibleSize.height - origin.y;

	
	_centerPoint = cocos2d::Point(origin.x + visibleSize.width/2 , origin.y+visibleSize.height/2);
	
	_paopaoOffset.x = 0;
	_paopaoOffset.y = 0;
	
	PAOPAO_SIZE = std::min(width, height) / MAX_ROWS;
	if(PAOPAO_SIZE%2==1)
		PAOPAO_SIZE -= 1;
	PIC_RADIO =  (float) PAOPAO_SIZE / (float)PAOPAO_ORIGINAL_SIZE;

	int logicWidth = PAOPAO_SIZE * MAX_COLS;
	int logicHeight = PAOPAO_SIZE * MAX_ROWS;

	int cRow = floor(MAX_ROWS/2.0);
	int cCol = floor(MAX_COLS/2.0);
	int BUBBLE_RADIUS = PAOPAO_SIZE/2;
	
	_gameFrame.x =  _centerPoint.x - logicWidth/2;
	_gameFrame.y =  _centerPoint.y - logicHeight/2;
	_gameFrame.width = logicWidth;//this->_totalCol * PAOPAO_SIZE;
	_gameFrame.height = logicHeight;//this->_totalRow * PAOPAO_SIZE;

	_paopaoOffset = cocos2d::Point( _centerPoint.x - logicWidth/2 ,  _centerPoint.y - logicHeight/2 );

	//÷––ƒ≈›≈›µƒ µº ◊¯±Í
	//int cposX = cCol * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (cRow % 2) * BUBBLE_RADIUS + _gameFrame.x;
	//int cposY = ( cRow * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS ) + _gameFrame.y;
	cocos2d::Point  centerPpPos = getPointByRowAndCol(cRow, cCol);

	_centerPointOffset.x = _centerPoint.x - centerPpPos.x;
	_centerPointOffset.y = _centerPoint.y - centerPpPos.y;
	
}

void GameLayer::update(float fDelta)
{
	
	float easing = 0.03f; // æ˘ºı°¢º”µƒª∫¥Êœµ ˝  
	float dx = abs(_rorateDegeree);
	float addX;  
	addX = easing * dx;  

	if(_rorateDegeree > 0){
		this->setRotation( this->getRotation() + addX);
		_rorateDegeree -= addX;
		checkGameOver();
		processBubbleAfterRatation();
	}
	
	if(_rorateDegeree < 0)
	{
		this->setRotation( this->getRotation() - addX);
		_rorateDegeree += addX;
		checkGameOver();
		processBubbleAfterRatation();
	}

	if(abs(_rorateDegeree) < 2 && _rorateDegeree!=0){
		_rorateDegeree = 0;
		checkBubbleWarning();
		processBubbleAfterRatation();
	}
	
	if(g_shotLayer)
		g_shotLayer->updateLoop(fDelta);

	checkBumbBubble();

	animationNormalBubble(fDelta);
}

void GameLayer::processBubbleAfterRatation()
{
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j]!=NULL && _board[i][j]->getColorType() == PAOPAO_INK){
				_board[i][j]->setRotation(-this->getRotation());
			}
		}  
	} 
}

bool GameLayer::initTheBoard()  
{  
	//≥ı ºªØŒ™ø’  
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			_board[i][j] = NULL;  
		}  
	} 

	centerPp =  Bubble::initWithType( static_cast<PAOPAO_TYPE>(PAOPAO_TYPE_CENTER) );
	this->addChild(centerPp, 2);
	int cRow = floor(MAX_ROWS/2.0);
	int cCol = floor(MAX_COLS/2.0);
cocos2d::Point  centerPpPos = getPointByRowAndCol(cRow, cCol);
	centerPp->setPosition( centerPpPos.x  , centerPpPos.y);
	centerPp->setRowCol(cRow, cCol);
	
	_board[cRow][cCol] = centerPp;
    
	//centerPp->playNormalAction(true);
	//centerPp->playEffectForEver2(B_Blood, 0.2f);
	//centerPp->startTingle();
	return true;

}

//Õ®π˝––¡–ªÒ»°Œª÷√
cocos2d::Point  GameLayer::getPointByRowAndCol(int row, int col)  
{ 
	int BUBBLE_RADIUS = PAOPAO_SIZE/2;

	int diffx = _paopaoOffset.x + _centerPointOffset.x;
	int diffy = _paopaoOffset.y + _centerPointOffset.y;

	int posX = col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS  + (row % 2) * BUBBLE_RADIUS + diffx ;
	int posY = ( row * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS ) + diffy ;
	return cocos2d::Point(posX, posY);
}  

RowCol GameLayer::getRowColByPos( int nPosX, int nPosY)
{
	int nRow, nCol;
	//–Ë“™Àƒ…·ŒÂ»Î
	int BUBBLE_RADIUS = PAOPAO_SIZE/2;
	int diffx = _paopaoOffset.x + _centerPointOffset.x;
	int diffy = _paopaoOffset.y + _centerPointOffset.y;
	nRow = ( nPosY - diffy -BUBBLE_RADIUS )/( 2 *BUBBLE_RADIUS *sin ( PI/3 ) )  +0.5; 
	nCol = ( nPosX - diffx - ( nRow % 2 ) * BUBBLE_RADIUS - BUBBLE_RADIUS ) * 1.0 /(  2 *BUBBLE_RADIUS ) + 0.5; 

	return RowCol( nRow, nCol );
}

//≥ı ºªØπÿø®
void GameLayer::initBarrier(int barrier,bool isRestTime)
{
	if(barrier-1 < FileUtil::barrierList.size())
		_currentBarrierData = FileUtil::barrierList.at(barrier-1);
	else
	{
		_currentBarrierData = FileUtil::barrierList.at(0);
	}

    if(isRestTime==false)
    {
        _enemyBossHp = _currentBarrierData->bossHp;
        _oldEnemyBossHp = _currentBarrierData->bossHp;
    }

	int cRow = floor(MAX_ROWS/2.0);
	int cCol = floor(MAX_COLS/2.0);

	int layerCount = _currentBarrierData->bubbleLayer;

	std::vector<RowCol> rcList ;

	std::vector<RowCol> tempList ;
	tempList.push_back(RowCol(cRow, cCol));
	while (layerCount>0)
	{
		std::vector<RowCol> tempList2;
		for(std::vector<RowCol>::const_iterator iterTemp = tempList.begin();
			iterTemp != tempList.end();
			iterTemp++)
		{
			std::vector<RowCol> vec;
			getAround(iterTemp->m_nRow, iterTemp->m_nCol, vec);

			for (std::vector<RowCol>::const_iterator iterBubble = vec.begin();
				iterBubble != vec.end();
				iterBubble++)
			{
				int row = iterBubble->m_nRow;
				int col = iterBubble->m_nCol;
				if(false == isInRowColList(rcList, row, col) && !(row==cRow&&col==cCol)){
					tempList2.push_back(RowCol(row, col));
					rcList.push_back(RowCol(row, col));
				}
			}
		}
		
		tempList = tempList2;
		
		layerCount--;
	}

	
	if (barrier == 1)
	{
		firstBarrierSpecialList();
	}
	else{
		for (std::vector<RowCol>::const_iterator iterBubble = rcList.begin();
			iterBubble != rcList.end();
			iterBubble++)
		{
			int row = iterBubble->m_nRow;
			int col = iterBubble->m_nCol;
			if(_board[row][col] == NULL)
			{
				Bubble * obj = randomBubble();  
				obj->setRowCol(row, col);  
				obj->setPosition(getPointByRowAndCol(row, col));  
				this->addChild(obj);  
				//obj->setRowCol(row, col);
				_board[row][col] = obj;
			}
		}
	}
    g_scoreLayer->setBossHp(_enemyBossHp,_oldEnemyBossHp);

	//this->randomAngle();

}

void GameLayer::firstBarrierSpecialList()
{
	int cRow = floor(MAX_ROWS/2);
	int cCol = floor(MAX_COLS/2);

	

	if(cRow % 2==0){
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow,cCol-2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol-3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow,cCol+2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol+3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+1,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+1,cCol+2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol+3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow-1,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-1,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-1,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-1,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-1,cCol+2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-1,cCol+3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+2,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+2,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+2,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+2,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+2,cCol+2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow-2,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-2,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-2,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-2,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-2,cCol+2);


		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+3,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+3,cCol-1);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+3,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+3,cCol+2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow-3,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-3,cCol-1);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-3,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-3,cCol+2);
		
	}
	else{
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow,cCol-2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol-3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow,cCol+2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow,cCol+3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+1,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+1,cCol+2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+1,cCol+3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow-1,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-1,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-1,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-1,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-1,cCol+2);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-1,cCol+3);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+2,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+2,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+2,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+2,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+2,cCol+2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow-2,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-2,cCol-1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-2,cCol-2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-2,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow-2,cCol+2);


		  this->setFirstBarrierBubble(PAOPAO_TYPE2,cRow+3,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+3,cCol-1);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow+3,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow+3,cCol+2);

		  this->setFirstBarrierBubble(PAOPAO_TYPE3,cRow-3,cCol);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-3,cCol-1);

		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-3,cCol+1);
		  this->setFirstBarrierBubble(PAOPAO_TYPE1,cRow-3,cCol+2);




	}
}
void GameLayer::setFirstBarrierBubble(PAOPAO_TYPE type,int cRow,int cCol)
{
	int row = 0, col = 0;
	Bubble *bubble = Bubble::initWithType(type); 
	row = cRow;
	col = cCol;
	bubble->setRowCol(row, col);
		
	bubble->setPosition(getPointByRowAndCol(row, col));  
	this->addChild(bubble);  
	_board[row][col] = bubble;
}
void GameLayer::setAttachAttribute(Bubble *pBubble, BubbleEffect effect)
{
	pBubble->setAttachAttribute(effect);
}

bool GameLayer::isInRowColList(std::vector<RowCol> &vec, int row, int col)
{
	for (std::vector<RowCol>::const_iterator iterBubble = vec.begin();
		iterBubble != vec.end();
		iterBubble++)
	{
		int nrow = iterBubble->m_nRow;
		int ncol = iterBubble->m_nCol;
		if(nrow == row && ncol == col)
			return true;
	}
	return false;
}

//≈–∂œ «∑Ò”–’®µØ“™±¨’®
void GameLayer::checkBumbBubble()
{
	if(_bumbingTime==0)
		return;
	if(_bumbingTime >= time(nullptr))
		return;
	//g_miscLayer->playWav("sd.wav");
	int findBumb = 0;
	std::vector<RowCol> tempList;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;
			if(_board[i][j]->getColorType() == PAOPAO_BOMB && findBumb==0){
				getAround(i, j, tempList);
				tempList.push_back(RowCol(i, j));
				findBumb++;		
			}
			else if(_board[i][j]->getColorType() == PAOPAO_BOMB){
				findBumb++;	
			}
		}
		
	}
	
	ROWCOL_LIST clearList;
	for (std::vector<RowCol>::const_iterator iterBubble = tempList.begin();
		iterBubble != tempList.end();
		iterBubble++)
	{
		int nRow = iterBubble->m_nRow;
		int nCol = iterBubble->m_nCol;
		if(_board[nRow][nCol] != NULL && _board[nRow][nCol]->getColorType() != PAOPAO_TYPE_CENTER){
			clearList.push_back(RowCol(nRow, nCol));
		}
	}
	this->clearBubble(clearList);

	if(findBumb <= 1)
		_bumbingTime = 0;
	else
		_bumbingTime = time(NULL) + 1;

	this->addBubbleOperation(PAOPAO_BOMB);

}

bool GameLayer::stickAGrid(Bubble *paopao, ShotLayer *sl)
{
	//if(this->getGameState() == G_OVER)
		//return false;

	float angle = this->getRotation(); //—°‘ÒµƒΩ«∂»£¨≤ª◊™ «0
	cocos2d::Point  paopaoPos = paopao->getPosition();
	cocos2d::Point  truePoint = convertA1ToA2(_centerPoint, paopaoPos, angle);

	int BUBBLE_RADIUS = PAOPAO_SIZE/2;

	int finalSpeedX = paopao->getSpeedX();
	int finalSpeedY = paopao->getSpeedY();
	int finalX = 0;
	int finalY = 0;


	if(paopao->getColorType() == PAOPAO_PASS)
		return false;

	//bool findBubble = false;
	RowCol rc = this->checkCollision(paopao, angle);
	if((rc.m_nRow==-1&&rc.m_nCol==-1)){
		return false;
	}

	if(isValidPos(rc.m_nRow, rc.m_nCol) == false){ 
		paopao->retain();

		sl->removeChild(paopao, true);
		this->addChild(paopao);
		paopao->setParent(this);
		paopao->setRowCol(rc.m_nRow, rc.m_nCol);
		cocos2d::Point  adjustPos = this->getPointByRowAndCol(rc.m_nRow, rc.m_nCol);
		paopao->setPosition(adjustPos);
		paopao->setBubbleState(Bubble_S_Dirty);
		paopao->release();
		sl->stopShot(false, false);
		
	
		gameOver();
		
		return false;
	}
	
	finalX = paopao->getPosition().x;
	finalY = paopao->getPosition().y;
	cocos2d::Point  adjustPos = this->getPointByRowAndCol(rc.m_nRow, rc.m_nCol);//adjustPaoPaoPosition(truePoint);
	//paopao->setParent(this);
	paopao->retain();

	sl->removeChild(paopao, false);
	this->addChild(paopao);
	paopao->setParent(this);
	paopao->setRowCol(rc.m_nRow, rc.m_nCol);
	paopao->setPosition(adjustPos);
	paopao->setBubbleState(Bubble_S_Dirty);
	paopao->release();

	//_listBubble.push_back(paopao);

	_board[rc.m_nRow][rc.m_nCol] = paopao;

	killFinalEnemy(paopao);


	if(paopao->getColorType() == PAOPAO_BOMB){
		_bumbingTime = time(NULL) + 1;
		paopao->playEffectForEver2(B_Bumb_Ready, 0.125f);
	}

	if(finalSpeedX!=0 && finalSpeedY!=0){

		float temp = area(_centerPoint.x,_centerPoint.y,finalX,finalY,finalX+finalSpeedX, finalY+finalSpeedY);

		float angelSpeed = getAngleSpeed(3000, paopao->getAngleForce() );
		if(temp > 0) {
			this->_rorateDegeree = -angelSpeed; //-120 * abs(finalX - _centerPoint.x) /( _gameFrame.width / 2);
			
		}
		else{ 
			this->_rorateDegeree = angelSpeed; //120 * abs(finalX - _centerPoint.x) / (_gameFrame.width / 2 );
		}
	}
	if(abs(this->_rorateDegeree) < 2)
		checkBubbleWarning();

	PAOPAO_TYPE operation;
	ROWCOL_LIST clearBubbleList = findClearBubble(paopao);
	if(clearBubbleList.size() < REMOVE_COUNT){
		//º∑—π∂Øª≠
		std::vector<RowCol> tempList;
		tempList.push_back(rc);
		this->getAround(rc.m_nRow, rc.m_nCol, tempList);
		removeCenterInList(tempList);
		this->crowdEffect(tempList, paopao->getSpeedX(), paopao->getSpeedY(), 6);

		std::vector<RowCol> tempList2;
		findAGroundInList(tempList, tempList2);
		removeCenterInList(tempList2);
		this->crowdEffect(tempList2, paopao->getSpeedX(), paopao->getSpeedY(), 4);

		restorBubbleTypeByPlague(paopao->getRow(), paopao->getCol());

		operation = PAOPAO_TYPE_UNKNOW;
	}
	else{
		operation = paopao->getColorType();

		clearBubble(clearBubbleList);

		for(ROWCOL_LIST::iterator iter = clearBubbleList.begin(); iter!=clearBubbleList.end(); ++iter){
			restorBubbleTypeByPlague(iter->m_nRow, iter->m_nCol);
		}
		

		//checkSpecialReward(); //Ω±¿¯
		
	}
	
	
	this->checkGameOver();

	if(paopao->getColorType() == PAOPAO_BOMB){
		sl->stopShot(true, false);
	}
	else
		sl->stopShot(true, clearBubbleList.size()<REMOVE_COUNT);

	_plagueStep++;
	_inkStep++;

	//if skillBox-HasRemainSkill
	//use skill, box remove skill
	bool storeSkill = false;
	if (g_skillBoxLayer->hasSkill())
	{
		g_shotLayer->changeShottingBubble(static_cast<PAOPAO_TYPE>(g_skillBoxLayer->useSkill())); 
		storeSkill = true;
	}
	
	/*std::list<int> addSkillList;

	if(paopao->getCollisionCount() >=2 && clearBubbleList.size() >= REMOVE_COUNT){

		if(g_skillBoxLayer->hasSkill() || storeSkill==true){
			//g_skillBoxLayer->addSkill(PAOPAO_PASS);
			addSkillList.push_back(PAOPAO_PASS);
			storeSkill = true;
		}
		else{
			g_shotLayer->changeShottingBubble(PAOPAO_PASS);
		}
	}*/

	if(clearBubbleList.size() >= REMOVE_COUNT)
		this->addBubbleOperation(paopao->getColorType());
	else
		this->addBubbleOperation(PAOPAO_TYPE_UNKNOW);

	//g_skillBoxLayer->addSkill(addSkillList);

	this->specialBubbleProcess();

	return true;
}

void GameLayer::restorBubbleTypeByPlague(int row, int col)
{
	std::vector<RowCol> vec;
	this->getAround(row, col, vec);

	std::vector<RowCol>::iterator iter;
	for(iter=vec.begin(); iter!=vec.end(); ++iter)
	{
		int iRow = iter->m_nRow;
		int iCol = iter->m_nCol;
		if (_board[iRow][iCol]!=NULL && _board[iRow][iCol]->getColorType()==PAOPAO_PLAGUE)
		{
			if(_board[iRow][iCol]->getOldColorType()>=PAOPAO_TYPE1 && _board[iRow][iCol]->getOldColorType()<=PAOPAO_TYPE6)
			{
				_board[iRow][iCol]->changeType( static_cast<PAOPAO_TYPE>(_board[iRow][iCol]->getOldColorType()) );
				this->addPower(PAOPAO_PLAGUE,0,_board[iRow][iCol]->getPosition());
			}
		}
	}
}


bool GameLayer::stickAGrid2(Bubble *paopao, ShotLayer *sl)
{
	float angle = this->getRotation(); //—°‘ÒµƒΩ«∂»£¨≤ª◊™ «0
	cocos2d::Point  paopaoPos = paopao->getPosition();

	RowCol rc = this->checkCollision(paopao, angle);

	if((rc.m_nRow==-1&&rc.m_nCol==-1)){
		return false;
	}


	if(isValidPos(rc.m_nRow, rc.m_nCol) == false ){
		sl->removeChild(paopao, true);
		sl->stopRandomBubble(paopao);
		return false;
	}
	cocos2d::Point  adjustPos = this->getPointByRowAndCol(rc.m_nRow, rc.m_nCol);

	sl->stopRandomBubble(paopao);

	paopao->retain();

	sl->removeChild(paopao, false);
	this->addChild(paopao);
	paopao->setParent(this);
	paopao->setRowCol(rc.m_nRow, rc.m_nCol);

	paopao->setPosition(adjustPos);
	
	paopao->release();

	_board[rc.m_nRow][rc.m_nCol] = paopao;

	return true;
}

void GameLayer::checkAttachEffect(Bubble *obj)
{
	if(obj->getAttachAttribute() == B_Attribute_AssistLine){
		g_shotLayer->setLienShow(true);
	}

	if(obj->getAttachAttribute() == B_Attribute_EarthQuake){
		this->earthquake();
	}
	if(obj->getAttachAttribute() == B_Attribute_ResetHeart){
		g_shotLayer->resetHeart();
	}

	if(obj->getAttachAttribute() == B_Attribute_ChangeType){
		changeMinTypeInBoard();
	}

}

PAOPAO_TYPE GameLayer::changeMinTypeInBoard()
{
	std::map<PAOPAO_TYPE, int> countMap;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;

			PAOPAO_TYPE type = _board[i][j]->getColorType();

			std::map<PAOPAO_TYPE, int>::iterator iter = countMap.find(type);
			if(iter == countMap.end()){
				countMap[type] = 1;
			}
			else{
				countMap[type] = iter->second + 1;
			}
		}
	}

	std::map<PAOPAO_TYPE, int>::iterator iter;
	int minCount = 999;
	PAOPAO_TYPE minType = PAOPAO_TYPE_UNKNOW;
	for (iter = countMap.begin(); iter != countMap.end(); iter++)
	{
		if(iter->second < minCount){
			minCount = iter->second;
			minType = iter->first;
		}
	}

	if(minType != PAOPAO_TYPE_UNKNOW){
		for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
		{  
			for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
			{  
				if(_board[i][j] == NULL)
					continue;

				_board[i][j]->changeType(minType);
			}
		}
	}
	return minType;
}

//œ˚≥˝bubbleList¿ÔÀ˘”–µƒ«Ú
void GameLayer::clearBubble(const ROWCOL_LIST &bubbleList, bool hurtBoss)
{
	int nRow, nCol;
	
	g_scoreLayer->addScore(bubbleList.size() * _currentBarrierData->scoreBase);

	int addGold = 0;
	if( _currentBarrierData->goldTalent == 1){
		addGold = this->getCurrentBarrierGold(bubbleList.size());
		_totalCoinInRound += addGold;
	}
	else{
		addGold = bubbleList.size() * _currentBarrierData->goldBase;
		_totalCoinInRound += addGold;
	}
	g_goodsData->gold += addGold;
	this->saveGold2File(g_goodsData->gold);
   
    g_scoreLayer->updateGold(_totalCoinInRound);
	//_winCoinInOneRound += bubbleList.size() * _currentBarrierData->goldBase;

	std::list<Point> bubblePointList ;

	bool killBossOnce = false;
	bool bombHurt = false;
	bool clearHurt = false;
	int *hurtHp = new int(0);

	for (ROWCOL_LIST::const_iterator iterBubble = bubbleList.begin();
		iterBubble != bubbleList.end();
		iterBubble++)
	{
		nRow = iterBubble->m_nRow;
		nCol = iterBubble->m_nCol;
		Bubble *obj = _board[nRow][nCol];
		PAOPAO_TYPE bubbleType = obj->getColorType();
		if (obj)
		{

			checkAttachEffect(obj);
			cocos2d::Point  pt = obj->convertToWorldSpace(obj->getAnchorPoint());//Point pt = this->convertToWorldSpace(obj->getPosition());
			bubblePointList.push_back(pt);
			_board[nRow][nCol] = NULL;

			obj->retain();

			this->removeChild(obj);
			g_miscLayer->addChild(obj);
			obj->setPosition(pt);

			obj->release();

			g_miscLayer->clearBubbleAction(obj,hurtBoss);

			if( true == isKillFinalEnemy(nRow, nCol))
			{
				killBossOnce = true;
				if (bubbleType == PAOPAO_BOMB && bombHurt==false)
				{
					bombHurt = true;

					*hurtHp += g_talentMgr->getCurrentEffectValue(TT_BossHurt_Electric);
				}
				else if (clearHurt==false)
				{
					clearHurt = true;
					*hurtHp += g_talentMgr->getCurrentEffectValue(TT_BossHurt_Clear);
				}
			}
		}
	
	}
	if(bubbleList.size() >= 1){
		g_miscLayer->playMp3("clear1.mp3");
		this->findNonLinkedBubble();
	}

	if(*hurtHp > 0 && hurtBoss==true)
	{
		this->runAction ( Sequence::create(DelayTime::create(0.1f),
			CCCallFuncND::create(this, callfuncND_selector(GameLayer::hurtEnemyBossByBubbleClear), hurtHp),
			NULL
			));
	}


	g_miscLayer->flyNumber(bubblePointList);

	_statData.bubbleEliminate += bubbleList.size();

}

void GameLayer::hurtEnemyBossByBubbleClear(Node *obj, void *param)
{
	int *hurt = static_cast<int *>(param);
	this->changeEnemyBossHp(*hurt);
	delete hurt;
}

ROWCOL_LIST GameLayer::findNonLinkedBubble()
{

	int visit[MAX_ROWS+BOARD_EXTRA_SIZE][MAX_COLS+BOARD_EXTRA_SIZE]; //∆Â≈Ã ;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			visit[i][j] = 0;  
		}  
	}

	ROWCOL_LIST  queue;
	RowCol center(MAX_ROWS / 2, MAX_COLS / 2);
	queue.push_back(center); 

	visit[MAX_ROWS / 2][MAX_COLS / 2] = 1;
	while(queue.size() > 0){
		RowCol p = queue.back(); 
		queue.pop_back();

		std::vector<RowCol> roundRowCol;
		getAround(p.m_nRow, p.m_nCol, roundRowCol);
		for(size_t i=0; i<roundRowCol.size(); i++)
		{
			RowCol temp = roundRowCol.at(i);

			if(visit[temp.m_nRow][temp.m_nCol] == 0 && _board[temp.m_nRow][temp.m_nCol]!=NULL){
				queue.push_back(temp);
				visit[temp.m_nRow][temp.m_nCol] = 1;
			}
		}

		//queue.pop_front();

	}
	ROWCOL_LIST removeBubbleList;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(visit[i][j] == 0 && _board[i][j] != NULL)
			{
				if(_board[i][j]->getColorType() != PAOPAO_TYPE_CENTER){
					RowCol rc(i,j);
					removeBubbleList.push_back(rc);
				}
			}
		}
	}

	this->dropBubble(removeBubbleList);
	return queue;
}

//≤ªœ‡¡¨µƒµÙ¬‰–ßπ˚
void GameLayer::dropBubble(const ROWCOL_LIST & bubbleList)
{
	int nRow, nCol;

	g_scoreLayer->addScore(bubbleList.size() * _currentBarrierData->scoreBase);

	int addGold = 0;
	if(_currentBarrierData->goldTalent == 1){
		addGold = this->getCurrentBarrierGold(bubbleList.size());
		_totalCoinInRound += addGold;
	}
	else{
		addGold =  bubbleList.size() * _currentBarrierData->goldBase;
		_totalCoinInRound += addGold;
	}
	g_goodsData->gold += addGold;
	this->saveGold2File(g_goodsData->gold);
   
	g_scoreLayer->updateGold(_totalCoinInRound);

	//_winCoinInOneRound += bubbleList.size() * _currentBarrierData->goldBase;

	for (ROWCOL_LIST::const_iterator iterBubble = bubbleList.begin();
		iterBubble != bubbleList.end();
		iterBubble++)
	{
		nRow = iterBubble->m_nRow;
		nCol = iterBubble->m_nCol;

		Bubble *obj = _board[nRow][nCol];
		if (obj)
		{
			/*
			if(obj->getColorType() == PAOPAO_STONE){
				g_shotLayer->resetHeartKeepResetRound();
			}
			else if(obj->getColorType() == PAOPAO_GUARD){
				this->changeSmallest2OtherColor();
			}*/

			_board[nRow][nCol] = NULL;
			
			obj->retain();
			cocos2d::Point  pt = obj->convertToWorldSpace(obj->getAnchorPoint());

			this->removeChild(obj);
			g_miscLayer->addChild(obj);
			obj->setPosition(pt);
			
			obj->release();
			
			g_miscLayer->dropBubbleAction(obj);
	
		}	
	}

	if(bubbleList.size() >= 1){
		//checkSpecialReward();
	}

	_statData.bubbleDrop += bubbleList.size();
}


ROWCOL_LIST GameLayer::findClearBubble(Bubble *pReadyBubble)
{
	ROWCOL_LIST clearRowCollist;
	if (pReadyBubble == NULL)
	{
		return clearRowCollist;
	}

	std::map<Bubble *, int> findedMap;
	clearRowCollist = findSameBubble(pReadyBubble, findedMap);
	if (clearRowCollist.size() < REMOVE_COUNT)
	{
		clearRowCollist.clear();
	}

	return clearRowCollist;
}

ROWCOL_LIST GameLayer::findSameBubbleByColor(PAOPAO_TYPE nColor, int row, int col)
{
	ROWCOL_LIST samelist;
	samelist.push_back(RowCol(row, col));
	ROWCOL_LIST::iterator itCur = samelist.begin();
	do 
	{
		std::vector<RowCol> vecRowCol;

		getAround(itCur->m_nRow, itCur->m_nCol, vecRowCol);

		for (size_t i = 0; i < vecRowCol.size(); i++)
		{
			Bubble* pCurBubble = _board[ vecRowCol[i].m_nRow ][ vecRowCol[i].m_nCol ];
			if (pCurBubble !=NULL)
			{
				if( (pCurBubble->getColorType() == nColor || pCurBubble->getColorType()==PAOPAO_COLOR)
						&& (pCurBubble->getColorType()!=PAOPAO_TYPE_CENTER  && pCurBubble->getColorType()!=PAOPAO_INK && pCurBubble->getColorType()!=PAOPAO_STONE
							&&pCurBubble->getColorType()!=PAOPAO_PLAGUE)
					)
				{
					RowCol rc(vecRowCol[i].m_nRow, vecRowCol[i].m_nCol);
					ROWCOL_LIST::iterator itFind = std::find(samelist.begin(), samelist.end(), rc);
					//≤È’“ «∑Ò“—æ≠‘⁄¡–±Ì÷–
					if (itFind == samelist.end())
					{
						samelist.push_back(vecRowCol[i]);
					}
				}
			}
		}
		itCur++;
	} while (itCur != samelist.end());

	return samelist;
}

ROWCOL_LIST GameLayer::findSameBubble(Bubble *pReadyBubble, std::map<Bubble *, int> findedMap)
{
	if(findedMap.find(pReadyBubble) == findedMap.end())
	{
		findedMap[pReadyBubble] = 1;

		ROWCOL_LIST samelist;
		PAOPAO_TYPE nColor= pReadyBubble->getColorType();
		int nRow = pReadyBubble->getRow();
		int nCol = pReadyBubble->getCol();
		samelist.push_back(RowCol(nRow, nCol));

		//ROWCOL_LIST::iterator itCur = samelist.begin();

		//—≠ª∑≤È’“Õ¨—˘µƒ«Ú
		if(pReadyBubble->getColorType() != PAOPAO_COLOR )
		{
			return findSameBubbleByColor(nColor, nRow, nCol);
		}
		else{
			std::vector<RowCol> firstVecRowCol;
			getAround(nRow, nCol, firstVecRowCol);

			for (size_t i = 0; i < firstVecRowCol.size(); i++){
				Bubble* pCurBubble = _board[ firstVecRowCol[i].m_nRow ][ firstVecRowCol[i].m_nCol ];
				if(pCurBubble!=NULL && pCurBubble->getColorType()!=PAOPAO_COLOR)
				{
					nColor = pCurBubble->getColorType();
					ROWCOL_LIST samelist = findSameBubbleByColor(nColor, nRow, nCol);
					if(samelist.size() >= REMOVE_COUNT)
						return samelist;
				}
				else if(pCurBubble!=NULL && pCurBubble->getColorType()==PAOPAO_COLOR){
					return findSameBubble(pCurBubble, findedMap);
				}
			}

		}
		return samelist;
	}

	ROWCOL_LIST samelist;
	return samelist;
}
void GameLayer::getAround( int nRow, int nCol, std::vector<RowCol> & vecPos )
{
	if (!isValidPos(nRow, nCol))
	{
		return;
	}

	//Õ¨“ª––
	if (isValidPos(nRow, nCol - 1))
	{
		vecPos.push_back(RowCol(nRow, nCol - 1));
	}
	if (isValidPos(nRow, nCol + 1))
	{
		vecPos.push_back(RowCol(nRow, nCol + 1));
	}

	//◊Û–±œﬂ
	if (isValidPos(nRow - 1, nCol))
	{
		vecPos.push_back(RowCol(nRow - 1, nCol));
	}
	if (isValidPos(nRow + 1, nCol))
	{
		vecPos.push_back(RowCol(nRow + 1, nCol));
	}

	//»Áπ˚ «µ•––
	int curCol;
	if (nRow % 2 == 0)
		curCol = nCol - 1;
	else
		curCol = nCol + 1;

	//”“–±œﬂ
	if (isValidPos(nRow - 1, curCol))
	{
		vecPos.push_back(RowCol(nRow - 1, curCol));
	}
	if (isValidPos(nRow + 1, curCol))
	{
		vecPos.push_back(RowCol(nRow + 1, curCol));
	}

}

std::vector<RowCol> GameLayer::checkPass(PAOPAO_TYPE color, int oldx, int oldy, int newx, int newy)
{
	std::vector<RowCol> retList;
	int r = PAOPAO_SIZE/2 - 2;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;
			float txi = _board[i][j]->getPosition().x;
			float tyi = _board[i][j]->getPosition().y;
			cocos2d::Point  tempPt = _board[i][j]->convertToWorldSpaceAR(cocos2d::Point(0, 0));
			float xi = tempPt.x;
			float yi = tempPt.y;
			float h = distance(xi,yi,oldx,oldy,newx,newy);
			if(h > 2*r + 1e-4) continue;
			
			if(_board[i][j]->getColorType() <= PAOPAO_TYPE6){
				retList.push_back(RowCol(i,j));
				_board[i][j]->changeType(color);
			}

			if(_board[i][j]->getColorType() == PAOPAO_TYPE_CENTER){
				int hurt = g_talentMgr->getCurrentEffectValue(TT_BossHurt_Bump);
				g_gameLayer->changeEnemyBossHp(hurt);
			}

		}
	}
	
	return retList;
}


RowCol GameLayer::checkCollision(Bubble *pp, float angle)
{
	int r = PAOPAO_SIZE/2 - 2;
	float speedx, speedy;
	speedx = pp->getSpeedX() ;
	speedy = pp->getSpeedY() ;
	float totalspeed = sqrt(speedx*speedx+speedy*speedy);
	float newx = pp->getPosition().x; //oldx + speedx;
	float newy =  pp->getPosition().y; //oldy + speedy;
	float oldx = newx - speedx;
	float oldy = newy - speedy;
	float mindist = 10000000.0;
	int k = -1;
	int truex = -1;
	int truey = -1;
	float ddd = 0;
	Bubble * stickPp;
	
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;
			float txi = _board[i][j]->getPosition().x;
			float tyi = _board[i][j]->getPosition().y;
			cocos2d::Point  tempPt = _board[i][j]->convertToWorldSpaceAR(cocos2d::Point(0, 0));
			float xi = tempPt.x;
			float yi = tempPt.y;
			float h = distance(xi,yi,oldx,oldy,newx,newy);
			if(h > 2*r + 1e-4) continue;
			cocos2d::Point  temp = chuizu(xi,yi,oldx,oldy,newx,newy);
			float xx = temp.x;
			float yy = temp.y;
			float dd = sqrt(4*r*r - h*h);
			float ax = xx - speedx * dd/totalspeed;
			float ay = yy - speedy * dd/totalspeed;
			if((ax-oldx)*(ax - newx) > 0) continue;
			if((ay-oldy)*(ay -newy) > 0) continue;
			ddd = dist(oldx, oldy, ax, ay);
			if(ddd < mindist) {
				mindist = ddd;
				k = i;
				truex = ax;
				truey = ay;
				stickPp = _board[i][j] ;
			}
			
		}
	}
	if(k == -1) {
		// ’‚“ª÷°¿Ô√Ê√ª”–≈ˆ◊≤
	} else {
		//’‚“ª÷°¿Ô√Ê≈ˆ◊≤¡À£¨≈ˆ◊≤ ±∑…––÷–µƒbubbleµƒŒª÷√ « truex,truey
		cocos2d::Point  truePt = convertA1ToA2(_centerPoint, cocos2d::Point(truex, truey), angle);
		
		RowCol rc =  this->getRowColByPos(truePt.x, truePt.y);
		pp->setStickPp(stickPp);
		
		rc = findTrueBlankPos(rc, cocos2d::Point(newx, newy), speedx, speedy, angle);
	
		return rc;
	}
	RowCol rc(-1, -1);
	return rc;
}


RowCol GameLayer::findTrueBlankPos(RowCol rc, cocos2d::Point  currentXY, float speedx, float speedy, float angle)
{
	RowCol ret(MAX_ROWS, MAX_COLS);
	if(isValidPos(rc.m_nRow, rc.m_nCol) == false)
		return rc;
	if(_board[rc.m_nRow][rc.m_nCol] == NULL)
		return rc;
	else{
		//std::vector<RowCol> roundRowCol;
		//this->getAround(rc.m_nRow, rc.m_nCol, roundRowCol);
		//float distance = 1000000.0;
		for(int i=0; i<PAOPAO_SIZE; i++)
		{
			//ªÿÕÀ∞Î∏ˆR
			float r = PAOPAO_SIZE/2;
			float skew = sqrt(pow(speedx, 2) + pow(speedy, 2));
			float oldx = currentXY.x - r * speedx /skew * i;
			float oldy = currentXY.y - r * speedy /skew * i;
cocos2d::Point  skewPoint = convertA1ToA2(_centerPoint, cocos2d::Point(oldx, oldy), angle);

			RowCol temp =  this->getRowColByPos(skewPoint.x, skewPoint.y);
			if(isValidPos(temp.m_nRow, temp.m_nCol) == false)
				return temp;
			if(_board[temp.m_nRow][temp.m_nCol] == NULL)
				return temp;
		}
		return ret;
	}
}

void GameLayer::checkBubbleWarning()
{
	int raduis = PAOPAO_SIZE/2;
	//int minx = 999999;
	//Bubble *minPp;
//	for(int row=0; row<MAX_ROWS+BOARD_EXTRA_SIZE; row++)
//	{
//		for(int col=0; col<MAX_COLS+BOARD_EXTRA_SIZE; col++)
//		{
//			if(_board[row][col] != NULL  && _board[row][col]->getColorType()>= PAOPAO_TYPE1 && _board[row][col]->getColorType() <= PAOPAO_TYPE6){
//				Bubble *pp = _board[row][col];
//				
//				Point pos =	pp->convertToWorldSpaceAR(Point(0,0));
//				
//				//check warning
//				bool warning = false;
//				int diffMulti = 3;
//				if( pos.x - diffMulti*raduis <=  _gameFrame.x ){
//					pp->playEffectForEver2(B_Warning, 0.2f);
//					warning = true;
//
//				}
//
//				if( pos.x + diffMulti*raduis >=  _gameFrame.x + _gameFrame.width ){
//					pp->playEffectForEver2(B_Warning, 0.2f);
//					warning = true;
//
//				}
//				if(pos.y - diffMulti*raduis <= _gameFrame.y){
//					pp->playEffectForEver2(B_Warning, 0.2f);
//					warning = true;
//
//				}
//				if(pos.y + diffMulti*raduis >= _gameFrame.y + _gameFrame.height){
//					pp->playEffectForEver2(B_Warning, 0.2f);
//					warning = true;
//					//std::cout << pp << "|" << (int)pos.x << "," << (int)pos.y << std::endl;
//				}
//
//				if(warning == false){
//					pp->removeEffect2();
//				}
//			}
//		}
//	}
}

bool GameLayer::isValidPos( int nRow, int nCol )
{
	if (nRow < 0 || nCol < 0)
	{
		return false;
	}
	if (nRow >= MAX_ROWS+BOARD_EXTRA_SIZE || nCol >= MAX_COLS+BOARD_EXTRA_SIZE - nRow % 2)
	{
		return false;
	}

	return true;
}

void GameLayer::checkGameOver()
{
	if (getGameState() == G_OVER)
	{
		return;
	}
	int raduis = PAOPAO_SIZE/2;

	for(int row=0; row<MAX_ROWS+BOARD_EXTRA_SIZE; row++)
	{
		for(int col=0; col<MAX_COLS+BOARD_EXTRA_SIZE; col++)
		{
			if(_board[row][col] != NULL){
				Bubble *pp = _board[row][col];
				cocos2d::Point  pos = pp->convertToWorldSpaceAR(cocos2d::Point(0,0));

				if( (pos.x -raduis <=  _gameFrame.x )
					|| ( pos.x +raduis >=  _gameFrame.x + _gameFrame.width )
					|| (pos.y - raduis <= _gameFrame.y)
					|| (pos.y + raduis >= _gameFrame.y + _gameFrame.height)
					)
				{
					gameOver();
				}
			}
		}

	}
}


void GameLayer::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{ 
	CCLayer::draw(renderer, transform, transformUpdated);
	
/*	glLineWidth(1);
	glColor4ub(255, 255, 0, 255);
	//glColor4f(1.0, 1.0, 0.0, 1.0); 
	for(int row=0; row<MAX_ROWS; row++)
	{
		
		int startx =  _paopaoOffset.x;
		int endx = MAX_COLS * PAOPAO_SIZE + _paopaoOffset.x;

		ccDrawLine(cocos2d::Point(startx, row * PAOPAO_SIZE+_paopaoOffset.y),Point(endx, row * PAOPAO_SIZE+_paopaoOffset.y)); 
		
	}

	for(int col=0; col<=MAX_COLS; col++)
	{
		int starty =  _paopaoOffset.y;
		int endy = MAX_ROWS * PAOPAO_SIZE + _paopaoOffset.y;

		ccDrawLine(cocos2d::Point(col*PAOPAO_SIZE + _paopaoOffset.x, starty),Point(col*PAOPAO_SIZE + _paopaoOffset.x, endy)); 
	}
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
cocos2d::Size  visibleSize = Director::getInstance()->getWinSize();  
	glLineWidth(3);
	//_centerPoint = ccp(origin.x + visibleSize.width/2 + PAOPAO_SIZE/2, origin.y+visibleSize.height/2);
	ccDrawLine(cocos2d::Point(0, _centerPoint.y), Point(origin.x + visibleSize.width, _centerPoint.y));
	ccDrawLine(cocos2d::Point(_centerPoint.x, 0), Point(_centerPoint.x, origin.y+visibleSize.height));
	*/
}

//º∑≈›≈›–ßπ˚
void GameLayer::crowdEffect(std::vector<RowCol> &bubbleList, float speedx, float speedy, int deltaDistance)
{
	for (std::vector<RowCol>::const_iterator iterBubble = bubbleList.begin();
		iterBubble != bubbleList.end();
		iterBubble++)
	{
		int nRow = iterBubble->m_nRow;
		int nCol = iterBubble->m_nCol;

		Bubble *obj = _board[nRow][nCol];
		if(obj){
			int oldx = obj->getPosition().x;
			int oldy = obj->getPosition().y;
			int deltax = 0, deltay=0;
			if(speedx<0)
				deltax = -deltaDistance;
			else
				deltax = deltaDistance;
			if(speedy<0)
				deltay = -deltaDistance;
			else
				deltay = deltaDistance;
			obj->runAction(
				Sequence::create(
				MoveTo::create(0.03f, cocos2d::Point(oldx+deltax, oldy+deltay)),
				MoveTo::create(0.07f, cocos2d::Point(oldx, oldy)),
				NULL
				)
				);
		}
	}

}


void GameLayer::findAGroundInList(std::vector<RowCol> &list, std::vector<RowCol> &ret)
{
	std::map<std::string, int> keymap;
	for (std::vector<RowCol>::const_iterator iterBubble = list.begin();
		iterBubble != list.end();
		iterBubble++)
	{
		int nRow = iterBubble->m_nRow;
		int nCol = iterBubble->m_nCol;

		std::stringstream sstrm;
		sstrm << nRow << "-" <<nCol;

		if(keymap.find(sstrm.str()) == keymap.end()){
			keymap.insert(std::map <std::string, int> :: value_type ( sstrm.str(), 1));
		}


	}

	for (std::vector<RowCol>::const_iterator iterBubble = list.begin();
		iterBubble != list.end();
		iterBubble++)
	{
		int nRow = iterBubble->m_nRow;
		int nCol = iterBubble->m_nCol;
		std::vector<RowCol> vec;
		this->getAround(nRow, nCol, vec);

		for (std::vector<RowCol>::const_iterator temp = vec.begin();
			temp != vec.end();
			temp++)
		{
			std::stringstream sstrm;
			sstrm << temp->m_nRow << "-" << temp->m_nCol;

			if(keymap.find(sstrm.str()) == keymap.end()){
				keymap.insert(std::map <std::string, int> :: value_type ( sstrm.str(), 1));
				ret.push_back(*temp);
			}
		}
	}
}

void GameLayer::killFinalEnemy(Bubble *pp)
{
	if(true == isKillFinalEnemy(pp->getRow(), pp->getCol())){
		int hurt = 10;
		hurt = g_talentMgr->getCurrentEffectValue(TT_BossHurt_Bump);
		changeEnemyBossHp(hurt);
	}
}

bool GameLayer::isKillFinalEnemy(int row, int col)
{
	std::vector<RowCol> vec;
	this->getAround(MAX_ROWS/2, MAX_COLS/2, vec);

	for (std::vector<RowCol>::const_iterator temp = vec.begin();
		temp != vec.end();
		temp++)
	{
		int nRow = temp->m_nRow;
		int nCol = temp->m_nCol;
		if(nRow == row && nCol == col){
			return true;
		}
	}
	return false;

}

void GameLayer::changeEnemyBossHp(int hurt)
{
	g_miscLayer->flyOneNumber2(Point(_gameFrame.x+_gameFrame.width/2, _gameFrame.y+_gameFrame.height/2), -hurt);
	_enemyBossHp -= hurt;
	int count = hurt/5;
	for(int i=0;i<count;i++)
	{
		this->addPower((PAOPAO_TYPE)PAOPAO_TYPE_CENTER,0,_centerPoint);
	}
	
	if(_enemyBossHp <= 0){
		g_scoreLayer->setBossHp(0,_oldEnemyBossHp);
		gameWin();
	}
	else{
		g_scoreLayer->setBossHp(_enemyBossHp,_oldEnemyBossHp);
	}
}

void GameLayer::removeCenterInList(std::vector<RowCol>& list)
{
	for (std::vector<RowCol>::iterator temp = list.begin();
		temp != list.end();
		temp++)
	{
		int nRow = temp->m_nRow;
		int nCol = temp->m_nCol;
		if(nRow == MAX_ROWS/2 && nCol == MAX_COLS/2){
			list.erase(temp);
			break;
		}
	}
}

void GameLayer::genSpecailBubbleInRule()
{
	genPlagueBubble();
	genGuardBubble();
	genInkBubble();
	genStoneBubble();
}

//≤˙…˙Œ¡“ﬂ
void GameLayer::genPlagueBubble()
{
	if(_currentBarrierData==NULL)
		return;
	if(_currentBarrierData->wenyi == 0)
		return;
	std::vector<Bubble *> list;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;

			if(_board[i][j]->getColorType() == PAOPAO_PLAGUE)
				return;

			if(_board[i][j]->getColorType() != PAOPAO_TYPE_CENTER)
				list.push_back(_board[i][j]);
		}
	}
	if(list.size() > 0){
		int hit = rand() % list.size();
		Bubble *bubble = list.at(hit);
		bubble->changeType(PAOPAO_PLAGUE);
		g_miscLayer->playWav("wenyi.wav" );

		this->setAttachAttribute(bubble, B_Attribute_EarthQuake);
	}
}

//≤˙…˙ª§Œ¿
void GameLayer::genGuardBubble()
{
	if(_currentBarrierData==NULL)
		return;
	if(_currentBarrierData->huwei == 0)
		return;
	std::vector<Bubble *> list;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;

			if(_board[i][j]->getColorType() == PAOPAO_GUARD)
				return;

			if(_board[i][j]->getColorType() != PAOPAO_TYPE_CENTER)
				list.push_back(_board[i][j]);
		}
	}
	if(list.size() > 0){
		int hit = rand() % list.size();
		Bubble *bubble = list.at(hit);
		bubble->changeType(PAOPAO_GUARD);

		this->setAttachAttribute(bubble, B_Attribute_ChangeType);
	}
}

/**ÀÊª˙ ØÕ∑
*/
void GameLayer::genStoneBubble()
{
	if(_currentBarrierData==NULL)
		return;
	if(_currentBarrierData->shitou == 0)
		return;
	std::vector<Bubble *> list;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;
			if(_board[i][j]->getColorType() != PAOPAO_TYPE_CENTER)
				list.push_back(_board[i][j]);
		}
	}
	if(list.size() > 0){
		int hit = rand() % list.size();
		Bubble *bubble = list.at(hit);
		bubble->changeType(PAOPAO_STONE);

		this->setAttachAttribute(bubble, B_Attribute_ResetHeart);
	}

}

//≤˙…˙ƒ´÷≠
void GameLayer::genInkBubble()
{
	if(_currentBarrierData==NULL)
		return;
	if(_currentBarrierData->mozhi == 0)
		return;
	std::vector<Bubble *> list;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;

			if(_board[i][j]->getColorType() == PAOPAO_INK)
				return;

			if(_board[i][j]->getColorType() != PAOPAO_TYPE_CENTER)
				list.push_back(_board[i][j]);
		}
	}
	if(list.size() > 0){
		int hit = rand() % list.size();
		Bubble *bubble = list.at(hit);
		
		bubble->setInkEffectRange(10);
		bubble->changeType(PAOPAO_INK);

		this->setAttachAttribute(bubble, B_Attribute_AssistLine);
	}
}



void GameLayer::specialBubbleProcess()
{
	bool isGuard = false;
	bool isPlague = false;
	bool isInk = false;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;

			if(_board[i][j]->getColorType() == PAOPAO_GUARD && isGuard== false){
				std::vector<RowCol> vlist;
				getAround(i, j, vlist);
				for (std::vector<RowCol>::iterator temp = vlist.begin();
					temp != vlist.end();
					temp++)
				{
					int nRow =temp->m_nRow;
					int nCol = temp->m_nCol;
					if(_board[nRow][nCol] == NULL){
						Bubble * obj = randomBubble();  
						obj->setRowCol(nRow, nCol);  
						obj->setPosition(getPointByRowAndCol(nRow, nCol));  
						this->addChild(obj);  
						obj->setRowCol(nRow, nCol);
						obj->setScale(obj->getScale()/3);
						_board[nRow][nCol] = obj;

						obj->runAction(Sequence::create(DelayTime::create(0.3f),
							ScaleTo::create(0.2f,obj->getScale()*3),
							NULL));
						isGuard = true;
						break;
					}
				}
			}

			if(_board[i][j]->getColorType() == PAOPAO_PLAGUE && isPlague==false && _plagueStep%5==0){ //Œ¡“ﬂ
				//Õ˘Àƒ÷‹¿©…¢
				std::vector<RowCol> vlist;
				getAround(i, j, vlist);
				for (std::vector<RowCol>::iterator temp = vlist.begin();
					temp != vlist.end();
					temp++)
				{
					int nRow =temp->m_nRow;
					int nCol = temp->m_nCol;
					if(_board[nRow][nCol] != NULL && _board[nRow][nCol]->getColorType()!=PAOPAO_PLAGUE
						&& _board[nRow][nCol]->getColorType()!=PAOPAO_INK
						&& _board[nRow][nCol]->getColorType()!=PAOPAO_GUARD
						&& _board[nRow][nCol]->getColorType()!=PAOPAO_STONE
						&& _board[nRow][nCol]->getColorType()!=PAOPAO_TYPE_CENTER){
						_board[nRow][nCol]->changeType(PAOPAO_PLAGUE);
						g_miscLayer->playWav("wenyi.wav" );
						isPlague = true;
						break;
					}
				}
			}
			if(_board[i][j]->getColorType() == PAOPAO_INK && isInk==false && _inkStep%5==0)
			{
cocos2d::Point  basePt = this->convertToWorldSpaceAR(_board[i][j]->getPosition());
				if(_board[i][j]->getInkEffectRange() != 0)
				{
					std::vector<RowCol> vlist;
					getAround(i, j, vlist);
					for (std::vector<RowCol>::iterator temp = vlist.begin();
						temp != vlist.end();
						temp++)
					{
						int nRow =temp->m_nRow;
						int nCol = temp->m_nCol;
						if(_board[nRow][nCol] != NULL 
							&& _board[nRow][nCol]->getColorType()!=PAOPAO_TYPE_CENTER
							&& _board[nRow][nCol]->getColorType()!=PAOPAO_INK
							&& _board[nRow][nCol]->getColorType()!=PAOPAO_PLAGUE
							&& _board[nRow][nCol]->getColorType()!=PAOPAO_GUARD
							&& _board[nRow][nCol]->getColorType()!=PAOPAO_STONE)
						{
cocos2d::Point  cmpPt = this->convertToWorldSpaceAR(_board[nRow][nCol]->getPosition());
							if(cmpPt.y < basePt.y - PAOPAO_SIZE/3 && abs(cmpPt.x - basePt.x) < PAOPAO_SIZE/2){
								_board[nRow][nCol]->setInkEffectRange( _board[i][j]->getInkEffectRange() -1 );

								_board[nRow][nCol]->changeType(PAOPAO_INK);
								
								_board[i][j]->removeInkEffect();

								isInk = true;
								break;
							}
						}
					}
				}
			}
		}
	}

}


int GameLayer::roundCount(int row, int col)
{
	int crow = floor(MAX_ROWS/2);
	int type = crow % 2;
	row = row - floor(MAX_ROWS/2);
	col = col - floor(MAX_COLS/2);

	if (row % 2 == 0) 
	{
		if (abs(col) * 2 <= abs(row)) return abs(row);
		else return abs(row)+(abs(col)-abs(row)/2);
	} else if (type == 0){
		if(col <= (abs(row)-1)/2 && col >= (-abs(row)-1)/2) return abs(row);
		else if(col > (abs(row)-1)/2) return abs(row) + col - (abs(row)-1)/2;
		else if(col < (-abs(row)-1)/2) return abs(row) + (-abs(row)-1)/2 - col;
	} else {
		if(col <= (abs(row)+1)/2 && col >= (-abs(row)+1)/2) return abs(row);
		else if(col > (abs(row)+1)/2) return abs(row) + col - (abs(row)+1)/2;
		else if(col < (-abs(row)+1)/2) return abs(row) + (-abs(row)+1)/2 - col;
	}

	return 0;
}

void GameLayer::earthquake()
{

	int talentLevel = 1;//g_talentMgr->getTalent(TT_Earthquake)->getTalentLevel();
	ROWCOL_LIST removeBubbleList;

	int round = 0;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL){
				int rd = roundCount(i, j);
				if(rd!=0 && rd>round)
					round = rd;
			}
		}
	}

	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL && _board[i][j]->getColorType() != PAOPAO_TYPE_CENTER){
				int rd = roundCount(i, j);
				if(round != 0 && rd>round-talentLevel)
				{
					removeBubbleList.push_back(RowCol(i, j));
				}
			}
		}
	}


	if(removeBubbleList.size() > 0){
		auto shake = Shake::create(0.2f, 5);
		this->runAction(shake);

		dropBubble(removeBubbleList);
	}
}


float GameLayer::getAngleSpeed(float v, float r)
{
	float sum = 1.0f;
	for (int i = 0; i < MAX_ROWS; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS; ++ j)  
		{  
			if(_board[i][j] == NULL)
				continue;

			sum += _board[i][j]->getAngleForce();
		}
	}

	return v * r / sum;
}

void GameLayer::randomAngle()
{
	int angle = rand() % 90;
	FiniteTimeAction *action = RotateTo::create(0.6f * angle/30, angle);
	this->runAction(action);
}


bool GameLayer::hasBubbleWithSameType(PAOPAO_TYPE type)
{
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL && _board[i][j]->getColorType()==type)
				return true;
		}
	}
	return false;
}

int GameLayer::bubbleWithSameType(PAOPAO_TYPE type)
{
	int sum = 0;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL && _board[i][j]->getColorType()==type)
				sum++;
		}
	}
	return sum;
}

void GameLayer::checkSpecialReward()
{
	bool ink = g_shotLayer->hasSpecialBubbleOccur(PAOPAO_INK);
	if(ink == true)
	{
		bool inkReward = true;
		for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
		{  
			for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
			{  
				if(_board[i][j] != NULL && _board[i][j]->getColorType()==PAOPAO_INK)
					inkReward = false;
			}
		}

		if(inkReward == true)
		{
			g_shotLayer->setLienShow(true);
			g_shotLayer->deleteSpecialBubbleOccur(PAOPAO_INK);
		}
	}
	bool plague = g_shotLayer->hasSpecialBubbleOccur(PAOPAO_PLAGUE);
	if(plague == true)
	{
		bool plagueReward = true;
		for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
		{  
			for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
			{  
				if(_board[i][j] != NULL && _board[i][j]->getColorType()==PAOPAO_PLAGUE)
					plagueReward = false;
			}
		}

		if (plagueReward == true)
		{
			g_shotLayer->deleteSpecialBubbleOccur(PAOPAO_PLAGUE);
			this->earthquake();
		}
	}
	
}

void GameLayer::changeSmallest2OtherColor()
{
	std::map<int, int> countMap;
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL)
			{
				int type = _board[i][j]->getColorType();
				if(type>=PAOPAO_TYPE1 && type<=PAOPAO_TYPE6){
					if(countMap.find(type) != countMap.end())
						countMap[type] = countMap[type] + 1;
					else
						countMap[type] = 1;
				}
			}
		}
	}

	if(countMap.size() <= 2)
		return;

	int smallest = 9999;
	int smallestType = PAOPAO_TYPE_UNKNOW;
	std::vector <int> colorVec;
	for(std::map<int, int>::iterator iter=countMap.begin(); iter!=countMap.end(); ++iter)
	{
		if(iter->second < smallest){
			smallestType = iter->first;
			smallest = iter->second;
		}

		colorVec.push_back(iter->first);
	}

	for(std::vector <int>::iterator iter=colorVec.begin(); iter!=colorVec.end(); ++iter)
	{
		if((*iter) == smallestType){
			colorVec.erase(iter);
			break;
		}
	}

	int rndIndex = rand() % colorVec.size();

	int changeColor = colorVec.at(rndIndex);

	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  
			if(_board[i][j] != NULL && _board[i][j]->getColorType()==smallestType)
			{
				_board[i][j]->changeType(static_cast<PAOPAO_TYPE>(changeColor));
			}
		}
	}
}

StatData GameLayer::getStatData()
{
	return this->_statData;
}


void GameLayer::animationNormalBubble(float fDelta)
{
	_normalBubbleActionTime += fDelta;

	if(static_cast<int>(_normalBubbleActionTime) % 3 == 1){
		int totalBubble = 0;
		for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
		{  
			for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
			{  
				if(_board[i][j] != NULL && _board[i][j]->getColorType()>=PAOPAO_TYPE1 && _board[i][j]->getColorType()<=PAOPAO_TYPE6)
				{
					totalBubble++;
					//_board[i][j]->stopNormalAction();
				}
			}
		}

		if(totalBubble > 0){
			for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
			{  
				for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
				{  
					if(_board[i][j] != NULL && _board[i][j]->getColorType()>=PAOPAO_TYPE1 && _board[i][j]->getColorType()<=PAOPAO_TYPE6)
					{
						if(rand()% totalBubble <= 3){
							//_board[i][j]->playNormalAction(true);
							//_board[i][j]->tingle(fDelta);
						}
					}
				}
			}
		}
	}
}

void GameLayer::doublePower()
{
	this->_power *= 2;
	_power = std::min((float)MAX_POWER, _power);
	g_powerLayer->changePowerBar(_power);
    if(_power>=MAX_POWER)
	{
		g_powerLayer->setPowericonEffect();
		return;
	}
}

void GameLayer::addPower(PAOPAO_TYPE type,int clearType,cocos2d::Point bubblePo)//0 œ˚≥˝ 1 µÙ¬‰
{
	if(_power>=MAX_POWER)
	{
		g_powerLayer->setPowericonEffect();
		return;
	}
	float tt_value=g_talentMgr->getCurrentEffectValue(TT_Power)/100.0;
	switch (type)
	{
		case PAOPAO_TYPE1: 
		case PAOPAO_TYPE2: 
		case PAOPAO_TYPE3:  
		case PAOPAO_TYPE4:  
		case PAOPAO_TYPE5: 
		case PAOPAO_TYPE6:
		case PAOPAO_BOMB: 
		case PAOPAO_COLOR: 
			if(clearType==0)
			{
				_power += NORMAL_CLEAR*tt_value;
			}else
			{
				_power += NORMAL_DROP*tt_value;
			}
			break;
		case PAOPAO_PLAGUE:
			if(clearType==0)
			{
				_power += PLAGUE_CLEAR*tt_value;
			}else
			{
				_power += PLAGUE_DROP*tt_value;
			}
			break;
		case PAOPAO_GUARD:
			_power += GUARD*tt_value;
			break;
		case PAOPAO_INK:
			_power += INK*tt_value;
			break;
		case PAOPAO_STONE:
			_power += STONE*tt_value;
			break;
		case PAOPAO_TYPE_CENTER://BOSSµÙ—™
			_power +=BOSS_HP *tt_value;
			break;
	}
	_power = std::min((float)MAX_POWER, _power);
	g_powerLayer->changePowerBar(_power);
	if(_power>=MAX_POWER)
	{
		g_powerLayer->setPowericonEffect();
		return;
	}
}
void GameLayer::addShotPower()
{
	if(_power>=MAX_POWER)
	{
		g_powerLayer->setPowericonEffect();
		return;
	}
	float tt_value=g_talentMgr->getCurrentEffectValue(TT_Power)/100.0;
	_power += SHOT*tt_value;
	_power = std::min((float)MAX_POWER, _power);
	g_powerLayer->changePowerBar(_power);

	if(_power>=MAX_POWER)
	{
		g_powerLayer->setPowericonEffect();
		return;
	}
}

bool GameLayer::isCollisionBubble(float prex, float prey, float x, float y)
{
	float angle = this->getRotation(); 

	int step = PAOPAO_SIZE / 8;

	if(abs(x-prex) >  1e-4)
	{
		/*
		float factor = abs(y-prey) / abs(x-prex);

		float checkx = prex;
		float checky = prey;

		while (abs(checkx-x) < 1.0 && abs(checky-y) < 1.0)
		{
			cocos2d::Point  truePoint = convertA1ToA2(_centerPoint, cocos2d::Point(checkx, checky), angle);
			RowCol rc = this->getRowColByPos(truePoint.x, truePoint.y);
			if(_board[rc.m_nRow][rc.m_nCol] != NULL)
				return true;
			//return false;
			float checkx = prex + step * sign(x-prex);
			float checky = prey + step * sign(y-prey) * factor;
		}*/
	}

	cocos2d::Point  truePoint = convertA1ToA2(_centerPoint, cocos2d::Point(x, y), angle);
	RowCol rc = this->getRowColByPos(truePoint.x, truePoint.y);
	if(_board[rc.m_nRow][rc.m_nCol] != NULL)
		return true;
	return false;
}

//µ±«∞¥•∑¢ƒƒ∏ˆ
void GameLayer::setPowerTrick(Talent_Type type)
{
	int count = 0;
	switch (type)
	{
	case TT_AssistLine:
		g_shotLayer->setLienShow(true);
		break;
	case TT_HeartFreeze:
		g_scoreLayer->iceSprite->setVisible(true);
		count = g_talentMgr->getCurrentEffectValue(TT_HeartFreeze);
		g_shotLayer->setHeartFreeze(count);
		break;
	case TT_Electric:
		count = g_talentMgr->getCurrentEffectValue(TT_Electric);
		g_shotLayer->setElectricBubble(count);
		break;
	case TT_FireShot:
		g_miscLayer->addFireShot(true);
		g_shotLayer->setIsShotting(true);
		count = g_talentMgr->getCurrentEffectValue(TT_FireShot);
		_hpCount=count;
		this->runAction(Sequence::create(
			DelayTime::create(4.0f),
			CallFuncN::create(this,callfuncN_selector(GameLayer::changeHp)),
			NULL));
		break;
	}
}
void GameLayer::changeHp(Node* node)
{
	g_miscLayer->playWav("fire.wav");
	this->changeEnemyBossHp(_hpCount);
	g_miscLayer->addFireShot(false);
	g_shotLayer->setIsShotting(false);
}
Talent_Type GameLayer::getPowerTrick()
{
	return _powerTalent;
}
void GameLayer::setNowPowerType(Talent_Type type)
{
	_powerTalent = type;
}


void GameLayer::addBubbleOperation(PAOPAO_TYPE type)
{
	if (_bubbleOperationList.size() >= 3)
	{
		_bubbleOperationList.pop_front();
	}

	_bubbleOperationList.push_back(type);

	/*if (_bubbleOperationList.size() < 3)
	{
		return;
	}*/

	int level = g_talentMgr->getCurrentTalentLevel(TT_Skill_XX);
	PAOPAO_TYPE rewardType = PAOPAO_TYPE_UNKNOW;

	PAOPAO_TYPE op[3];
	int index = 0;
	for(int i=0; i<3; i++)
	{
		op[i]=PAOPAO_TYPE_UNKNOW;
	}

	if(_bubbleOperationList.size() >= 3)
	{
		for(std::list<PAOPAO_TYPE>::iterator it=_bubbleOperationList.begin(); it!=_bubbleOperationList.end(); it++)
		{
			op[index] = (*it);
			index++;
		}
	}
	else
	{
		for(std::list<PAOPAO_TYPE>::iterator it=_bubbleOperationList.begin(); it!=_bubbleOperationList.end(); it++)
		{
			op[2-index] = (*it);
			index++;
		}
	}

	std::list<int> addSkillList;
	bool storeSkill = false;


	
	if(level>=0)
	{	if(
			(op[0]>=PAOPAO_TYPE1 && op[0]<=PAOPAO_TYPE6)
			&&
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}
		
		if( 
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)&&
            (op[2]==op[1])
		)
		{
			rewardType = PAOPAO_BOMB;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

	}
	if(level>=1)
    {    if(
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_BOMB;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}
    }
	if (level>=2) {
       if(
           (op[0]==PAOPAO_COLOR)
           &&
           (op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
           &&
           (op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
           )
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}
    }
	if(level>=3)
	{
        
        if(
			(op[1]==PAOPAO_COLOR)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_BOMB;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
			&&
			(op[2]==PAOPAO_COLOR)
		)
		{
			rewardType = PAOPAO_BOMB;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]==PAOPAO_COLOR)
		)
		{
			rewardType = PAOPAO_BOMB;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}
	}
	

	if(level==4)
	{	if(
			(op[0]==PAOPAO_COLOR)
			&&
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]==PAOPAO_COLOR)
			&&
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]==PAOPAO_BOMB)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]==PAOPAO_COLOR)
			&&
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
			&&
			(op[2]==PAOPAO_BOMB)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]==PAOPAO_BOMB)
			&&
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]==PAOPAO_BOMB)
			&&
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]==PAOPAO_BOMB)
			&&
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]==PAOPAO_BOMB)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]==PAOPAO_BOMB)
			&&
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
			&&
			(op[2]==PAOPAO_BOMB)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

		if( 
			(op[0]>=PAOPAO_TYPE1 && op[0]<=PAOPAO_TYPE6)
			&&
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]>=PAOPAO_TYPE1 && op[2]<=PAOPAO_TYPE6)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}
		if( 
			(op[0]>=PAOPAO_TYPE1 && op[0]<=PAOPAO_TYPE6)
			&&
			(op[1]==PAOPAO_BOMB)
			&&
			(op[2]==PAOPAO_BOMB)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}
		if( 
			(op[0]>=PAOPAO_TYPE1 && op[0]<=PAOPAO_TYPE6)
			&&
			(op[1]>=PAOPAO_TYPE1 && op[1]<=PAOPAO_TYPE6)
			&&
			(op[2]==PAOPAO_BOMB)
		)
		{
			rewardType = PAOPAO_COLOR;
			if(g_skillBoxLayer->hasSkill() || storeSkill==true){
					addSkillList.push_back(rewardType);
					storeSkill=true;
			}else{
					g_shotLayer->changeShottingBubble(rewardType);
                    storeSkill=true;
			}
		}

    }
	
	if(rewardType == PAOPAO_TYPE_UNKNOW)
		return;
	g_skillBoxLayer->addSkill(addSkillList);
	

	_bubbleOperationList.clear();
}

void GameLayer::restoreCurrentBarrier()
{
	this->resetValue();
	this->removeAllChildren();
	this->initTheBoard();

	this->initBarrier(this->_currentBarrierData->id,true);
	
	if (this->getGameState() == G_OVER)
	{
		g_shotLayer->gameRestart();
		this->scheduleUpdate();
		this->setGameState(G_START);
	}
}

void GameLayer::setPower(int power)
{
	_power = power;
}

void GameLayer::useLaser()
{
    if(g_miscLayer->getChildByTag(MASKLAYER)==NULL)
    {
        MaskLayer *mask = MaskLayer::create();
        g_miscLayer->addChild(mask);
        mask->setTag(MASKLAYER);
    }
    
	_lasterClearList.clear();
	for (int i = 0; i < MAX_ROWS+BOARD_EXTRA_SIZE; ++ i)  
	{  
		for (int j = 0; j < MAX_COLS+BOARD_EXTRA_SIZE; ++ j)  
		{  

			if(_board[i][j] != NULL&&_board[i][j]->getColorType()!=PAOPAO_TYPE_CENTER)
			{
				bool erase = true;
				std::vector<RowCol> vec;
				getAround(i, j, vec);
				for (std::vector<RowCol>::const_iterator iterBubble = vec.begin();
					iterBubble != vec.end();
					iterBubble++)
				{
					Bubble *bb = _board[iterBubble->m_nRow][iterBubble->m_nCol];
					if(bb!=NULL && bb->getColorType() == _board[i][j]->getColorType()){
						erase = false;
						break;
					}
				}

				if(erase == true)
				{
					_lasterClearList.push_back(RowCol(i, j));
					_board[i][j]->playEffectOnce(B_Laster);

					cocos2d::Point  pt = _board[i][j]->convertToWorldSpaceAR(_board[i][j]->getAnchorPoint());
					g_miscLayer->fireLaster(pt);
				}
			}
			if(_board[i][j] != NULL&&_board[i][j]->getColorType()==PAOPAO_TYPE_CENTER)
			{
				cocos2d::Point  pt = _board[i][j]->convertToWorldSpaceAR(_board[i][j]->getAnchorPoint());
				g_miscLayer->fireLaster(cocos2d::Point(pt.x,pt.y+40));
			}
		}
	}
	
	
	this->runAction(
	Sequence::create(
		DelayTime::create(1.0f),
		CallFuncN::create(this, callfuncN_selector(GameLayer::removeLasterClearList)),
		NULL
		)
		);

	this->runAction(
		Sequence::create(
		DelayTime::create(1.0f),
		CallFuncN::create(this, callfuncN_selector(GameLayer::lasterKillBoss)),
		NULL
		)
		);
	//this->changeEnemyBossHp(10);
}

void GameLayer::removeLasterClearList(Node *obj)
{
	if(_lasterClearList.size() > 0){
		this->clearBubble(_lasterClearList, false);
		this->findNonLinkedBubble();
	}
}


void GameLayer::lasterKillBoss(Node *obj)
{
	this->changeEnemyBossHp(10);
    if(g_miscLayer->getChildByTag(MASKLAYER)!=NULL)
    {
        g_miscLayer->removeChild(g_miscLayer->getChildByTag(MASKLAYER));
    }
}
int GameLayer::getCurrentBarrierGold(int size)
{
	int gold=0; 
	switch (g_talentMgr->getCurrentTalentLevel(TT_GoldMulti))
	{
	case 0:
		gold= size * _currentBarrierData->goldBase;
		break;
	case 1:
		if(_currentBarrierData->id==1)
		{
			gold= size * _currentBarrierData->goldBase;
		}else
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *g_talentMgr->getCurrentEffectValue(TT_GoldMulti);
		}
		break;
	case 2:
		if(_currentBarrierData->id==1)
		{
			gold= size * _currentBarrierData->goldBase;
		}else if(_currentBarrierData->id==2)
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *2;
		}else
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *g_talentMgr->getCurrentEffectValue(TT_GoldMulti);
		}
		break;
	case 3:
		if(_currentBarrierData->id==1)
		{
			gold= size * _currentBarrierData->goldBase;
		}else if(_currentBarrierData->id==2)
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *2;
		}else if(_currentBarrierData->id==3||_currentBarrierData->id==4)
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *3;
		}else
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *g_talentMgr->getCurrentEffectValue(TT_GoldMulti);
		}
		break;
	case 4:
		if(_currentBarrierData->id==1)
		{
			gold= size * _currentBarrierData->goldBase;
		}else if(_currentBarrierData->id==2)
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *2;
		}else if(_currentBarrierData->id==3||_currentBarrierData->id==4)
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *3;
		}else if(_currentBarrierData->id==5||_currentBarrierData->id==6)
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *4;
		}else
		{
			gold= size * _currentBarrierData->goldBase*_currentBarrierData->goldTalent *g_talentMgr->getCurrentEffectValue(TT_GoldMulti);
		}
		break;
	default:
		break;
	}
	return gold;
}
void GameLayer::addtotalCoin(int gold)
{
    _totalCoinInRound+=gold;
    g_scoreLayer->updateGold(_totalCoinInRound);
}
void GameLayer::addBuyGold(int goldType)
{
	int gold=g_coinShopMgr->getCoin(goldType);
    if(gold==0&&goldType==0)
    {
        g_goodsData->gold=g_goodsData->gold*2;
    }else
    {
        g_goodsData->gold+=gold;
    }
	
    g_gameLayer->saveGold2File(g_goodsData->gold);
	g_coinShopUI->updateGold(g_goodsData->gold);
    
    
    if(g_miscLayer->getChildByTag(UPGRADE))
        g_upGradeLayer->updateGold(g_goodsData->gold);
    
}
void GameLayer::addBuyGem(int gemType)
{
	int gem=g_coinShopMgr->getCoin(gemType);
	g_goodsData->gem+=gem;
    g_gameLayer->saveGold2File(g_goodsData->gem);
	g_coinShopUI->updateGem(g_goodsData->gem);
    if (g_miscLayer->getChildByTag(GOODSHOP)) {
        g_goodsShopLayer->updateGem(g_goodsData->gem);
    }
    
    if(g_miscLayer->getChildByTag(UPGRADE))
        g_upGradeLayer->updateGem(g_goodsData->gem);
}
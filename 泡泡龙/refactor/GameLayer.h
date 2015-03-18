#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__
#include "cocos2d.h"
#include "GameConst.h"
#include "MyRect.h"
#include "GameEnum.h"
#include "EffectDef.h"
#include "Stat.h"
using namespace cocos2d;

class Bubble;
class ShotLayer;
class BarrierData;

typedef std::list<RowCol>  ROWCOL_LIST;

#define BOARD_EXTRA_SIZE 10 //需要增加数值大小，对角线的时候不算死

class GameLayer :
	public Layer
{
public:
	GameLayer(void);
	virtual ~GameLayer(void);
	virtual bool init();  
	CREATE_FUNC(GameLayer);  
    virtual void update(float fDelta); 
	virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated);
	void doScale(Node *node);

public:
	void gameStart();
	void gameOver();
	void gameRealOver();
	void gameWin();
	void gameRestart();
	void nextBarrier();
	CC_SYNTHESIZE(MyRect, _gameFrame, GameFrame);
	void initBarrier(int barrier,bool isRestTime);

	bool stickAGrid(Bubble *paopao, ShotLayer *sl);

	CC_SYNTHESIZE(int, _gameState, GameState);
	cocos2d::Point  getCenterPoint() { return this->_centerPoint;}
	bool stickAGrid2(Bubble *paopao, ShotLayer *sl);

	//bool stickAGridBySpecailBubble(Bubble *paopao, ShotLayer *sl);

	void genSpecailBubbleInRule();
	void genPlagueBubble(); //随机瘟疫
	void genGuardBubble(); //随机护卫
	void genInkBubble(); //随机滴墨汁
	void genStoneBubble(); ////随机石头
	void specialBubbleProcess(); //护卫球，会滋生随机泡泡
	void earthquake(); //地震

	BarrierData * getCurrentBarrierData(){return _currentBarrierData;}
	int getEnemyBossHp(){return _enemyBossHp;}
	int getOldEnemyBossHp(){return _oldEnemyBossHp;}

	std::vector<RowCol> checkPass(PAOPAO_TYPE color, int oldx, int oldy, int newx, int newy);


	void saveGold2File(int gold);
    void addGold(int addGold);
    
    void addBuyGold(int goldType);
    void addBuyGem(int gemType);

	bool hasBubbleWithSameType(PAOPAO_TYPE type);
	int bubbleWithSameType(PAOPAO_TYPE type);
	int roundCount(int row, int col);
	void setFirstBarrierBubble(PAOPAO_TYPE type,int cRow,int cCol);
	void gameRestartFormOne();

	StatData getStatData();
	void addNextUI(Node* node);
	void bossDie(Node* node);
	void removeBoss(Node* node);

	bool isCollisionBubble(float prex, float prey, float x, float y);

	void addPower(PAOPAO_TYPE type,int clearType,cocos2d::Point bubblePo);//0 消除 1 掉落
	void addShotPower();//发射得分

	void setPowerTrick(Talent_Type type);
	Talent_Type getPowerTrick();
	void setNowPowerType(Talent_Type type);//当前选择的power

	void changeHp(Node* node);
	
	void restoreCurrentBarrier();
	void doublePower();
	void setPower(int power);
	void useLaser();

	CC_SYNTHESIZE(int, _enableTalentUpdate, EnableTalentUpdate);

	void changeEnemyBossHp(int hurt);
private:
	void calOffsetXY();
	//通过行列获取位置
	cocos2d::Point  getPointByRowAndCol(int row, int col);

	RowCol getRowColByPos( int nPosX, int nPosY);
	bool initTheBoard();


	//随机产生一个泡泡  
	Bubble * randomBubble()  ;

	RowCol checkCollision(Bubble *pp, float angle);

	RowCol findTrueBlankPos(RowCol rc, cocos2d::Point  currentXY, float speedx, float speedy, float angle);

	bool isValidPos( int nRow, int nCol );

	float area(double x0,double y0,double x1,double y1,double x2,double y2){ //有向面积
		return x0*y1+y0*x2+x1*y2-x2*y1-x0*y2-x1*y0 ;
	}

	void checkBubbleWarning();

	ROWCOL_LIST findClearBubble(Bubble *pReadyBubble);
	ROWCOL_LIST findSameBubble(Bubble *pReadyBubble, std::map<Bubble *, int> findedMap);

	void getAround( int nRow, int nCol, std::vector<RowCol> & vecPos );

	void clearBubble(const ROWCOL_LIST &bubbleList, bool hurtBoss=true);
	ROWCOL_LIST findNonLinkedBubble();
	void dropBubble(const ROWCOL_LIST & bubbleList);

	void checkGameOver();

	void crowdEffect(std::vector<RowCol> &bubbleList, float speedx, float speedy, int deltaDistance);
	void findAGroundInList(std::vector<RowCol> &list, std::vector<RowCol> &ret);

	void checkBumbBubble(); //判断是否有炸弹要爆炸

	ROWCOL_LIST findSameBubbleByColor(PAOPAO_TYPE nColor, int row, int col);

	void killFinalEnemy(Bubble *pp);

	void removeCenterInList(std::vector<RowCol>& list);

	bool isKillFinalEnemy(int row, int col);

	
	void hurtEnemyBossByBubbleClear(Node *obj, void *param);


	void processBubbleAfterRatation();

	bool isInRowColList(std::vector<RowCol> &vec, int row, int col);

	void setAttachAttribute(Bubble *pBubble, BubbleEffect effect);
	
	void checkAttachEffect(Bubble *obj);

	PAOPAO_TYPE changeMinTypeInBoard(); //获得颜色最少的球

	float getAngleSpeed(float v, float r);

	void randomAngle();

	void resetValue();

	void restorBubbleTypeByPlague(int row, int col); //中瘟疫的需要恢复

	void checkSpecialReward(); //检查特殊奖励

	void changeSmallest2OtherColor();

	void firstBarrierSpecialList(); //第一关卡的

	void animationNormalBubble(float fDelta); //随机动画

	void dropAllLeftBubbleWhenWin(Node *node);

	void addBubbleOperation(PAOPAO_TYPE type);

	void removeLasterClearList(Node *obj);
	void lasterKillBoss(Node *obj);
	int getCurrentBarrierGold(int size);
private:
	Bubble *_board[MAX_ROWS+BOARD_EXTRA_SIZE][MAX_COLS+BOARD_EXTRA_SIZE]; //棋盘
	cocos2d::Point  _centerPoint;
	cocos2d::Point  _centerPointOffset;
	cocos2d::Point  _paopaoOffset;

	float _rorateDegeree;

	int _bumbingTime;

	int _enemyBossHp;
	int _oldEnemyBossHp;

	int _currrentBarrierProgress;
	BarrierData *_currentBarrierData;

	//int _seqClearCount; //连续消除计数
	//int _seqSameClearCount; //连续消除同色计数
	//PAOPAO_TYPE _lastClearColor; 

	std::list<PAOPAO_TYPE> _bubbleOperationList; //记录最后三次的操作
	

	int _plagueStep ; //瘟疫步骤数
	int _inkStep; //墨汁步骤数

	StatData _statData;

	int _totalCoinInRound;
	//int _winCoinInOneRound;

	float _normalBubbleActionTime;


	Talent_Type _powerTalent;

	Bubble *centerPp;
	int _hpCount;
	float _power;
	

	ROWCOL_LIST _lasterClearList;

public://购买道具次数
	int _EarthquakeCount;
	int _EnergyInjectionCount;
	int	_LaserCount;
	int	_TimelaspeCount;
    
    void addtotalCoin(int gold);
};

#endif


#ifndef __ShotLayer_H__
#define __ShotLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "GameEnum.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;
class GunSprite;
class Bubble;
class BarrierData;
class GrassSprite;

class ShotLayer :
	public Layer
{
public:
	ShotLayer(void);
	virtual ~ShotLayer(void);

	virtual bool init();  
	CREATE_FUNC(ShotLayer);  

	void genShotPaoPao(PAOPAO_TYPE type=PAOPAO_TYPE_UNKNOW);

	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
	
	
	void initGun();

	
	void bubbleMove(Bubble *pp, float fDelta, bool isShotPaoPao);

	void updateLoop(float fDelta);

	void stopShot(bool bGenNextPaoPao, bool incNullBubble);


	void gameOver();
	void gameRestart();
	void addBubbleDifficulty();
	void stopRandomBubble(Bubble *bb);

	//void addMozhiBubble(Point pos, float delayTime);
	void addBubble2SpecialList(Node *pp);
	
	void removeSpecialBubbleInShotLayer(Bubble *pBubble);

	//void processSpecialBubbleWhenStick(Bubble *pBubble);

	void showAssistLine(cocos2d::Point start,  float angle);

	void setLienShow(bool show);

	void changeShottingBubble(PAOPAO_TYPE type);
	void changeShottingBubbleByRandom();
	void resetHeart();
	void resetHeartKeepResetRound();

	bool hasSpecialBubbleOccur(PAOPAO_TYPE type);

	void changeBarrierNumber(Sprite *sp, int num);
	void deleteSpecialBubbleOccur(PAOPAO_TYPE type);

	void setHeartFreeze(int count);

	void setElectricBubble(int count);
	void setIsShotting(bool isShotting);

	cocos2d::Point getGunPosition();
    cocos2d::Point _nextBubblePo;

	void showAGrass(int index);
	void hideAGrass(int index);

	void resetCurrentShottingType2UnKnow()
	{
		_currentShottingType = PAOPAO_TYPE_UNKNOW;
	}

private:
	float getRotationByPos(cocos2d::Point mouseCurser);
	void shotBubble();
	//随机产生一个泡泡  
	Bubble * randomBubble4Shot(PAOPAO_TYPE type=PAOPAO_TYPE_UNKNOW);
	Bubble *  randomBubble4Noraml();
	void genNextPaoPao();

	void testButton();
	void testTouchEvent(Sprite* pSender,TouchEventType type);
	cocos2d::Point  generateSpeed(cocos2d::Point start,cocos2d::Point  target);

	//bool processSpecialBubbleInShotLayer(Bubble *pBubble);
	void removeSpriteInShotLayer(Node *node);
	void reset();

	Bubble * genSpecialBubble4RandomAdd(PAOPAO_TYPE type);

	
	PAOPAO_TYPE whichSepcialType(BarrierData *curBarrier, int pos);

	void restoreCurrentBubblePosition();

	bool isCollision(cocos2d::Point pos1, cocos2d::Point pos2, float r1, float r2);

	int genGrass();

	void checkGrassDie();

private:
	GunSprite *_gun;
	Bubble *_nextBubble;
	Bubble *_currentBubble;
	std::vector<Bubble *> _randomBubbleList;
	std::vector<Bubble *> _specialBubbleList;
private:
	int _shotNullCount; //放空枪
	int _resetHeartRound; // 生命消耗第几轮了
	cocos2d::Point  _gunPos;
	cocos2d::Point  _shotPaoPaoPos;

	bool _isShotting;
	bool _enableShotting;
	float _speedX;
	float _speedY;
	float _lastBossShowtime;

	bool _bShowLine;

	float _currentAngle;

	//Sprite **_barrierNumbers;
	

	std::map<int, int> _specialBubbleShowMap; //特殊球是不是出现过

	int _lineShowCount;

	int _heartFreeze; //已经用了几次了
	int _electricBubble; //触发电击

	bool _isOver;

	PAOPAO_TYPE _currentShottingType;

	std::vector<GrassSprite *> _grassList;

	int _shotCount;

	int _grassShowCount ;
    int _nextDiff;
public:
    //Sprite *_gunBg;
	void changeBubblePosition(Node *node);
};

#endif


#ifndef __Bubble_H__
#define __Bubble_H__

#include "cocos2d.h"
#include "GameEnum.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "EffectDef.h"
using namespace cocostudio;
using namespace cocos2d;
class Bubble :
	public Sprite
{
public:
	Bubble(void);
	virtual ~Bubble(void);
	CREATE_FUNC(Bubble);
	static Bubble *initWithType(PAOPAO_TYPE type); 
	static Bubble *initWithPic(std::string pic);
public:
	
	PAOPAO_TYPE getColorType();
	void setColorType(PAOPAO_TYPE type);
	//CC_SYNTHESIZE(PAOPAO_TYPE, _colorType, ColorType);//泡泡的类型  
	void setRowCol(int row, int col);
	int getRow(){return _row;}
	int getCol(){return _col;}
	CC_SYNTHESIZE(bool, _enableShot, EnableShot);
	CC_SYNTHESIZE(float, _speedX, SpeedX);
	CC_SYNTHESIZE(float, _speedY, SpeedY);
	CC_SYNTHESIZE(int, _collisionCount, CollisionCount);

	CC_SYNTHESIZE(Bubble*, _stickPp, StickPp);

	CC_SYNTHESIZE(int, _bubbleState, BubbleState);
	CC_SYNTHESIZE(BubbleEffect, _effectType, EffectType);//泡泡的类型  
	void setSpeedXY(float px, float py){this->_speedX = px; this->_speedY = py;}

	void playEffectOnce(BubbleEffect type, float delay=0.05f);
	void playEffectForEver(BubbleEffect type, float delay);
	void playEffectForEver2(BubbleEffect type, float delay);
	//void surroundMozhi();
	void changeType(PAOPAO_TYPE type);

	void playNormalAction(bool loop);

	CC_SYNTHESIZE(int, _specialAbility, SpecialAbility);

	void setInkEffectRange(int range);
	int getInkEffectRange();
	void removeInkEffect();

	void setAttachAttribute(BubbleEffect attribute);
	BubbleEffect getAttachAttribute();

	void changeImage(const char *image);

	CC_SYNTHESIZE(float, _angleForce, AngleForce);

	CC_SYNTHESIZE(int, _oldColorType, OldColorType);

	void startTingle();
	void removeEffect2();

	void stopNormalAction();

	void tingle(float dt);
    void playBossEffectOnce(int type,float delay=0.1f);
    void bossEffectOnce(Node *node);

private:
    static std::string getStringByType(PAOPAO_TYPE type);
	int _row;
	int _col;
	Sprite *_extraEffectSprite;
	int _inkEffectRange;
	Label *_textLabel;
	BubbleEffect _attachAttribute;
	PAOPAO_TYPE _colorType;

	Action *_normalAction;
};

#endif


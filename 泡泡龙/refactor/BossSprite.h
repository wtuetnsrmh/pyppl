#ifndef __BossSprite_H__
#define __BossSprite_H__
#include "GameEnum.h"
#include "cocos2d.h"
using namespace cocos2d;

class BossSprite :
	public Sprite
{
public:
	BossSprite(void);
	virtual ~BossSprite(void);

	CREATE_FUNC(BossSprite);

	static BossSprite * initWithType(BOSS_TYPE type);
	
	
	CC_SYNTHESIZE(int, _currentBoss, CurrentBoss);

	CC_SYNTHESIZE(Layer *, _inWhere, InWhere);
	void bossShow(Layer *inWhere);

	static void preloadAnimation();
private:
	void playWarning();
	void stopWarningAndShowBoss(Node *node);
	void playAnimation();
	void stopAnimation(Node *node);
	void tuMozhi();
	void doTuMozhi(Node *node);

	void callMozhiEffect(Node *node);
};

#endif // !__BossSprite_H__

#ifndef __EffectSprite_H__
#define __EffectSprite_H__

#include "cocos2d.h"
using namespace cocos2d;

class EffectSprite :
	public Sprite
{
public:
	EffectSprite(void);
	virtual ~EffectSprite(void);

	CREATE_FUNC(EffectSprite);

	static EffectSprite *initWithType(int type);

	void playEffectOnce(float delay=0.06f);
	void playEffectForEver(float delay=0.06f);
	CC_SYNTHESIZE(int, _type, Type);
private:
	std::string getStringByType(int type);
	
};

#endif
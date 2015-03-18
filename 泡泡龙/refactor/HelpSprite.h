#ifndef __HelpSprite_H__
#define __HelpSprite_H__
#include "cocos2d.h"
using namespace cocos2d;

class HelpSprite :
	public Sprite
{
public:
	HelpSprite(void);
	virtual ~HelpSprite(void);

	CREATE_FUNC(HelpSprite);
	void initWithType(int type);
	CC_SYNTHESIZE(int, _helpType, HelpType);
	void changeType(int type);
private:
	Sprite *_sp;
};

#endif
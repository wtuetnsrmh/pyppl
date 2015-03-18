#ifndef __GunSprite_H__
#define __GunSprite_H__
#include "cocos2d.h"
#include <string.h>
using namespace cocos2d;
class GunSprite :
	public Sprite
{
public:
	GunSprite(void);
	virtual ~GunSprite(void);

	CREATE_FUNC(GunSprite);
	static GunSprite * initWithPic(std::string pic );
	void startAction();
private:
	Animation* _normalAnimation;
};

#endif


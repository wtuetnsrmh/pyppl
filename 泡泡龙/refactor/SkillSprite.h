#ifndef __SkillSpirte_H__
#define __SkillSpirte_H__

#include "cocos2d.h"

using namespace cocos2d;

class SkillSprite: 
	public Sprite
{
public:
	SkillSprite(void);
	virtual ~SkillSprite(void);

	CREATE_FUNC(SkillSprite);

	static SkillSprite * initWithType(int type);

	CC_SYNTHESIZE(int, _skillType, SkillType); 

};

#endif
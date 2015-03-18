#ifndef __SkillStateBg_H__
#define  __SkillStateBg_H__
#include "cocos2d.h"
using namespace cocos2d;

class SkillStateBg: public
	Sprite
{
public:
	SkillStateBg();
	~SkillStateBg();

	CREATE_FUNC(SkillStateBg);

	static SkillStateBg * initWithState(int state);

	CC_SYNTHESIZE(int, _bgState, BgState);

	void changeState(int state);

private:

};

#endif
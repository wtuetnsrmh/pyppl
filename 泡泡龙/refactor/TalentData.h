#ifndef __Talent_H__
#define __Talent_H__
#include "cocos2d.h"

class TalentData
{
public:
	TalentData(void);
	virtual ~TalentData(void);

	CC_SYNTHESIZE(int, _talentType, TalentType);

	int getTalentLevel();
	void setTalentLevel(int level);
private:
	int _talentLevel;

};
#endif
#ifndef __TalentMgr_H__
#define __TalentMgr_H__
#include "cocos2d.h"
#include "GameEnum.h"

class TalentData;

struct TalentCost
{
	int type;
	int level;
	int cost;
};

struct LevelEffect
{
	int type;
	int level;
	int effectValue;
};

class TalentMgr
{
public:
	TalentMgr();
	~TalentMgr();

	void addTalent(int type, int level);

	void initTalentData(int tt[TT_END]);
	TalentData *getTalent(int type);
	int upgradeTalent(int type);
    int getCost(int type, int level);
	int getEffectValue(int type, int level);
	int getCurrentEffectValue(int type);
    void restTalentTest();
	int getCurrentTalentLevel(int type);
private:
	void initCostData();
	

	std::map<int, TalentData*> _talentDataMap;
	std::vector<TalentCost> _costList;
	std::vector<LevelEffect> _effectValueList;
};



#endif // !__TalentMgr_H__

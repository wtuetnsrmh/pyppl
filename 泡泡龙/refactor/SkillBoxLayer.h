#ifndef __SkillBoxLayer_H__
#define __SkillBoxLayer_H__

#include "cocos2d.h"
using namespace cocos2d;

#define SKILL_STATE 1000
class SkillSprite;
class SkillStateBg;

class SkillBoxLayer :
	public Layer
{
public:
	SkillBoxLayer(void);
	~SkillBoxLayer(void);
	virtual bool init();  
	CREATE_FUNC(SkillBoxLayer);  

	//void addSkill(int type);
	void addSkill(std::list<int> typeList);
	bool hasSkill();
	int useSkill();
	bool hasColumn();

	void doSkillMove();
	
	void updateSkillBgState();
private:
	std::list<SkillSprite*> _skillList;

	std::vector<SkillStateBg *> _skillBgStateList;

};
#endif
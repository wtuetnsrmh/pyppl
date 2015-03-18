#ifndef __TalentItemLayer_H__
#define __TalentItemLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;


class TalentItemLayer :
	public Layer
{
public:
	TalentItemLayer(void);
	~TalentItemLayer(void);

	virtual bool init();  
	CREATE_FUNC(TalentItemLayer);

	void initWithType(int type);
	void upgradeButtonTouch(Sprite* pSender,TouchEventType type);
	CC_SYNTHESIZE(int, _talentType, TalentType);

	void updateLevelShow(int level);
    void updateGoldByLevel(int gold);
	void updateItem(int idx);
    void reset();

private:	
    Label *_goldlabel;
	Sprite *_talentImg;
	Label *_talentName;
    ui::Button *upgradeBtn;
};
#endif
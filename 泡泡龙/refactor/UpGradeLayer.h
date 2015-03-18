#ifndef __UpGradeLayer_H__
#define __UpGradeLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "TalentView.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;


class UpGradeLayer :
	public Layer
{
public:
	UpGradeLayer(void);
	~UpGradeLayer(void);

	virtual bool init();  
	CREATE_FUNC(UpGradeLayer);
	void updateGold(int gold);
	void updateGem(int gem);

	void touchEvent(Sprite* pSender,TouchEventType type);
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
private:
	Label *_goldLabel;
	Label *_gemLabel;
    TalentView *_talentView;
};
#endif
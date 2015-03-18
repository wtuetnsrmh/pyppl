#ifndef __PowerUpsUI_H__
#define __PowerUpsUI_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "GameEnum.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;
using namespace std;

class PowerUpsView;

class PowerUpsUI :
	public Layer
{
public:
	PowerUpsUI(void);
	~PowerUpsUI(void);

	virtual bool init();  
	CREATE_FUNC(PowerUpsUI);
	void setNowPower(Talent_Type type);
	Talent_Type getNowPower();
	void updateNowPower(Talent_Type type,std::string name,std::string des, int itemIndex);
	
	void touchEvent(Sprite* pSender,TouchEventType type);
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;

public:
	Sprite *_nowIcon;
	Label *_nowName;
	Label *_nowDes;
private:
	Talent_Type _nowPower;
	PowerUpsView *_tableView;
};
#endif
#ifndef __PowerLayer_H__
#define __PowerLayer_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "GameEnum.h"
using namespace cocos2d::ui;
using namespace cocos2d;
using namespace cocostudio;


class PowerLayer : public Layer
{
public:
	PowerLayer(void);
	~PowerLayer(void);
	virtual bool init(); 
	CREATE_FUNC(PowerLayer);
    void touchEvent(Sprite* pSender,TouchEventType type);

	void changePowerBar(float num);
	void changePowerIcon(Talent_Type type);
	void powerEffect();
	void setPowericonEffect();//能量条满，可点击
    void reset();

	virtual void onEnter();
	virtual void onExit();
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
private:
	ProgressTimer *_power;
	Label *_powerLabel;
	Talent_Type _powerType;
	Button *powericon;

	bool _isPower;
	Sprite *_effectSprite;

};

#endif
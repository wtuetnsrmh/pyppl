#ifndef __CoinShopUI_H__
#define __CoinShopUI_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

class CoinShopUI :
	public Layer
{
public:
	CoinShopUI(void);
	~CoinShopUI(void);

	bool init();  
	CREATE_FUNC(CoinShopUI);
	void updateGold(int gold);
	void updateGem(int gem);

	void touchButtonEvent(Sprite* pSender,TouchEventType type);
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
private:
	Label *_goldLabel;
	Label *_gemLabel;
};
#endif
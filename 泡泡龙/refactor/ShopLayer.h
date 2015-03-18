#ifndef __ShopLayer_H__
#define __ShopLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

class ShopGoodsSprite;

class ShopLayer :
	public Layer
{
public:
	ShopLayer(void);
	~ShopLayer(void);

	virtual bool init();  
	CREATE_FUNC(ShopLayer);
	void updateGold(int gold);

	void touchButtonEvent(Sprite* pSender,TouchEventType type);
	void ChangePage(int index);

	virtual void onEnter();
	virtual void onExit();


	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;

private:
	Label *_goldLabel;
	ShopGoodsSprite *_goodsImg;
	int _index;

    cocos2d::Point _touchBeganPoint;
	cocos2d::Point _touchMovePoint;
	cocos2d::Point _touchEndPoint;

};
#endif
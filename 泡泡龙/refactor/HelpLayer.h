#ifndef __HelpLayer_H__
#define __HelpLayer_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;


class HelpSprite;

class HelpLayer :
	public Layer
{
public:
	HelpLayer(void);
	~HelpLayer(void);
	virtual bool init();  
	CREATE_FUNC(HelpLayer);  
	void touchEvent(Sprite* pSender,TouchEventType type);
	void changePageControlSprite();

	virtual void onEnter();
	virtual void onExit();


	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
private:
	int  _type;
	HelpSprite *_helpSp;
	int _curPage;

	Point _touchBeganPoint;
	Point _touchMovePoint;
	Point _touchEndPoint;
    LayerColor *lc;
};

#endif
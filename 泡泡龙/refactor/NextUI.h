#ifndef __NextUI_H__
#define __NextUI_H__

#include "cocos2d.h"

#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;
class NextUI :
	public Layer
{
public:
	NextUI(void);
	~NextUI(void);
	virtual bool init();  
	CREATE_FUNC(NextUI);  
	void touchEvent(Sprite*psender,TouchEventType type);
    
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
};

#endif

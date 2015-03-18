#ifndef __MenuLayer_H__
#define __MenuLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;
class MenuLayer :
	public Layer
{
public:
	MenuLayer(void);
	~MenuLayer(void);
	virtual bool init();  
	CREATE_FUNC(MenuLayer);  
	void initChapter();
	virtual void touchEvent(Sprite* pSender,TouchEventType type);
	void doStart(Node* node);
};

#endif


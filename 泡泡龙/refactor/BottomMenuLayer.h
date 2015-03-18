#ifndef __BottomMenuLayer_H__
#define __BottomMenuLayer_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocos2d;


class BottomMenuLayer : public Layer
{
public:
	BottomMenuLayer(void);
	~BottomMenuLayer(void);
	virtual bool init(); 
	CREATE_FUNC(BottomMenuLayer);
    void touchEvent(Sprite* pSender,TouchEventType type);

    void openMenu();
private:
    Button *_menuBtn;
    Button *soundBtn ;
    bool _isSound;
};

#endif

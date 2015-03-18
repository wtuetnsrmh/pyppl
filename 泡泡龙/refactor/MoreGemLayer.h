//
//  MoreGemLayer.h
//  bubble2
//
//  Created by MacMini on 14-7-24.
//
//

#ifndef __bubble2__MoreGemLayer__
#define __bubble2__MoreGemLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocos2d;

class MoreGemLayer :
public Layer
{
public:
	MoreGemLayer(void);
	~MoreGemLayer(void);
	virtual bool init();
	CREATE_FUNC(MoreGemLayer);
    void moreGem();
    
    void touchEvent(Sprite* pSender,TouchEventType type);
};


#endif /* defined(__bubble2__MoreGemLayer__) */

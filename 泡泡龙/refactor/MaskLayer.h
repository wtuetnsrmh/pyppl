//
//  MaskLayer.h
//  bubble2
//
//  Created by MacMini on 14-7-16.
//
//

#ifndef bubble2_MaskLayer_h
#define bubble2_MaskLayer_h
#include "cocos2d.h"
using namespace cocos2d;

class MaskLayer :
public Layer
{
public:
	MaskLayer(void);
	~MaskLayer(void);
	virtual bool init();
	CREATE_FUNC(MaskLayer);
    
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
};

#endif

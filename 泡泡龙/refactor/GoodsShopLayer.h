#ifndef __GoodsShopLayer_H__
#define __GoodsShopLayer_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

class MoreGemLayer;

class GoodsShopLayer : public Layer
{
public:
	GoodsShopLayer(void);
	~GoodsShopLayer(void);
	virtual bool init(); 
	CREATE_FUNC(GoodsShopLayer);

	void touchEvent(Sprite* pSender,TouchEventType type);
	void updateGem(int gem);
	void initItem(bool disabel);
    void moreGem();
    MoreGemLayer *more;
    
    
    virtual void onEnter();
	virtual void onExit();
    
    
	virtual bool onTouchBegan (Touch *touch, Event *unused_event) override;
	virtual void onTouchMoved (Touch *touch, Event *unused_event) override;
	virtual void onTouchEnded (Touch *touch, Event *unused_event) override;
    LayerColor *lc;

private:
	Label *_gemLabel;
};

#endif

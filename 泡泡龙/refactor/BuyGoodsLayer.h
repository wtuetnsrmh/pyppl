#ifndef __BuyGoodsLayer_H__
#define __BuyGoodsLayer_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocos2d;


class BuyGoodsLayer : public Layer
{
public:
	BuyGoodsLayer(void);
	~BuyGoodsLayer(void);
	virtual bool init(); 
	CREATE_FUNC(BuyGoodsLayer);

	void touchEvent(Sprite* pSender,TouchEventType type);
private:
	Label *_gemLabel;
};

#endif

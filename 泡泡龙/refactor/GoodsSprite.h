#ifndef __GoodsSprite_H__
#define __GoodsSprite_H__
#include "cocos2d.h"

#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocos2d;

class GoodsSprite: 
	public Layer  
{
public:
	GoodsSprite(void);
	virtual ~GoodsSprite(void);
	virtual bool init(); 
	CREATE_FUNC(GoodsSprite);

	//virtual bool init();  

	void initWithType(int type);

	CC_SYNTHESIZE(int, _goodsType, GoodsType);

	int getGoodsCount();
	void setGoodsCount(int count);

	void touchEvent(Sprite* pSender,TouchEventType type);

	void enableButton();
	void disableButton();

	void updateCountShow(int count);


private:
	Button *_enableButton;
	Sprite *_disableButton;
	Label *_countLabel;
	int _goodsCount;

};
#endif // !__GoodsSprite_H__

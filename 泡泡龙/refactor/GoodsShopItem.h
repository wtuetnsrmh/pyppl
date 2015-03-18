#ifndef __GoodsShopItem_H__
#define __GoodsShopItem_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocos2d;


class GoodsShopItem : public Layer
{
public:
	GoodsShopItem(void);
	~GoodsShopItem(void);
	virtual bool init(); 
	CREATE_FUNC(GoodsShopItem);
	CC_SYNTHESIZE(int, _goodsType, GoodsType);
	void initWithType(int type);
	void touchEvent(Sprite* pSender,TouchEventType type);

	void addShizhong(Node *node);
	void initGem(int type);
	void changeGemSp(int count,bool isChange);
	void restoreBarrier(Node *node);
	void doAction(Node*node);
	void removeThis(Node*node);
	void disabelItem(bool disabel);
private:
	Label *_gemLabel;
	Sprite *_goodsName;
	Sprite *_gemSp1;
	Sprite *_gemSp2;
	int _currentType;
	Sprite *gray ;
	Button *btn;
};

#endif
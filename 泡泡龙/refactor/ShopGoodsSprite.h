#ifndef __ShopGoodsSprite_H__
#define __ShopGoodsSprite_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class ShopGoodsSprite : public Layer
{
public:
	ShopGoodsSprite(void);
	virtual ~ShopGoodsSprite(void);
	virtual bool init(); 
	CREATE_FUNC(ShopGoodsSprite);

	void initWithType(int type);

	CC_SYNTHESIZE(int, _shopGoodsType, ShopGoodsType);
	void changeType(int type);

private:
	int _type;
	Sprite *goods;
	Sprite *goodsName;
};

#endif 

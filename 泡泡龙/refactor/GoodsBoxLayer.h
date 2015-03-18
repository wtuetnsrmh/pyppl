#ifndef __GoodsBoxLayer_H__
#define __GoodsBoxLayer_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

class GoodsSprite;

class GoodsBoxLayer:
	public Layer
{
public:
	GoodsBoxLayer(void);
	~GoodsBoxLayer(void);
	virtual bool init();  
	CREATE_FUNC(GoodsBoxLayer); 
	void touchEvent(Sprite*psender,TouchEventType type);

	//virtual void onEnter() override;

	void updateGoods(int type, int changeCount);
private:
	std::map<int, GoodsSprite*> _goodsMap;
};

#endif // !__GoodsBoxLayer_H__

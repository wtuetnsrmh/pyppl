#ifndef __PowerUpsItem_H__
#define __PowerUpsItem_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "GameEnum.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;


class PowerUpsItem :
	public Layer
{
public:
	PowerUpsItem(void);
	~PowerUpsItem(void);

	virtual bool init();  
	CREATE_FUNC(PowerUpsItem);

	void initWithType(int type);
	void touchEvent(Sprite* pSender,TouchEventType type);
	Talent_Type getPowerType();
	void setPowerType(Talent_Type type);
	void updateItemState(int state);
	void changeState(int state);
private:
	Sprite *_itemBg;
	Sprite *_iconImg;
	Label  *_nameLabel;
	Label  *_desLabel;
	Button *_chooseBtn;
	Talent_Type _powerType;
	int _state;
	int _itemIndex;
};
#endif
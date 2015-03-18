#ifndef __CoinShopItem_H__
#define __CoinShopItem_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

class CoinShopItem :
	public Layer
{
public:
	CoinShopItem(void);
	~CoinShopItem(void);

	bool init();  
	CREATE_FUNC(CoinShopItem);
	CC_SYNTHESIZE(int, _coinType, CoinType);

	void initWithType(int type);
	void updateItem(int type);
	

	void touchButtonEvent(Sprite* pSender,TouchEventType type);

	void setCoin(int coin,float money);

	void changeNumber(Sprite *sp, int num);
	void addCoin(int coin);

	void initCost(int type);
	void addGold();
	void addGem();
private:
	//Sprite **_coinNumbers;
	Button *buyBtn;
	Sprite* _iconImg;
	//Sprite*_coinImg;
	//cocos2d::Point _coinPo;
    Label *_moneyLabel;
    Label *_coinsLabel;

};
#endif
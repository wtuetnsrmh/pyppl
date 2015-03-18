#ifndef __CoinShopMgr_H__
#define __CoinShopMgr_H__
#include "cocos2d.h"
#include "GameEnum.h"

struct CoinsCost
{
	int type;
	int coins;
	float cost;
};
class CoinShopMgr
{
public:
	CoinShopMgr();
	~CoinShopMgr();

	void initCost();
	float getCost(int type);
	int getCoin(int type);
private:

	std::vector<CoinsCost> _coinsList;
};



#endif
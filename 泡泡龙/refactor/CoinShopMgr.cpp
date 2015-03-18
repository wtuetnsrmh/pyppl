#include "CoinShopMgr.h"

CoinShopMgr::CoinShopMgr()
{
}

CoinShopMgr::~CoinShopMgr()
{
}
void CoinShopMgr::initCost()
{
	CoinsCost c0 = {0, 0, 4.99};
	_coinsList.push_back(c0);
	CoinsCost c1 = {1, 5000, 0.99};
	_coinsList.push_back(c1);
	CoinsCost c2 = {2, 50000, 4.99};
	_coinsList.push_back(c2);
	CoinsCost c3 = {3, 150000, 9.99};
	_coinsList.push_back(c3);
	CoinsCost c4 = {4, 400000, 19.99};
	_coinsList.push_back(c4);
	CoinsCost c5 = {5, 5, 0.99};
	_coinsList.push_back(c5);
	CoinsCost c6 = {6, 50, 4.99};
	_coinsList.push_back(c6);
	CoinsCost c7 = {7, 150, 9.99};
	_coinsList.push_back(c7);
	CoinsCost c8 = {8, 500, 19.99};
	_coinsList.push_back(c8);
}
float  CoinShopMgr::getCost(int type)
{
	for (std::vector<CoinsCost>::iterator iter=_coinsList.begin(); iter!=_coinsList.end(); ++iter)
	{
		CoinsCost &tc = (*iter);
		if(tc.type==type)
			return tc.cost;
	}
	return 0;
}
int  CoinShopMgr::getCoin(int type)
{
	for (std::vector<CoinsCost>::iterator iter=_coinsList.begin(); iter!=_coinsList.end(); ++iter)
	{
		CoinsCost &tc = (*iter);
		if(tc.type==type)
			return tc.coins;
	}
	return -1;
}
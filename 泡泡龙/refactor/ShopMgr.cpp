#include "ShopMgr.h"
#include "Global.h"
#include "MoneyCostDef.h"
ShopMgr::ShopMgr()
{
}

ShopMgr::~ShopMgr()
{
}


void ShopMgr::buyGoods(int  count)
{
	//int goldNeed = GOODS_COST_MAP[type];

	//int count = 1;

	//if (g_goodsData->gold < goldNeed)
	//{
	//	g_miscLayer->showAlert(NO_GOLD);
	//	return;
	//}
	//g_goodsData->gold -= goldNeed;

	//g_gameLayer->saveGold2File(g_goodsData->gold);

	//g_goodsBoxLayer->updateGoods(type, count);

	//g_miscLayer->showAlert(BUY_SUCCESS);



	if (g_goodsData->gem < count)
	{
		//g_miscLayer->showAlert(NO_GOLD);
		return;
	}
	g_goodsData->gem -= count;
    g_gameLayer->saveGold2File(g_goodsData->gem);
	g_goodsShopLayer->updateGem(g_goodsData->gem);



	g_miscLayer->showAlert(BUY_SUCCESS);





}
void ShopMgr::upgradeTalent(int type)
{

}
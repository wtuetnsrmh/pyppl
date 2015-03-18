#ifndef __MoneyConstDef_H__
#define __MoneyConstDef_H__
#include "GameEnum.h"
#include <map>

const std::map<int, int>::value_type goods_cost_value[] =
{
	
	std::map<int, int>::value_type(GT_Earthquake, 20),
	std::map<int, int>::value_type(GT_EnergyInjection, 10),
	std::map<int, int>::value_type(GT_Laser, 30),
	std::map<int, int>::value_type(GT_Timelaspe, 40)
};
static std::map<int, int> GOODS_COST_MAP(goods_cost_value, goods_cost_value+4); //道具cost表


#endif
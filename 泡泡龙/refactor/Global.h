#ifndef __Global_H__
#define __Global_H__

#include "GameLayer.h"
#include "ShotLayer.h"
#include "ScoreLayer.h"
#include "MiscLayer.h"
#include "GameGoodsData.h"
#include "SkillBoxLayer.h"
#include "TalentMgr.h"
#include "GoodsBoxLayer.h"
#include "ShopMgr.h"
#include "MenuLayer.h"
#include "PowerLayer.h"
#include "PowerUpsUI.h"
#include "FireLayer.h"
#include "UpGradeLayer.h"
#include "CoinShopMgr.h"
#include "GoodsShopLayer.h"
#include "CoinShopUI.h"


extern int PAOPAO_SIZE;
extern float PIC_RADIO;
extern int g_recordScore;

extern GameLayer *g_gameLayer;
extern ShotLayer *g_shotLayer;
extern ScoreLayer * g_scoreLayer;
extern MiscLayer *g_miscLayer;
extern SkillBoxLayer *g_skillBoxLayer;

extern int version;
extern GameGoodsData * g_goodsData;
extern GoodsBoxLayer * g_goodsBoxLayer;

extern TalentMgr *g_talentMgr;

extern ShopMgr *g_shopMgr;
extern MenuLayer *g_menuLayer;
extern PowerLayer *g_powerLayer;
extern PowerUpsUI *g_powerupsUI;
extern FireLayer *g_fireLayer;
extern UpGradeLayer *g_upGradeLayer;

extern CoinShopMgr *g_coinShopMgr;
extern GoodsShopLayer*g_goodsShopLayer;
extern CoinShopUI*g_coinShopUI;

extern int g_guide;

#endif
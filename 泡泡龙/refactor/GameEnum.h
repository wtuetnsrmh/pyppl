#ifndef __GameEnum_H__
#define __GameEnum_H__
  
enum PAOPAO_TYPE{  
    PAOPAO_TYPE_UNKNOW,  
    PAOPAO_TYPE1,  
    PAOPAO_TYPE2,  
    PAOPAO_TYPE3,  
    PAOPAO_TYPE4,  
    PAOPAO_TYPE5,  
    PAOPAO_TYPE6,  
	PAOPAO_BOMB, //炸弹
	PAOPAO_COLOR, //彩球
	PAOPAO_PLAGUE, //瘟疫
	PAOPAO_GUARD, //护卫
	PAOPAO_INK, //墨汁泡泡
	PAOPAO_PASS, //穿透
	PAOPAO_STONE, //石头
    PAOPAO_BLACK=101,  //黑色
    //PAOPAO_STONE,  //石头
	PAOPAO_HEART = 103,
	
	PAOPAO_SPECIAL, //以下都是特殊泡泡
};  

enum PAOPAO_SPECIAL_ABILITY
{
	SPECIAL_ABILITY_UNKNOW,
	SPECIAL_ABILITY_GUARD, //护卫
	SPECIAL_ABILITY_PLAGUE, //瘟疫
};


enum BOSS_TYPE{  
	BOSS_TYPE0,  
	BOSS_TYPE1, 
};  

enum  Bubble_State{
	Bubble_S_Normal,
	Bubble_S_Dirty, //脏数据

};
  
enum Game_State{
	G_START,
	G_OVER,
};


enum Goods_Type{
	//GT_AssistLine = 1,
	GT_Earthquake,
	//GT_ResetHeart,
	//GT_ChangeShottingBubbleType,
	GT_EnergyInjection, //能量翻倍
	GT_Laser, //激光
	GT_Timelaspe, //时光倒流
};

/*enum Talent_Type{
	TT_Skill_Column, //技能栏
	TT_Add_Random_Count, //增加随机球个数
	TT_AssistLine,
	TT_Earthquake,
	TT_HeartFreeze, //冻住红心
	TT_Power, //能力增长速度
	TT_7,
};*/

enum Talent_Type{
	TT_Skill_Column, //技能栏
	TT_Skill_XX, //组合技能
	TT_AssistLine,
	TT_HeartFreeze, //冻住红心
	TT_Electric, //电击
	TT_FireShot, //开火
	TT_ScoreMulti,
	TT_GoldMulti,
	TT_BossHurt_Electric,
	TT_BossHurt_Clear,
	TT_BossHurt_Bump,
	TT_Power, //能力增长速度
	TT_GrassBubble,//增益球开放
	TT_GrassBubbleSpawn,//增益球概率
	TT_END,
};
enum GrassBubble_Type{
	GBT_AssistLine=1,
	GBT_HeartFreeze, //冻住红心
	GBT_Electric, //电击
	GBT_FireShot, //开火
	GBT_Earthquake,
	GBT_EnergyInjection, //能量翻倍
	GBT_Laser, //激光
	GBT_Timelaspe, //时光倒流
	GBT_OneGem,//1宝石
	GBT_TwoGem,
	GBT_OneHundredGold,//100金币
	GBT_TwoHundredGold,//200金币
	GBT_Pass_Type1,
	GBT_Pass_Type2,
	GBT_Pass_Type3,
	GBT_Pass_Type4,
	GBT_Pass_Type5,
	GBT_Pass_Type6,
};

#endif  
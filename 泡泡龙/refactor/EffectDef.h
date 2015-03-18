#ifndef __EffectDef_H__
#define __EffectDef_H__

#define PP_A_WARNING 1000

#define BOSS_A_WARNING 2000
#define BOSS_A_ZY 3000

enum BubbleEffect
{
	B_UNKNOWN,
	B_Drop_Broken,
	B_Bumb_Broken,
	B_Bumb_Red,
	B_Bumb_Ready,
	B_Plague, //瘟疫
	B_Ink, //墨汁
	B_Blood, //流血
	B_Warning, //警告
	B_BossDie,

	B_Attribute_AssistLine=100, //额外奖励,辅助射线
	B_Attribute_ResetHeart=101, //重置生命
	B_Attribute_EarthQuake=102, //地震
	B_Attribute_ChangeType=103, //额外奖励,自动将场上数量最少的精灵种类

	B_Laster=200, // 200以上有20张图片
};
#endif
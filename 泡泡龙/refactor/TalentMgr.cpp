#include "TalentMgr.h"
#include "TalentData.h"
#include "GameEnum.h"
#include "FileUtil.h"
#include "Global.h"
#include "SkillBoxLayer.h"
#include "math.h"
TalentMgr::TalentMgr()
{
}

TalentMgr::~TalentMgr()
{
}


void TalentMgr::addTalent(int type, int level)
{
	if(_talentDataMap.find(type) != _talentDataMap.end()){
		_talentDataMap[type]->setTalentLevel(level);
	}
	else{
		TalentData *td = new TalentData;
		td->setTalentType(type);
		_talentDataMap[type] = td;
	}
}

TalentData *TalentMgr::getTalent(int type)
{

	return _talentDataMap[type];
}


void TalentMgr::initTalentData(int tt[TT_END])
{
	for(int i=TT_Skill_Column; i<TT_END; i++)
	{
		TalentData *td = new TalentData;
		td->setTalentType(i);
		td->setTalentLevel(tt[i]);

		_talentDataMap[i] = td;
	}

	initCostData();
}

void TalentMgr::initCostData()
{
	
		//升级金币
		TalentCost tc00= {0, 0, 0};
		_costList.push_back(tc00);
		TalentCost tc01= {0, 1, 1000};
		_costList.push_back(tc01);
		TalentCost tc02= {0, 2, 2500};
		_costList.push_back(tc02);
		TalentCost tc03= {0, 3, 5000};
		_costList.push_back(tc03);
		TalentCost tc04= {0, 4, 10000};
		_costList.push_back(tc04);

		TalentCost tc10= {1, 0, 0};
		_costList.push_back(tc10);
		TalentCost tc11= {1, 1, 5000};
		_costList.push_back(tc11);
		TalentCost tc12= {1, 2, 10000};
		_costList.push_back(tc12);
		TalentCost tc13= {1, 3, 25000};
		_costList.push_back(tc13);
		TalentCost tc14= {1, 4, 50000};
		_costList.push_back(tc14);

		TalentCost tc20= {2, 0, 0};
		_costList.push_back(tc20);
		TalentCost tc21= {2, 1, 250};
		_costList.push_back(tc21);
		TalentCost tc22= {2, 2, 500};
		_costList.push_back(tc22);
		TalentCost tc23= {2, 3, 1000};
		_costList.push_back(tc23);
		TalentCost tc24= {2, 4, 2500};
		_costList.push_back(tc24);

		TalentCost tc30= {3, 0, 0};
		_costList.push_back(tc30);
		TalentCost tc31= {3, 1, 1000};
		_costList.push_back(tc31);
		TalentCost tc32= {3, 2, 2500};
		_costList.push_back(tc32);
		TalentCost tc33= {3, 3, 5000};
		_costList.push_back(tc33);
		TalentCost tc34= {3, 4, 10000};
		_costList.push_back(tc34);

		TalentCost tc40= {4, 0, 0};
		_costList.push_back(tc40);
		TalentCost tc41= {4, 1, 2500};
		_costList.push_back(tc41);
		TalentCost tc42= {4, 2, 5000};
		_costList.push_back(tc42);
		TalentCost tc43= {4, 3, 10000};
		_costList.push_back(tc43);
		TalentCost tc44= {4, 4, 25000};
		_costList.push_back(tc44);

		TalentCost tc50= {5, 0, 0};
		_costList.push_back(tc50);
		TalentCost tc51= {5, 1, 2500};
		_costList.push_back(tc51);
		TalentCost tc52= {5, 2, 5000};
		_costList.push_back(tc52);
		TalentCost tc53= {5, 3, 10000};
		_costList.push_back(tc53);
		TalentCost tc54= {5, 4, 25000};
		_costList.push_back(tc54);

		TalentCost tc60= {6, 0, 0};
		_costList.push_back(tc60);
		TalentCost tc61= {6, 1, 2500};
		_costList.push_back(tc61);
		TalentCost tc62= {6, 2, 5000};
		_costList.push_back(tc62);
		TalentCost tc63= {6, 3, 10000};
		_costList.push_back(tc63);
		TalentCost tc64= {6, 4, 25000};
		_costList.push_back(tc64);

		TalentCost tc70= {7, 0, 0};
		_costList.push_back(tc70);
		TalentCost tc71= {7, 1, 2500};
		_costList.push_back(tc71);
		TalentCost tc72= {7, 2, 5000};
		_costList.push_back(tc72);
		TalentCost tc73= {7, 3, 10000};
		_costList.push_back(tc73);
		TalentCost tc74= {7, 4, 25000};
		_costList.push_back(tc74);

		TalentCost tc80= {8, 0, 0};
		_costList.push_back(tc80);
		TalentCost tc81= {8, 1, 1000};
		_costList.push_back(tc81);
		TalentCost tc82= {8, 2, 2500};
		_costList.push_back(tc82);
		TalentCost tc83= {8, 3, 5000};
		_costList.push_back(tc83);
		TalentCost tc84= {8, 4, 10000};
		_costList.push_back(tc84);

		TalentCost tc90= {9, 0, 0};
		_costList.push_back(tc90);
		TalentCost tc91= {9, 1, 1000};
		_costList.push_back(tc91);
		TalentCost tc92= {9, 2, 2500};
		_costList.push_back(tc92);
		TalentCost tc93= {9, 3, 5000};
		_costList.push_back(tc93);
		TalentCost tc94= {9, 4, 10000};
		_costList.push_back(tc94);

		TalentCost tc100= {10, 0, 0};
		_costList.push_back(tc100);
		TalentCost tc101= {10, 1, 1000};
		_costList.push_back(tc101);
		TalentCost tc102= {10, 2, 2500};
		_costList.push_back(tc102);
		TalentCost tc103= {10, 3, 5000};
		_costList.push_back(tc103);
		TalentCost tc104= {10, 4, 10000};
		_costList.push_back(tc104);

		TalentCost tc110= {11, 0, 0};
		_costList.push_back(tc110);
		TalentCost tc111= {11, 1, 2500};
		_costList.push_back(tc111);
		TalentCost tc112= {11, 2, 5000};
		_costList.push_back(tc112);
		TalentCost tc113= {11, 3, 10000};
		_costList.push_back(tc113);
		TalentCost tc114= {11, 4, 25000};
		_costList.push_back(tc114);

		TalentCost tc115= {12, 0, 0};
		_costList.push_back(tc115);
		TalentCost tc116= {12, 1, 500};
		_costList.push_back(tc116);
		TalentCost tc117= {12, 2, 1000};
		_costList.push_back(tc117);
		TalentCost tc118= {12, 3, 2500};
		_costList.push_back(tc118);
		TalentCost tc119= {12, 4, 5000};
		_costList.push_back(tc119);

		TalentCost tc120= {13, 0, 0};
		_costList.push_back(tc120);
		TalentCost tc121= {13, 1, 1000};
		_costList.push_back(tc121);
		TalentCost tc122= {13, 2, 2500};
		_costList.push_back(tc122);
		TalentCost tc123= {13, 3, 5000};
		_costList.push_back(tc123);
		TalentCost tc124= {13, 4, 10000};
		_costList.push_back(tc124);

		//升级效果
		LevelEffect le00 = {0, 0, 0};
		_effectValueList.push_back(le00);
		LevelEffect le01 = {0, 1, 1};
		_effectValueList.push_back(le01);
		LevelEffect le02 = {0, 2, 2};
		_effectValueList.push_back(le02);
		LevelEffect le03 = {0, 3, 3};
		_effectValueList.push_back(le03);
		LevelEffect le04 = {0, 4, 4};
		_effectValueList.push_back(le04);

		LevelEffect le10 = {1, 0, 0};
		_effectValueList.push_back(le10);
		LevelEffect le11 = {1, 1, 1};
		_effectValueList.push_back(le11);
		LevelEffect le12 = {1, 2, 2};
		_effectValueList.push_back(le12);
		LevelEffect le13 = {1, 3, 3};
		_effectValueList.push_back(le13);
		LevelEffect le14 = {1, 4, 4};
		_effectValueList.push_back(le14);

		LevelEffect le20 = {2, 0, 3};
		_effectValueList.push_back(le20);
		LevelEffect le21 = {2, 1, 5};
		_effectValueList.push_back(le21);
		LevelEffect le22 = {2, 2, 7};
		_effectValueList.push_back(le22);
		LevelEffect le23 = {2, 3, 9};
		_effectValueList.push_back(le23);
		LevelEffect le24 = {2, 4, 11};
		_effectValueList.push_back(le24);

		LevelEffect le30 = {3, 0, 3};
		_effectValueList.push_back(le30);
		LevelEffect le31 = {3, 1, 4};
		_effectValueList.push_back(le31);
		LevelEffect le32 = {3, 2, 5};
		_effectValueList.push_back(le32);
		LevelEffect le33 = {3, 3, 6};
		_effectValueList.push_back(le33);
		LevelEffect le34 = {3, 4, 7};
		_effectValueList.push_back(le34);

		LevelEffect le40 = {4, 0, 1};
		_effectValueList.push_back(le40);
		LevelEffect le41 = {4, 1, 2};
		_effectValueList.push_back(le41);
		LevelEffect le42 = {4, 2, 3};
		_effectValueList.push_back(le42);
		LevelEffect le43 = {4, 3, 4};
		_effectValueList.push_back(le43);
		LevelEffect le44 = {4, 4, 5};
		_effectValueList.push_back(le44);

		LevelEffect le50 = {5, 0, 20};
		_effectValueList.push_back(le50);
		LevelEffect le51 = {5, 1, 30};
		_effectValueList.push_back(le51);
		LevelEffect le52 = {5, 2, 40};
		_effectValueList.push_back(le52);
		LevelEffect le53 = {5, 3, 60};
		_effectValueList.push_back(le53);
		LevelEffect le54 = {5, 4, 80};
		_effectValueList.push_back(le54);


		LevelEffect le60 = {6, 0, 0};
		_effectValueList.push_back(le60);
		LevelEffect le61 = {6, 1, 1};
		_effectValueList.push_back(le61);
		LevelEffect le62 = {6, 2, 2};
		_effectValueList.push_back(le62);
		LevelEffect le63 = {6, 3, 4};
		_effectValueList.push_back(le63);
		LevelEffect le64 = {6, 4, 8};
		_effectValueList.push_back(le64);

		LevelEffect le70 = {7, 0, 1};
		_effectValueList.push_back(le70);
		LevelEffect le71 = {7, 1, 2};
		_effectValueList.push_back(le71);
		LevelEffect le72 = {7, 2, 3};
		_effectValueList.push_back(le72);
		LevelEffect le73 = {7, 3, 4};
		_effectValueList.push_back(le73);
		LevelEffect le74 = {7, 4, 5};
		_effectValueList.push_back(le74);

		LevelEffect le80 = {8, 0, 10};
		_effectValueList.push_back(le80);
		LevelEffect le81 = {8, 1, 20};
		_effectValueList.push_back(le81);
		LevelEffect le82 = {8, 2, 30};
		_effectValueList.push_back(le82);
		LevelEffect le83 = {8, 3, 40};
		_effectValueList.push_back(le83);
		LevelEffect le84 = {8, 4, 50};
		_effectValueList.push_back(le84);

		LevelEffect le90 = {9, 0, 10};
		_effectValueList.push_back(le90);
		LevelEffect le91 = {9, 1, 20};
		_effectValueList.push_back(le91);
		LevelEffect le92 = {9, 2, 30};
		_effectValueList.push_back(le92);
		LevelEffect le93 = {9, 3, 40};
		_effectValueList.push_back(le93);
		LevelEffect le94 = {9, 4, 50};
		_effectValueList.push_back(le94);

		LevelEffect le100 = {10, 0, 10};
		_effectValueList.push_back(le100);
		LevelEffect le101 = {10, 1, 20};
		_effectValueList.push_back(le101);
		LevelEffect le102 = {10, 2, 30};
		_effectValueList.push_back(le102);
		LevelEffect le103 = {10, 3, 40};
		_effectValueList.push_back(le103);
		LevelEffect le104 = {10, 4, 50};
		_effectValueList.push_back(le104);

		LevelEffect le110 = {11, 0, 100};
		_effectValueList.push_back(le110);
		LevelEffect le111 = {11, 1, 110};
		_effectValueList.push_back(le111);
		LevelEffect le112 = {11, 2, 120};
		_effectValueList.push_back(le112);
		LevelEffect le113 = {11, 3, 140};
		_effectValueList.push_back(le113);
		LevelEffect le114 = {11, 4, 180};
		_effectValueList.push_back(le114);

		LevelEffect le115 = {12, 0, 1};
		_effectValueList.push_back(le115);
		LevelEffect le116 = {12, 1, 2};
		_effectValueList.push_back(le116);
		LevelEffect le117 = {12, 2, 3};
		_effectValueList.push_back(le117);
		LevelEffect le118 = {12, 3, 4};
		_effectValueList.push_back(le118);
		LevelEffect le119 = {12, 4, 5};
		_effectValueList.push_back(le119);

		LevelEffect le120 = {13, 0, 0};
		_effectValueList.push_back(le120);
		LevelEffect le121 = {13, 1, 50};
		_effectValueList.push_back(le121);
		LevelEffect le122 = {13, 2, 100};
		_effectValueList.push_back(le122);
		LevelEffect le123 = {13, 3, 150};
		_effectValueList.push_back(le123);
		LevelEffect le124 = {13, 4, 200};
		_effectValueList.push_back(le124);
}

int TalentMgr::getEffectValue(int type, int level)
{
	for (std::vector<LevelEffect>::iterator iter=_effectValueList.begin(); iter!=_effectValueList.end(); ++iter)
	{
		LevelEffect &tc = (*iter);
		if(tc.level==level && tc.type==type)
			return tc.effectValue;
	}
	return 0;
}

int TalentMgr::getCurrentEffectValue(int type)
{

	int level = _talentDataMap[type]->getTalentLevel();

	return getEffectValue(type, level);
}
int TalentMgr::getCurrentTalentLevel(int type)
{
	int level = _talentDataMap[type]->getTalentLevel();
	return level;
}
int TalentMgr::getCost(int type, int level)
{
	for (std::vector<TalentCost>::iterator iter=_costList.begin(); iter!=_costList.end(); ++iter)
	{
		TalentCost &tc = (*iter);
		if(tc.level==level && tc.type==type)
			return tc.cost;
	}
	return 0;
}

int TalentMgr::upgradeTalent(int type)
{
	//判断费用
	
	int level = _talentDataMap[type]->getTalentLevel();

	if (level >= 4)
	{
		return level;
	}

	int cost = getCost(type, level+1);

	

	if (cost > g_goodsData->gold)
	{
		g_miscLayer->showAlert(NO_GOLD);
		return level;
	}

	g_miscLayer->showAlert(BUY_SUCCESS);

	g_goodsData->gold -= cost;

	g_gameLayer->saveGold2File(g_goodsData->gold);

	_talentDataMap[type]->setTalentLevel(level+1);


	int skillColumn=0, addRandomCount=0, assistLine=0, earthquake=0;
	int tt[TT_END];
	for(int i=0; i<TT_END; i++)
	{
		tt[i] =  _talentDataMap[i]->getTalentLevel();
	}
	//int skillColumn, int addRandomCount, int assistLine, int earthquake
	FileUtil::writeTalentData(tt);
	
	if(type == TT_Skill_Column){
		g_skillBoxLayer->updateSkillBgState();
	}

	return level + 1;
}
void TalentMgr::restTalentTest()
{
    int tt[TT_END];
	for(int i=0; i<TT_END; i++)
	{
        _talentDataMap[i]->setTalentLevel(0);
		tt[i] = 0;
	}
    FileUtil::writeTalentData(tt);
}

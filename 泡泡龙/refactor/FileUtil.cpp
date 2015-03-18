#include "FileUtil.h"
#include <iostream>
#include <fstream>
#include "cocos2d.h"
#include "BarrierData.h"
#include "Global.h"
#include "TalentMgr.h"

using namespace std;
using namespace cocos2d;

std::vector<BarrierData *> FileUtil::barrierList;

FileUtil::FileUtil()
{
}

FileUtil::~FileUtil()
{
}

void FileUtil::writeBarrierProgress(int barrier)
{
	string filename = "progress.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ofstream o_file;
	o_file.open(path + filename);
	o_file << barrier ;
	o_file.close();
	//cout << path << endl;
}

void FileUtil::readBarrierProgress(int &barrier)
{
	string filename = "progress.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	 ifstream i_file;
	i_file.open(path + filename);
	if (i_file.is_open())
	{
		i_file >> barrier;
	}
	else{
		barrier = 1;
	}
	i_file.close();

}

void FileUtil::readGameGoods()
{
	if(g_goodsData == NULL)
		g_goodsData = new GameGoodsData;
	string filename = "goods.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ifstream i_file;
	i_file.open(path + filename);
	if (i_file.is_open())
	{
		
		i_file >> version;

		i_file >> g_goodsData->gold;
		i_file >> g_goodsData->gem;
		i_file >> g_goodsData->gEarthQuake;
		i_file >> g_goodsData->gEnergy;
		i_file >> g_goodsData->gLaser;
		
		i_file >> g_goodsData->gTimelaspe;
		

	}
	else{
		g_goodsData->gold = 0;
		g_goodsData->gem = 0;
		g_goodsData->gEarthQuake = 0;
		g_goodsData->gEnergy = 0;
		g_goodsData->gLaser = 0;
		
		g_goodsData->gTimelaspe = 0;
	}
	
	i_file.close();
}

void FileUtil::writeGameGoods()
{
	string filename = "goods.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ofstream o_file;
	o_file.open(path + filename);
	
	o_file << version << "\n";
	o_file << g_goodsData->gold << "\n";
	o_file << g_goodsData->gem << "\n";

	o_file << g_goodsData->gEarthQuake << "\n";
	o_file << g_goodsData->gEnergy << "\n";
	o_file << g_goodsData->gLaser << "\n";
	
	o_file << g_goodsData->gTimelaspe << "\n";

	o_file.close();
}


void FileUtil::readTalentData()
{
	string filename = "talent.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ifstream i_file;
	i_file.open(path + filename);


	int skillColumn=1, addRandomCount=1,  assistLine=1,  earthquake=1;
	int tt[TT_END] = {0};
	if (i_file.is_open())
	{
		i_file >> version;

		for(int i=0; i<TT_END; i++)
		{
			i_file >> tt[i];
		}
	}

	g_talentMgr = new TalentMgr;
	g_talentMgr->initTalentData(tt);

}

void FileUtil::writeTalentData(int tt[TT_END])
{
	string filename = "talent.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ofstream o_file;
	o_file.open(path + filename);

	o_file << version<< "\n";

	for(int i=0; i<TT_END; i++)
	{
		o_file << tt[i] << "\n";
	}
	o_file.close();
}


void FileUtil::readBarrierData()
{
	barrierList.clear();
	ValueVector txt_vec = FileUtils::getInstance()->getValueVectorFromFile("data/barrier.xml");
	for( auto& e : txt_vec)
	{
		auto txt_map = e.asValueMap();//将键值转化成Map格式，放入txt_map中
		int id_int = txt_map.at("id").asInt();//获取id
		int hp_int = txt_map.at("hp").asInt();
		int bubbleTypeCount_int = txt_map.at("bubble").asInt();
		int bubbleLayer_int = txt_map.at("layer").asInt();
		int scoreBase_int = txt_map.at("score").asInt();
		int goldBase_int = txt_map.at("gold").asInt();
		int suiji_int = txt_map.at("suiji").asInt();
		int shitou_int = txt_map.at("shitou").asInt();
		int mozhi_int = txt_map.at("mozhi").asInt();
		int wenyi_int = txt_map.at("wenyi").asInt();
		int huwei_int = txt_map.at("huwei").asInt();
		int huixue_int = txt_map.at("huixue").asInt();
		int goldTalent = txt_map.at("goldTalent").asInt();
		//int bubble_count = txt_map

		BarrierData *pData = new BarrierData;
		pData->id = id_int;
		pData->bossHp = hp_int;
		pData->bubbleTypeCount = bubbleTypeCount_int;
		pData->bubbleLayer = bubbleLayer_int;
		pData->scoreBase = scoreBase_int;
		pData->goldBase = goldBase_int;
		pData->suiji = suiji_int;
		pData->shitou = shitou_int;
		pData->mozhi = mozhi_int;
		pData->wenyi = wenyi_int;
		pData->huwei = huwei_int;
		pData->huixue = huixue_int;
		pData->goldTalent = goldTalent;
		barrierList.push_back(pData);
		
	}
	
}

void FileUtil::writeNewRecord(int record)
{
	if(record > g_recordScore){
		string filename = "record.txt";
		string path = FileUtils::getInstance()->getWritablePath();
		ofstream o_file;
		o_file.open(path + filename);

		o_file << version<< "\n";

		o_file << record << "\n";

		o_file.close();
	}
}

void FileUtil::readNewRecord()
{
	string filename = "record.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ifstream i_file;
	i_file.open(path + filename);

	if (i_file.is_open())
	{

		i_file >> version;

		i_file >> g_recordScore;
	}
	else
	{
		g_recordScore = 0;
	}
    i_file.close();

}



void FileUtil::writeGuide()
{
	
		string filename = "guide.txt";
		string path = FileUtils::getInstance()->getWritablePath();
		ofstream o_file;
		o_file.open(path + filename);

		o_file << version<< "\n";

		o_file << 1 << "\n";

		o_file.close();
	
}

void FileUtil::readGuide()
{
	string filename = "guide.txt";
	string path = FileUtils::getInstance()->getWritablePath();
	ifstream i_file;
	i_file.open(path + filename);

	if (i_file.is_open())
	{

		i_file >> version;

		i_file >> g_guide;
	}
	else
	{
		g_guide = 0;
	}
    i_file.close();

}
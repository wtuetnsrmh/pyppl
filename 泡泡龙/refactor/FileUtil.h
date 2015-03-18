#ifndef __File_Util_H__
#define __File_Util_H__
#include <vector>
#include "GameEnum.h"
class BarrierData;

class FileUtil
{
public:
	FileUtil();
	~FileUtil();
	static void readBarrierData();
	static void writeBarrierProgress(int barrier);
	static void readBarrierProgress(int &barrier);
	static std::vector<BarrierData *> barrierList;

	static void readGameGoods();
	static void writeGameGoods();

	static void readTalentData();
	static void writeTalentData(int tt[TT_END]);

	static void writeNewRecord(int record);
	static void readNewRecord();
	static void writeGuide();
	static void readGuide();
private:

};


#endif
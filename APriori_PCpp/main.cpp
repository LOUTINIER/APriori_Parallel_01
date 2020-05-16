#define _CRT_SECURE_NO_WARNINGS
#include "Apriori.h"
#include "myC.h"
#include <time.h>
#include <string>
using std::string;
using std::to_string;
using myC::InitData;
using myC::inDataSet;

void A(string path,const int k) {
	InitData(path.c_str());
	Apriori::minSup = k;
	
	clock_t s = clock();
	vector < Apriori> allSets = Apriori::GetAllSets();
	clock_t e = clock();

	printf("\n总耗时：%fs\n", float(e - s) / CLOCKS_PER_SEC);

	Apriori::ShowMaxFreqSets(allSets);
	for (int i = 0; i < 4; ++i)path.pop_back();
	for (auto &i : allSets)i.WriteToFile(path+"_minsup"+to_string(k)+".txt");
}


int main() {
	string pathes[] = {
	"C:\\Users\\13614\\OneDrive\\4\\数据挖掘\\实验\\EX1\\T1014D1K.dat",
	"C:\\Users\\13614\\OneDrive\\4\\数据挖掘\\实验\\EX1\\T1014D10K.dat",
	"C:\\Users\\13614\\OneDrive\\4\\数据挖掘\\实验\\EX1\\T1014D50K.dat",
	"C:\\Users\\13614\\OneDrive\\4\\数据挖掘\\实验\\EX1\\T1014D100K.dat",
	};
	int k[4][3] = {
		{6,8,10},{60,80,100 },{300,400,500},{600,800,1000}
	};
	for (int i = 0; i < 4; ++i) {
		string usePath = pathes[i];
		for (int j = 0; j < 3; ++j) {
			const int useK = k[i][j];
			for (int k = 0; k < 80; ++k)printf("*");
			printf("\n正在计算第%d个数据集，支持度为%d的情况\n", i + 1, useK);
			A(usePath, useK);
			InitData(usePath.c_str());
			for (int k = 0; k < 80; ++k)printf("*");
			printf("\n\n");
		}
	}

	int ttt;
	scanf("%d", &ttt);
	return 0;
}
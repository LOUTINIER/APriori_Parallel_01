#pragma once
#ifndef _APRIORI_H_
#define _APRIORI_H_
#include <vector>
#include <string>

using std::string;
using std::vector;

class Apriori
{
public:
	static int minSup;
	static vector<Apriori> GetAllSets();
	static void ShowMaxFreqSets(vector<Apriori>);
	void WriteToFile(string);
private:
	int K;
	vector<int> supCount;
	vector<vector<int>> freqSet;
	Apriori(int k);
	void GetFirstSet();
	Apriori GetNextSet();
	bool IsInFreq(const vector<int>&);
	static bool Contain(const vector<int>&,const int);
	static bool Contain(const vector<int>&, const vector<int>&);
	static bool Contain(const vector<vector<int>>&,const vector<int>&);
	static int Index(const vector<int>&,const int);
};
#endif
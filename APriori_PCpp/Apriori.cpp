#include "Apriori.h"
#include <sstream>
#include <fstream>
#include "myC.h"
#include <algorithm>
#include <time.h>
#include <omp.h>

using std::stringstream;
using std::to_string;
using std::ofstream;
using std::ifstream;
using std::swap;
using std::set_union;
using std::sort;
using myC::inDataSet;

int Apriori::minSup = 1;
Apriori::Apriori(int k) {
	K = k;
	supCount = vector<int>();
	freqSet = vector<vector<int>>();
}

void Apriori::WriteToFile(string path) {
	string a = to_string(K) + "项集:(" + to_string(supCount.size()) + "项)" + "\n";
	for (size_t i = 0; i < freqSet.size();++i) {
		a += "{";
		bool isFirst = true;
		for (const auto &j:freqSet[i]) {
			if (isFirst)isFirst = false;
			else a += ",";
			a += to_string(j);
		}
		a.append("} supCount=" + to_string(supCount[i]) + "\n");
	}
	ofstream ofs;
	ofs.open(path, std::ios_base::app);
	if (!ofs.is_open())
		printf("读取文件%s失败\n", path.c_str());
	ofs.write(a.c_str(), a.length());
	ofs.close();
	a.clear();
}

vector<Apriori> Apriori::GetAllSets() {
	vector<Apriori> allSets = vector<Apriori>();
	Apriori set = Apriori(1);
	set.GetFirstSet();
	while (set.supCount.size()>0)
	{
		allSets.push_back(set);
		set = set.GetNextSet();
	}
	return allSets;
}

void Apriori::ShowMaxFreqSets(vector<Apriori> allSets) {
	for (const auto &j:allSets) {
		int maxSup = 0;
		vector<int> maxSets = vector<int>();
		for (const auto &i : j.supCount)if (i > maxSup)maxSup = i;
		for (size_t i = 0; i < j.supCount.size(); ++i)
			if (j.supCount[i] == maxSup)maxSets.push_back(i);
		string a = "频繁" + to_string(j.K) + "项集有" + to_string(j.supCount.size()) + "项,最大支持度为:" + to_string(maxSup) + ",对应的频繁项集为:";
		for (const auto &i:maxSets) {
			a.append("{");
			bool isFirst = true;
			for (const auto &k:j.freqSet[i]) {
				if (isFirst)isFirst = false;
				else a.append(",");
				a.append(to_string(k));
			}
			a.append("}\n");
		}
		printf("%s", a.c_str());
	}
}

void Apriori::GetFirstSet() {
	vector<int> appearedValue = vector<int>();
	vector<int> tmpSupCount = vector<int>();
	// 对每一个出现的项计数得到频繁1项集
	for (const auto &k : inDataSet)
		for (const auto &l : k) {
			int indx = Index(appearedValue, l);
			if (indx == -1) {
				appearedValue.push_back(l);
				tmpSupCount.push_back(1);
			}
			else {
				tmpSupCount[indx] += 1;
			}
		}
	// 剪掉非频繁项
	for (unsigned int i = 0; i < appearedValue.size();++i) {
		if (tmpSupCount[i] >= minSup) {
			freqSet.push_back(vector<int>(1, appearedValue[i]));
			supCount.push_back(tmpSupCount[i]);
		}
	}
}

Apriori Apriori::GetNextSet() {
	Apriori ret = Apriori(K + 1);
	// 排序
	for (auto& i : freqSet)
		sort(i.begin(), i.end());

	// 并集
	vector<vector<int>> a[8] = {
		vector<vector<int>>(),vector<vector<int>>(),vector<vector<int>>(),vector<vector<int>>(),
		vector<vector<int>>(),vector<vector<int>>(),vector<vector<int>>(),vector<vector<int>>()
	 };
#pragma omp parallel num_threads(8) 
	{
		int id1 = omp_get_thread_num();
		for (size_t i = id1; i < freqSet.size(); i += 8)
		{
			for (size_t j = i + 1; j < freqSet.size(); ++j)
			{
				//vector<int> unit = vector<int>();
				//set_union(freqSet[i].begin(), freqSet[i].end(), freqSet[j].begin(), freqSet[j].end(), back_inserter(unit));
				//if (unit.size() == K + 1 && !IsInFreq(unit))a[id1].push_back(unit);
				for (int k = 0; k < K; ++k)
				{
					if (freqSet[i][k] != freqSet[j][k])
						if (k != K - 1)break;
						else {
							vector<int> unit = vector<int>();
							for (int g = 0; g < K + 1; ++g) {
								if (g != K)unit.push_back(freqSet[i][g]);
								else unit.push_back(freqSet[j][g - 1]);
							}
							if (!IsInFreq(unit)) {
								a[id1].push_back(unit);
							}
						}
				}
			}
		}
	}
	for (const auto &a1 : a)for (const auto &k1 : a1) { ret.freqSet.push_back(k1); }
	ret.supCount = vector<int>(ret.freqSet.size(), 0);
	// 计算支持度
	for (unsigned int i = 0; i < ret.freqSet.size(); ++i)
	{
		int tmpSup2[8] = { 0 };
#pragma omp parallel num_threads(8)
		{
			int id = omp_get_thread_num();
			for (unsigned int j = id; j < inDataSet.size(); j += 8)
			{
				if (Contain(inDataSet[j], ret.freqSet[i]))
					tmpSup2[id] += 1;
			}
		}
		for (const auto &j : tmpSup2)ret.supCount[i] += j;
	}
	// 剪枝
	for (size_t i = 0; i < ret.supCount.size();)
		if (ret.supCount[i] < minSup) {
			swap(ret.supCount[i], ret.supCount.back());
			ret.supCount.pop_back();
			swap(ret.freqSet[i], ret.freqSet.back());
			ret.freqSet.pop_back();
		}
		else ++i;
	return ret;

}

bool Apriori::IsInFreq(const vector<int> &a) {
	for (size_t i= 0; i < a.size(); ++i)
	{
		vector<int> sub = vector<int>();
		for (size_t j = 0; j < a.size(); ++j)
			if (j != i)sub.push_back(a[j]);
		if (!Contain(freqSet, sub))return true;
	}
	return false;
}

bool Apriori::Contain(const vector<int> &v,const int a) {
	for (auto &i:v) {
		if (i == a)return true;
	}
	return false;
}

bool Apriori::Contain(const vector<int> &a,const  vector<int> &b) {
	int cnt = 0;
	for (auto &i : b)if (Contain(a, i))cnt += 1;
	if (cnt == b.size())return true;
	return false;
}

bool Apriori::Contain(const vector<vector<int>> &a,const  vector<int> &b) {
	for (const auto &i : a) {
		int cnt = 0;
		for (const auto &j : b)if (Contain(i, j))cnt += 1;
		if (cnt == b.size())return true;
	}
	return false;
}

int Apriori::Index(const vector<int> &v,const int a) {
	for (size_t i = 0; i < v.size();++i) {
		if (v[i] == a)return i;
	}
	return -1;
}
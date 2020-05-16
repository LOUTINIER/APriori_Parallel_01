#define _CRT_SECURE_NO_WARNINGS
#include "myC.h"
#include <vector>
#include <fstream>
using std::vector;
using std::ifstream;
vector<vector<int>> myC::inDataSet = vector<vector<int>>();
void myC::InitData(const char* path) {
	inDataSet.clear();
	ifstream infile;
	infile.open(path, std::ios_base::binary);
	if (!infile.is_open())
		printf("¶ÁÈ¡ÎÄ¼þ%sÊ§°Ü\n", path);
	else
	{
		auto* buf = (char*)malloc(500 * sizeof(char));
		while (infile.getline(buf, 500 * sizeof(char))) {
			vector<int> tmp = vector<int>();
			char* p = strtok(buf, " ");
			while (p != nullptr)
			{
				tmp.push_back(atoi(p));
				p = strtok(nullptr, " ");
			}
			inDataSet.push_back(tmp);
			//auto* cursor = buf;
			//auto* begin = buf;
			//vector<int> tmp = vector<int>();
			//while (*cursor != '\r' && *(cursor + 1) != '\n') {
			//	if (*cursor == ' ') {
			//		*cursor = '\0';
			//		tmp.push_back(atoi(begin));
			//		begin = cursor + 1;
			//	}
			//	++cursor;
			//}
			//inDataSet.push_back(tmp);
		}
		free(buf);
	}
	infile.close();
}
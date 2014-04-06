#include <string>
#include <vector>
using namespace std;

struct KMP
{
	string needle;
	vector<int> T;
	KMP(const string needle)
	{
		this->needle = needle;
		T = vector<int>(needle.size() + 1);
		int i = 0 , j = -1;
		T[0] = -1;
		while(i < needle.size())
		{
			while(j >= 0 && needle[i] != needle[j])j = T[j];
			T[++i] = ++j;
		}
	}
	vector<int> match(const string hay)
	{
		vector<int> V;
		int i = 0 , j = 0;
		while(i < hay.size())
		{
			while(j >= 0 && hay[i] != needle[j])j = T[j];
			++i;++j;
			if(j == needle.size())
			{
				V.push_back(i - j);
				j = T[j];
			}
		}
		return V;
	}
};

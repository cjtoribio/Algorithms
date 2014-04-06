#include <string>
#include <algorithm>

using namespace std;

struct StringUtils
{
	static string getRandString(int maxSize = 0)
	{
		if(maxSize == 0)maxSize = 100;
		string ret = "";
		int sz = rand() % maxSize + 1;
		for(int i = 0; i < sz; ++i)
			ret += rand()%26 + 'a';
		return ret;
	}
};

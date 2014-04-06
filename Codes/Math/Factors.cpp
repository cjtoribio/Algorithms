#include <vector>
#include <iostream>
using namespace std;
struct Factors
{
	static vector<int> factorize(int N)
	{
		vector<int> ANS; ANS.reserve(200);
		while(N % 2 == 0) ANS.push_back(2), N /= 2;
		for(int i = 3; i*i <= N && N > 1; i += 2)
		{
			while(N % i == 0)
			{
				ANS.push_back(i);
				N /= i;
			}
		}
		if(N > 1)ANS.push_back(N);
		return ANS;
	}
};

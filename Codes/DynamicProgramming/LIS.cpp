#include <vector>
#include <map>
#include <iostream>
using namespace std;
struct LIS
{
	static vector<int> PreviusNumberLIS(const vector<int> &input)
	{
		int N = input.size();
		vector<int> KTH;
		vector<int> P(N);
		for(int i = 0; i < N; ++i)
		{
			int v = input[i];
			int p = lower_bound(KTH.begin(), KTH.end(),v) - KTH.begin();
			if(p == (int)KTH.size()) KTH.push_back(v);
			else KTH[p] = v;
			P[i] = p > 0 ? KTH[p-1] : -1000000000;
		}
		map<int,int> M; M[-1000000000] = -1;
		for(int i = 0; i < P.size(); ++i)
		{
			P[i] = M[P[i]];
			M[input[i]] = i;
		}
		return P;
	}
};

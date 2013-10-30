#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

vector<int> manachers(const string &A)
{
	vector<int> S;
	for(int i = 0; i < A.size(); ++i)
	{
		S.push_back(128);
		S.push_back(A[i]);
	}
	S.push_back(128);
	vector<int> P(S.size());
	int ma = 0;
	P[0] = 0;
	for(int i = 1; i < P.size(); ++i)
	{
		if( P[ma]+ma >= i)
			P[i] = min( P[2*ma-i] , 2*ma-i-(ma-P[ma]));
		int lo = i - P[i]-1, hi = i + P[i]+1;
		while(lo >= 0 && hi < P.size() && S[lo]==S[hi])
			P[i]++, lo--, hi++;
		if( P[ma]+ma < P[i]+i )
			ma = i;
	}
	return P;
}

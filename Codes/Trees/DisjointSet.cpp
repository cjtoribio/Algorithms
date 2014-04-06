#include <vector>
#include <iostream>
using namespace std;
struct DisjointSet
{
	vector<int> R , P;
	DisjointSet(int N)
	{
		R = vector<int>(N,0);
		P = vector<int>(N,-1);
	}
	int find(int x)
	{
		return P[x] == -1 ? x : (P[x] = find(P[x]));
	}
	bool join(int x,int y)
	{
		x = find(x); y = find(y);
		if(x == y)return false;
		if(R[x] > R[y]) P[y] = x;
		else P[x] = y;
		R[y] += R[x] == R[y];
		return true;
	}
};

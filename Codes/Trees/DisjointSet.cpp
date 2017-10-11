#include <vector>
#include <iostream>
using namespace std;
struct DisjointSet
{
	vector<int> P; // if < 0 then negative size, else parentId
	DisjointSet(int N) : P(N, -1) {}
	int find(int x) {
		return P[x] < 0 ? x : (P[x] = find(P[x]));
	}
	bool join(int x,int y) {
		if((x = find(x)) == (y = find(y))) return false;
		if(P[y] < P[x]) swap(x,y);
		P[x] += P[y];
		P[y] = x;
		return true;
	}
};

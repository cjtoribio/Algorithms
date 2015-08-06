#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <cassert>
#include <set>
#include <cmath>
using namespace std;
typedef long long Long;


typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int,int> PII;
typedef pair<PII,int> PPI;

struct CentroidDecomp {
	VVI adj;
	VI P, DI, SSZ;
	int N;
	
	// temp arrays
	VI V , SZ, BL;
	int TC;
	CentroidDecomp(int N):N(N){
		adj = VVI(N);
		TC = 0;
	}
	void addEdge(int u,int v){
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	void doneAddingEdges(){
		for(int i = 0; i < N; ++i){
			sort(adj[i].begin(), adj[i].end());
		}
	}
	int getCentroid(int u, int sz){
		V[u] = TC;
		SZ[u] = 1;
		int ma = 0, ans = -1;
		for(int i = 0; i < adj[u].size(); ++i){
			int v = adj[u][i];
			if(V[v] == TC || BL[v])continue;
			int t = getCentroid(v, sz);
			if(t != -1 && ans == -1)
				ans = t;
			SZ[u] += SZ[v];
			ma = max(ma, SZ[v]);
		}
		ma = max(ma, sz-SZ[u]);
		
		
		if(2*ma <= sz) return u;
		if(ans != -1)  return ans;
		return -1;
		
	}
	void dfs(int u, int &ma, int &maId,int LVL = 0){
		V[u] = TC;
		if(LVL > ma){
			maId = u;
			ma = LVL;
		}
		for(int i = 0; i < adj[u].size(); ++i){
			int v = adj[u][i];
			if(V[v] == TC || BL[v])continue;
			dfs(v,ma,maId,LVL + 1);
		}
	}
	int getHeight(int u){
		TC++;
		int ma = -1, maId = -1;
		dfs(u, ma, maId);
		return ma;
	}
	void getCentroids(){
		stack<PPI> STK;
		STK.push(PPI(PII(0,-1),N)); // start , forbidden , size of tree
		V  = vector<int>(N);
		SZ = vector<int>(N);
		BL = vector<int>(N);
		P  = vector<int>(N);
		DI = vector<int>(N);
		SSZ= vector<int>(N);
		
		
		while(STK.size()){
			PPI c = STK.top(); STK.pop();
			TC++;
			int cent = getCentroid(c.first.first, c.second);
			DI[cent] = getHeight(cent);
			SSZ[cent]= c.second;
			P[cent] = c.first.second;
			if(cent < 0)return;
			BL[cent] = 1;
			for(int i = 0; i < adj[cent].size(); ++i){
				int v = adj[cent][i];
				if(BL[v])continue;
				if(SZ[v] > SZ[cent]){
					STK.push(PPI(PII(v,cent),c.second - SZ[cent]));
				}else{
					STK.push(PPI(PII(v,cent),SZ[v]));
				}
			}
			cout << endl;
		}
		
		for (int i = 0; i < N; ++i) {
			cout << P[i] << ":" << DI[i] << ":" << SSZ[i] << "  ";
		}
		cout << endl;
		
	}
	void decompose(){
		vector<int> SZ(N,0);
		vector<int> V(N,0);
		
	}
};


int main()
{
	int N,M;
	cin >> N >> M;
	CentroidDecomp CD(N);
	for (int i = 0; i < N-1; ++i) {
		int u,v;
		cin >> u >> v;
		u--;v--;
		CD.addEdge(u,v);
	}
	CD.getCentroids();
}
/*

6 6
2 4
1 2
3 1
2 5
5 6

1 2 6
3 1 2
1 0 5
4 3 7


*/

#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

struct GraphCC
{
	typedef vector<int> VI;
	typedef vector<VI> VVI;
	typedef pair<int,int> PII;
	typedef vector<PII> VP;
	VI vis, LOW, ID , AP;
	VVI adj , SCC, BCC , CC;
	stack<int> S;
	VP B;
	int N;
	bool directed;
	GraphCC(int N, bool directed = false)
	{
		this->N = N; this->directed = directed;
		adj = VVI(N);
		vis = vector<int>(N);
		LOW = vector<int>(N);
		ID = vector<int>(N);
	}
	void addEdge(int u,int v)
	{
		adj[u].push_back(v);
		if(!directed)adj[v].push_back(u);
	}
	VI getArticPoints()
	{
		B.clear(); AP.clear(); BCC.clear(); SCC.clear();
		vis = vector<int>(N);
		S = stack<int>();
		for(int i = 0, IDX = 1; i < N; ++i , IDX = 1)
			dfsArticPoint(i,-1,IDX);
		sort(AP.begin(),AP.end());
		return AP;
	}
	VP getBridges()
	{
		B.clear(); AP.clear(); BCC.clear(); SCC.clear();
		vis = vector<int>(N);
		S = stack<int>();
		for(int i = 0, IDX = 1; i < N; ++i , IDX = 1)
			dfsArticPoint(i,-1,IDX);
		sort(B.begin(),B.end());
		return B;
	}
	VVI getBCC()
	{
		B.clear(); AP.clear(); BCC.clear(); SCC.clear();
		vis = vector<int>(N);
		S = stack<int>();
		for(int i = 0, IDX = 1; i < N; ++i , IDX = 1)
			dfsBCC(i,-1,IDX);
		for(int i = 0; i < (int)BCC.size(); ++i)
			sort(BCC[i].begin() , BCC[i].end());
		return BCC;
	}
	VVI getSCC()
	{
		B.clear(); AP.clear(); BCC.clear(); SCC.clear();
		vis = vector<int>(N);
		S = stack<int>();
		for(int i = 0, IDX = 1; i < N; ++i , IDX = 1)
			dfsSCC(i,IDX);
		for(int i = 0; i < (int)SCC.size(); ++i)
			sort(SCC[i].begin() , SCC[i].end());
		return SCC;
	}
	VVI getCC()
	{
		CC.clear();
		vis = vector<int>(N);
		S = stack<int>();
		for(int i = 0, IDX = 1; i < N; ++i , IDX = 1)
			dfsCC(i,IDX);
		for(int i = 0; i < (int)CC.size(); ++i)
			sort(CC[i].begin() , CC[i].end());
		return CC;
	}
private:
	void dfsSCC(int u,int &IDX)
	{
		if(vis[u])return;
		vis[u] = 1; // in S
		LOW[u] = ID[u] = IDX++;
		S.push(u);
		for(int i = 0; i < (int)adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(vis[v] == 2)continue;
			dfsSCC(v,IDX);
			LOW[u] = min(LOW[u] , LOW[v]);
		}
		if(ID[u] == LOW[u])
		{
			SCC.push_back(VI());
			do
			{
				vis[S.top()] = 2; // popped from S
				SCC.back().push_back(S.top());
				S.pop();
			}while(SCC.back().back() != u);
		}
	}
	void dfsArticPoint(int u ,int p,int &IDX)
	{
		if(vis[u])return;
		vis[u] = 1; // in S
		int cnt = 0;
		ID[u] = LOW[u] = IDX++;
		bool isAP = true;
		for(int i = 0; i < (int)adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(v == p)continue;
			if(!vis[v])
			{
				cnt++;
				dfsArticPoint(v,u,IDX);
				LOW[u] = min(LOW[u] , LOW[v]);
				if(LOW[v] > ID[u])
					B.push_back(pair<int,int>(u,v));
				isAP = isAP && (LOW[v] >= ID[u]);
			}
			else
			{
				LOW[u] = min(LOW[u] , ID[v]);
			}
		}
		if(cnt && isAP && (ID[u] > 1 || cnt > 1))
			AP.push_back(u);
	}
	void dfsBCC(int u ,int p, int &IDX)
	{
		if(vis[u])return;
		vis[u] = 1;
		S.push(u);
		ID[u] = LOW[u] = IDX++;
		for(int i = 0; i < (int)adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(v == p)continue;
			if(!vis[v])
			{
				dfsBCC(v,u,IDX);
				LOW[u] = min(LOW[u] , LOW[v]);
				if(LOW[v] >= ID[u])
				{
					BCC.push_back(VI());
					while(S.top() != u)
					{
						BCC.back().push_back(S.top());
						S.pop();
					}
					BCC.back().push_back(u);
				}
			}
			else
			{
				LOW[u] = min(LOW[u] , ID[v]);
			}
		}
	}
	void dfsCC(int u, int &IDX)
	{
		if(vis[u])return;
		ID[u] = IDX++;
		vis[u] = 1;
		S.push(u);
		for(int i = 0; i < (int)adj[u].size(); ++i)
		{
			int v = adj[u][i];
			dfsCC(v,IDX);
		}
		if(ID[u] == 1)
		{
			CC.push_back(VI());
			while(!S.empty())
			{
				CC.back().push_back(S.top());
				S.pop();
			}
		}
	}
};

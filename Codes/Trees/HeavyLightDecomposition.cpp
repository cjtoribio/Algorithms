typedef vector<int> VI;
typedef vector<VI> VVI;
struct HLD
{
	typedef vector<int> VI;
	typedef vector<VI> VVI;
	VI P , LVL , SZ , PC;
	VVI adj;
	VVI groups;
	VI groupId;
	VI groupPos;
	int N;
	HLD(VVI adj) {
		N = adj.size();
		P = PC = VI(N , -1);
		LVL = SZ = groupId = groupPos = VI(N , 0);
		this->adj = adj;
		for(int i = 0; i < N; ++i)
		{
			if(SZ[i] == 0)
			{
				initDfs(i);
				groups.push_back(VI());
				for(int j = i; j != -1; j = PC[j])
					groups.back().push_back(j) ,
					groupId[j] = groups.size() - 1 ,
					groupPos[j] = groups.back().size()-1;
			}
		}
	}
	void initDfs(int u,int level = 0)
	{
		LVL[u] = level;
		SZ[u] = 1;
		for(int i = 0; i < adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(SZ[v])continue;
			P[v] = u;
			initDfs(v,level + 1);
			SZ[u] += SZ[v];
		}
		for(int i = 0; i < adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(v == P[u])continue;
			if(2*SZ[v] > SZ[u]-1){
				PC[u] = v;
			}
			else
			{
				groups.push_back(VI());
				for(int j = v; j != -1; j = PC[j])
					groups.back().push_back(j) ,
					groupId[j] = groups.size() - 1 ,
					groupPos[j] = groups.back().size()-1;
			}
		}
	}
	int LCA(int u,int v)
	{
		while(u != v && u != -1 && v != -1)
		{
			if(LVL[v] < LVL[u])swap(u,v);
			if(groupId[u] == groupId[v])return u;
			bool vGoesUp = LVL[groups[groupId[u]][0]] < LVL[groups[groupId[v]][0]];
			(vGoesUp ? v : u) = P[ groups[groupId[(vGoesUp ? v : u)]][0] ];
		}
		return u == v ? v : -1;
	}
};
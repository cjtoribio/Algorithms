//  SOURCE: https://sites.google.com/site/indy256/algo/2_sat
typedef vector<int> VI;
typedef vector<VI> VVI;
struct SAT2
{
	VVI adj , radj;
	VI vis , order, comp , sat;
	SAT2(int N){
		adj = VVI(2*N);
		radj = VVI(2*N);
	}
	void addImplication(int i,int j,int neg)
	{
		adj[2*i+(neg>>1)].push_back(2*j+(neg&1));
		radj[2*j+(neg&1)].push_back(2*i+(neg>>1));
	}
	void dfs1(int u)
	{
		vis[u] = 1;
		for(int i = 0; i < adj[u].size(); ++i)
		{
			int v = adj[u][i];
			if(!vis[v]) dfs1(v);
		}
		order.push_back(u);
	}
	void dfs2(int u,int color)
	{
		comp[u] = color;
		for(int i = 0; i < radj[u].size(); ++i)
		{
			int v = radj[u][i];
			if(comp[v] == -1)
				dfs2(v,color);
		}
	}
	bool run2SAT()
	{
		int N = adj.size();
		vis = VI(N);
		comp = VI(N,-1);
		sat = VI(N / 2);
		for(int i = 0; i < N; ++i)
			if(!vis[i])
				dfs1(i);
		int c = 0;
		for(int i = 0; i < N; ++i)
		{
			int u = order[N-i-1];
			if(comp[u] == -1)
				dfs2(u,c++);
		}
		for(int i = 0; i < N; i++)
			if(comp[i] == comp[i^1])
				return false;
		for (int i = 0; i < N; i += 2)
		{
			sat[i / 2] = comp[i] > comp[i ^ 1];
//			cout << i/2 << " " << (sat[i/2]?"true":"false") << endl;
		}
		return true;
	}
};
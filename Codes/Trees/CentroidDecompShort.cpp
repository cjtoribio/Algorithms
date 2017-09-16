struct Centroid {
	struct Edge { int u, v; };
	vector<vector<Edge>> adj;
	vector<vector<int>> centAdj;
	vector<int> SZ, L, P;
	int root;
	Centroid(int N): adj(N), centAdj(N), SZ(N), L(N, -1), P(N, -1), root(-1){}
	void addEdge(int u,int v){
		adj[u].push_back((Edge){u, v});
		adj[v].push_back((Edge){v, u});
	}
	int getCentroid(int u, int p, int sz, int lvl){
		SZ[u] = 1;
		int cent = -1, good = true;
		for(Edge &e : adj[u]){ 
			if(e.v == p || L[e.v] != -1)continue;
			cent = max(cent, getCentroid(e.v, u, sz, lvl));
			SZ[u] += SZ[e.v];	
			good &= SZ[e.v] <= sz / 2;
		}
		good &= sz - SZ[u] <= sz / 2;
		return good ? u : cent;
	}
	void computeCentroid(int root = 0, int prev = -1, int lvl = 0, int sz = -1){
		if(sz == -1)sz = adj.size();
		int cent = getCentroid(root, -1, sz, lvl);
		L[cent] = lvl; P[cent] = prev;
		if(prev == -1)this->root = cent;
		else centAdj[prev].push_back(cent);
		for(Edge &e : adj[cent]){ 
			if(L[e.v] == -1){
				int nsz = (SZ[e.v] > SZ[cent] ? sz - SZ[cent] : SZ[e.v]);
				computeCentroid(e.v, cent, lvl + 1, nsz);
			}
		}
		SZ[cent] = sz;
	}
	int lca(int u, int v){
		while(L[u] > L[v])u = P[u];
		while(L[v] > L[u])v = P[v];
		while(u != v)u = P[u], v = P[v];
		return u;
	}
};
struct Centroid {
	struct Edge { int u, v, w; };
	vector<vector<Edge>> adj;
	vector<vector<int>> centAdj;

	vector<int> SZ; // size of the nodes in centered at u
	vector<int> L; // level in the centroid tree
	vector<int> P; // parent in the centroid tree
	int root;
	Centroid(int N): adj(N), centAdj(N), SZ(N), L(N, -1), P(N, -1), root(-1){ }
	void addEdge(int u,int v, int w) {
		adj[u].push_back((Edge){u, v, w});
		adj[v].push_back((Edge){v, u, w});
	}
	int dfsSz(int u, int p = -1) {
		SZ[u] = 1;
		for (Edge &e : adj[u])
			if (e.v != p && L[e.v] == -1)
				SZ[u] += dfsSz(e.v, u);
		return SZ[u];
	}
	int dfsGetCentroid(int u, int sz, int p = -1) {
		for (Edge &e : adj[u])
			if (e.v != p && L[e.v] == -1 && SZ[e.v] > sz / 2)
				return dfsGetCentroid(e.v, sz, u);
		return u;
	}
	void computeCentroid(int root = 0, int prev = -1, int lvl = 0){
		int cent = dfsGetCentroid(root, dfsSz(root));
		L[cent] = lvl; P[cent] = prev; SZ[cent] = SZ[root];
		if(prev == -1) this->root = cent; else centAdj[prev].push_back(cent);
		for(Edge &e : adj[cent])
			if (L[e.v] == -1)
				computeCentroid(e.v, cent, lvl + 1);
	}
	int lca(int u, int v){
		while(L[u] > L[v])u = P[u];
		while(L[v] > L[u])v = P[v];
		while(u != v)u = P[u], v = P[v];
		return u;
	}
};


template<typename T>
struct Centroid {
	struct Edge { int u, v; T data; };
	vector<vector<Edge>> adj;
	VI SZ, L, P;
	Centroid(int N): adj(N), SZ(N), L(N, -1), P(N, -1){}
	void addEdge(int u,int v, T data){
		adj[u].push_back((Edge){u, v, data});
		adj[v].push_back((Edge){v, u, data});
	}
	int getCentroid(int u, int p, int sz, int lvl){
		SZ[u] = 1;
		int cent = -1, good = true;
		for(Edge &e : adj[u]){ 
			if(e.v == p || L[e.v] != -1)continue;
			int t = getCentroid(e.v, u, sz, lvl);
			if(cent == -1 && t != -1) cent = t;
			SZ[u] += SZ[e.v];	
			good &= SZ[e.v] <= sz / 2;
		}
		good &= sz - SZ[u] <= sz / 2;
		if(good)cent = u;
		return cent;
	}
	void computeCentroid(int root = 0, int prev = -1, int lvl = 0, int sz = -1){
		if(sz == -1)sz = adj.size();
		int cent = getCentroid(root, -1, sz, lvl);
		L[cent] = lvl; P[cent] = prev;
		for(Edge &e : adj[cent]){ 
			if(L[e.v] == -1){
				int nsz = (SZ[e.v] > SZ[cent] ? sz - SZ[cent] : SZ[e.v]);
				computeCentroid(e.v, cent, lvl + 1, nsz);
			}
		}
	}
	void dfs(int blockLevel, int u, int p = -1, int depth = 0){
		// custom DFS in each subtree
		for(Edge &e : adj[u]){
			if(e.v == p || L[e.v] < blockLevel)continue;
			dfs(blockLevel, e.v, u, depth+1);
		}
	}
};
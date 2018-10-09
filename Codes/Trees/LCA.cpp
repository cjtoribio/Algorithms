Long D[100010]; // this distance is optional
struct BinaryLifting {
    VVI adj, T;
    VI L;
    int N;
    BinaryLifting(int N) : N(N), adj(N), T(20, VI(N, -1)), L(N) { }
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void build(int root = 0) {
        dfs(root);
        for (int i = 1; i < 20; ++i)
            for (int j = 0; j < N; ++j)
                if (T[i-1][j] != -1)
                    T[i][j] = T[i-1][ T[i-1][j] ];
    }
    void dfs(int u, int p = -1, int lvl = 0, Long v = 0) {
        T[0][u] = p; L[u] = lvl;
        D[u] += v; // add distance from root to have sum([v, root])
        for (int v : adj[u]) if (v != p) dfs(v, u, lvl+1, D[u]);
    }
    int lca(int u, int v) {
        if (L[u] < L[v]) swap(u, v);
        for (int i = 19; i >= 0; --i)
            if (L[u] - (1<<i) >= L[v])
                u = T[i][u];
        if (u == v) return u;
        for (int i = 19; i >= 0; --i)
            if (T[i][u] != T[i][v])
                u = T[i][u], v = T[i][v];
        return T[0][u];
    }
    Long cost(int u, int p) { // from [u, p)
        return D[u] - (p == -1 ? 0 : D[p]);
    }
};

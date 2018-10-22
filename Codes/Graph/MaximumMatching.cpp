struct MaxMatching {
    int L, R;
    vector<int> ML, MR;
    VVI adj;
    MaxMatching(int L,int R) : ML(L,-1), MR(R,-1), adj(L), L(L), R(R) {}
    void AddEdge(int u, int v){
        adj[u].push_back(v);
    }
    int findMatch(int u, VI &VIS) {
        if (VIS[u]) return 0;
        VIS[u] = 1;
        for (int v : adj[u]) {
            if (MR[v] == -1 || findMatch(MR[v], VIS)) {
                MR[v] = u;
                ML[u] = v;
                return 1;
            }
        }
        return 0;
    }
    int run() {
        VI VIS(L);
        int cnt = 0;
        for (int i = 0; i < L; ++i) {
            VIS = VI(L);
            cnt += findMatch(i, VIS);
        }
        return cnt;
    }
};

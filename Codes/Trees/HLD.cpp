struct HLD {
    int N;
    VI SZ, P, ST, EN, TOP, LVL;
    vector<int> PERM;
    VVI adj;
    HLD(int N) : N(N), adj(N), PERM(N), SZ(N), P(N), ST(N), EN(N), TOP(N), LVL(N) {}
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    void dfsSZ(int u, int p = -1, int lvl = 0) {
        SZ[u] = 1; P[u] = p; LVL[u] = lvl;
        VI &C = adj[u];
        int bi = -1;
        for (int i = 0; i < C.size(); ++i) {
            int v = C[i];
            if (v != p) {
                dfsSZ(v, u, lvl + 1);
                if (bi == -1 || SZ[C[bi]] < SZ[v]) 
                    bi = i;
                SZ[u] += SZ[v];
            }
        }
        while (bi > 0) swap(C[bi-1], C[bi]), bi--;
    }
    void dfsDiscovery(int u, int top, int &idx, int p = -1) {
        ST[u] = idx++; TOP[u] = u;
        for (int v : adj[u])
            if (v != p)
                dfsDiscovery(v, v == adj[u][0] ? top : v, idx, u);
        EN[u] = idx;
    }
    void build() {
        int idx = 0;
        for (int i = 0; i < N; ++i) {
            if (SZ[i]) continue;
            dfsSZ(i);
            dfsDiscovery(i, i, idx);
        }
        for (int i = 0; i < N; ++i) {
            PERM[ST[i]] = i;
        }
    }
    int lca(int u, int v) {
        while (u != -1 && v != -1) {
            if (TOP[u] == TOP[v]) return LVL[u] <= LVL[v] ? u : v;
            else if (LVL[TOP[u]] > LVL[TOP[v]]) u = P[TOP[u]];
            else v = P[TOP[v]];
        }
        return -1;
    }
    // note: need to reverse in the second leg if legs are not commutative
    vector<pair<int,int>> lift(int u, int p, bool includeP = true) {
        vector<pair<int,int>> o;
        while (true) {
            if (TOP[u] == TOP[p]) {
                if (ST[p] + (includeP ? 0 : 1) <= ST[u])
                    o.emplace_back(ST[p] + (includeP ? 0 : 1), ST[u]);
                break;
            } else o.emplace_back(ST[TOP[u]], ST[u]);
            u = P[TOP[u]];
        }
        return o;
    };
};

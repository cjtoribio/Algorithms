vector<int> byHeight[3010];
int NODE_H[3010], RANK[3010];
int dfs(int u, int p, VVI &adj, VVI &nadj) {
    int h = 0;
    for (int v : adj[u]) {
        if (v == p) continue;
        nadj[u].push_back(v);
        h = max(h, 1 + dfs(v, u, adj, nadj));
    }
    byHeight[h].push_back(u);
    return NODE_H[u] = h;
}

struct Comparator {
    int ch;
    VVI &adj;
    int operator()(int a, int b) {
        if (NODE_H[a] != NODE_H[b]) return NODE_H[a] > NODE_H[b];
        if (NODE_H[a] != ch) return RANK[a] < RANK[b];
        int mn = min(adj[a].size(), adj[b].size());
        for (int j = 0; j < mn; ++j) {
            if (RANK[adj[a][j]] != RANK[adj[b][j]])
                return RANK[adj[a][j]] < RANK[adj[b][j]];
        }
        return adj[a].size() > adj[b].size(); // if A has more it will be lexicografically smaller
    }
};

void build(int u, string &S, VVI &adj) {
    S += "(";
    for (int v : adj[u])
        build(v, S, adj);
    S += ")";
}

string encodeTree(int cent, VVI &adj) {
    int N = adj.size();
    VVI nadj(N);
    int currRank = N;
    for (int t = 0; t < N; ++t) byHeight[t].clear();
    int H = dfs(cent, -1, adj, nadj);
    for (int h = 0; h <= H; ++h) {
        Comparator cmp = {h, nadj};
        VI &L = byHeight[h];
        if (h == 0) {
            for (int u : L) RANK[u] = currRank-1;
            currRank--;
            continue;
        }
        for (int u : L) sort(nadj[u].begin(), nadj[u].end(), cmp);
        sort(L.begin(), L.end(), cmp);
        for (int i = int(L.size())-1; i >= 0; --i) {
            if (i +1 == L.size()) {
                RANK[L[i]] = --currRank;
            } else if (cmp(L[i], L[i+1]) == cmp(L[i+1], L[i])) {
                RANK[L[i]] = currRank;
            } else {
                RANK[L[i]] = --currRank;
            }
        }
    }
    string S;
    build(cent, S, nadj);
    return S;
}

typedef long long Long;
const Long INF = 1000000000000000000LL;
struct Edge { int to, cap, cost, flow, back; };
struct MinCostFlow {
    int n;
    vector<vector<Edge>> g;
    MinCostFlow(int n):n(n), g(n) {}
    void add_rib(int a, int b, int u, int c) {
        Edge r1 = {b, u, c, 0, g[b].size()};
        Edge r2 = {a, 0, -c, 0, g[a].size()};
        g[a].push_back(r1);
        g[b].push_back(r2);
    }
    pair<Long, Long> run(int s, int t, Long k = 1000000000000000000LL) {
        Long flow = 0, cost = 0;
        while (flow < k) {
            vector<int> id(n, 0);
            vector<Long> d(n, INF);
            vector<int> q(n);
            vector<int> p(n);
            vector<size_t> p_rib(n);
            int qh = 0, qt = 0;
            q[qt++] = s;
            d[s] = 0;
            while (qh != qt) {
                int v = q[qh++];
                id[v] = 2;
                if (qh == n) qh = 0;
                for (int i = 0; i < g[v].size(); ++i) {
                    Edge &r = g[v][i];
                    if (r.flow < r.cap && d[v] + r.cost < d[r.to]) {
                        d[r.to] = d[v] + r.cost;
                        if (id[r.to] == 0) {
                            q[qt++] = r.to;
                            if (qt == n) qt = 0;
                        } else if (id[r.to] == 2) {
                            if (--qh == -1) qh = n - 1;
                            q[qh] = r.to;
                        }
                        id[r.to] = 1;
                        p[r.to] = v;
                        p_rib[r.to] = i;
                    }
                }
            }
            if (d[t] == INF) break;
            Long addflow = k - flow;
            for (int v = t; v != s; v = p[v]) {
                Edge &f = g[p[v]][p_rib[v]];
                addflow = min(addflow, (Long) f.cap - f.flow);
            }
            for (int v = t; v != s; v = p[v]) {
                Edge &f = g[p[v]][p_rib[v]]; // forward edge
                Edge &b = g[v][f.back];      // backward edge
                f.flow += addflow; b.flow -= addflow;
                cost += f.cost * addflow;
            }
            flow += addflow;
        }
        return make_pair(flow, cost);
    }
};

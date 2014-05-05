// SOURCE:
//	   - http://www.stanford.edu/~liszt90/acm/notebook.html#file2
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>

using namespace std;

typedef long long Long;
typedef vector<Long> VL;
typedef vector<VL> VVL;
typedef vector<PII> VPII;
const Long INF = numeric_limits<Long>::max() / 4;
struct MinCostMaxFlow {
	int N;
	VVL cap, flow, cost;
	VL dist, pi, width , found;
	VPII P;
	MinCostMaxFlow(int N) {
		this->N = N;
		cap = flow = cost= VVL(N,VL(N));
		found = dist = pi = width = VL(N);
		P = VPII(N);
	}
	void AddEdge(int from, int to, Long cap, Long cost) {
		this->cap[from][to] = cap;
		this->cost[from][to] = cost;
	}
	Long Dijkstra(int s, int t) {
		fill(found.begin(), found.end(), false);
		fill(dist.begin(), dist.end(), INF);
		fill(width.begin(), width.end(), 0);
		dist[s] = 0;
		width[s] = INF;
		priority_queue<PII> PQ;
		PQ.push(PII(0, s));
		while (PQ.size()) {
			int u = PQ.top().second;
			PQ.pop();
			if (found[u]) continue;
			found[u] = true;
			for (int k = 0; k < N; k++) {
				if (found[k]) continue;
				int ca, nd;
				ca = cap[u][k] - flow[u][k], nd = dist[u] + pi[u] - pi[k] + cost[u][k];
				if (ca && nd < dist[k])
					dist[k] = nd, width[k] = ca, PQ.push(PII(-nd, k)), P[k] = PII(u, 1);
				ca = -flow[k][u], nd = dist[u] + pi[u] - pi[k] - cost[k][u];
				if (ca && nd < dist[k])
					dist[k] = nd, width[k] = ca, PQ.push(PII(-nd, k)), P[k] = PII(u, -1);
			}
		}
		for (int k = 0; k < N; k++)
			pi[k] = min(pi[k] + dist[k], INF);
		return width[t];
	}
	pair<Long, Long> GetMaxFlow(int s, int t) {
		Long totflow = 0, totcost = 0;
		while (Long amt = Dijkstra(s, t)) {
			totflow += amt;
			for (int x = t; x != s; x = P[x].first) {
				int d = P[x].second , u = P[x].first , v = x;
				if(d==-1)swap(u,v);
				flow[u][v] += amt * d;
				totcost += amt * cost[u][v] * d;
			}
		}
		return make_pair(totflow, totcost);
	}
};
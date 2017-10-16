struct Hungarian {
	VI matchL, matchR;
	int cost;
	int run(VVI &a){
		const int n = a.size(), m = a[0].size(), INF = 1000000000;
		vector<int> u(n + 1), v(m + 1), p(m + 1), way(m + 1);
		for (int i=1; i<=n; ++i) {
			p[0] = i;
			int j0 = 0;
			vector<int> minv (m+1, INF);
			vector<bool> used (m+1, false);
			do {
				used[j0] = true;
				int i0 = p[j0], delta = INF, j1;
				for (int j=1; j<=m; ++j)
				if (!used[j]) {
					int cur = a[i0-1][j-1]-u[i0]-v[j];
					if (cur < minv[j])
					minv[j] = cur, way[j] = j0;
					if (minv[j] < delta)
					delta = minv[j], j1 = j;
				}
				for (int j=0; j<=m; ++j)
				if (used[j])
				u[p[j]] += delta, v[j] -= delta;
				else
				minv[j] -= delta;
				j0 = j1;
			}while (p[j0] != 0);
			do {
				int j1 = way[j0];
				p[j0] = p[j1];
				j0 = j1;
			}while (j0);
		}
		matchL = VI(n, -1), matchR = VI(m);
		for (int j=1; j<=m; ++j) if(p[j]) matchL[p[j]-1] = j-1;
		for (int j=1; j<=m; ++j) matchR[j-1] = p[j]-1;
		return cost = -v[0];
	}
};
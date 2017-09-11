struct Fenwick2D {
	struct Point { 
		int x, y; 
	};
	int N;
	VVI F;
	VVI order;
	VI X;
	void build(vector<Point> P)
	{
		X.reserve(P.size());
		for(Point p : P)X.push_back(p.x);
		sort(X.begin(), X.end());
		X.erase(unique(X.begin(), X.end()), X.end());
		N = X.size() + 1;
		F.resize(N);
		order.resize(N);
		for(Point &p : P){
			p.x = upper_bound(X.begin(), X.end(), p.x) - X.begin();
		}
		sort(P.begin(), P.end(), [](const Point &a, const Point &b){ return a.y < b.y; });
		for(Point p : P){
			for (int x = p.x; x < N; x += x & -x) {
				order[x].push_back(p.y);
			}
		}
		for(int i = 0; i < N; ++i){
			F[i].resize(order[i].size() + 1);
		}
	}
	int getIndex(int x, VI &O){ return upper_bound(O.begin(), O.end(), x) - O.begin(); }
	int get(int x, int y){
		int ans = 0;
		for (int i = getIndex(x, X); i > 0; i -= i & -i) {
			for(int j = getIndex(y, order[i]); j > 0; j -= j & -j){
			    ans += F[i][j];
			}
		}
		return ans;
	}
	void add(int x, int y, int v){
		for (int i = getIndex(x, X); i < F.size(); i += (i & -i)) {
			for(int j = getIndex(y, order[i]); j < F[i].size(); j += (j & -j)){
			    F[i][j] += v;
			}
		}
	}
};
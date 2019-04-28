/// This fenwick is used when N^2 is too big to fit in memory.
/// For efficiency it is required that point are never heavily-clustered in a block
/// Complexity is:
/// 	Query: (MAX_CLUSTER + log(N/BSIZE)^2)
/// 	Update: log(N/BSIZE)^2
struct Fenwick2D {
	const int BSIZE = 300;
	struct Update {
		int x,y,v;
	};
	int N;
	VVI F;
	vector<vector<Update>> X;
	vector<vector<Update>> Y;
	Fenwick2D(int n):N(n / BSIZE+2), F(n / BSIZE+2, VI(n / BSIZE+2)), X(n / BSIZE+2), Y(n / BSIZE+2){}
	void ins(int x, int y, int v){
		_ins(x / BSIZE, y / BSIZE, v);
		addOrRemove(X[x/BSIZE], {x, y, v});
		addOrRemove(Y[y/BSIZE], {x, y, v});
	}
	void addOrRemove(vector<Update> &V, Update u) {
		for (int i = 0; i < V.size(); ++i) {
			if (V[i].x == u.x && V[i].y == u.y && V[i].v == -u.v) {
				V.erase(V.begin() + i);
				return;
			}
		}
		V.push_back(u);
	}
	void _ins(int x, int y, int v){
		for (int i = x+1; i < N; i += i & -i) {
			for (int j = y+1; j < N; j += j & -j) {
				F[i][j] += v;
			}
		}
	}
	int get(int x, int y){
		int r = _get(x / BSIZE, y / BSIZE);
		for(Update u : X[x / BSIZE]) if(u.y/BSIZE <= y/BSIZE && u.x > x) r -= u.v;
		for(Update u : Y[y / BSIZE]) if(u.x <= x && u.y > y) r -= u.v;
		return r;
	}
	int _get(int x, int y){
		int r = 0;
		for (int i = x+1; i > 0; i -= i & -i) {
			for (int j = y+1; j > 0; j -= j & -j) {
				r += F[i][j];
			}
		}
		return r;
	}
};

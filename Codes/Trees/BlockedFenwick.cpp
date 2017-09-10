
/// This fenwick is used when N^2 is too big to fit in memory.
/// The requirement is that in a RANGE of a coordinate [x, x+BLOCK] or [y, y+BLOCK]
/// there should be few numbers. Since complexity is:
/// 	((log(N/BSIZE)^2 + RANGE^2 * N/BSIZE)
struct Fenwick2D {
	const int BSIZE = 200;
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
		X[x/BSIZE].push_back({x,y,v});
		Y[y/BSIZE].push_back({x,y,v});
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
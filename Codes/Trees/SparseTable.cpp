#define dlog2(n) (31-__builtin_clz(n))
template<class T>
struct SparseTable {
	int N, L;
	vector<vector<T>> table;
	SparseTable(const vector<T> &V):
			N(V.size()), L(dlog2(N)+1), table(L, vector<T>(N)){
		table[0] = V;
		for(int sz = 1; sz < L; ++sz){
			for (int i = 0; i < N; ++i) {
				if(i + (1<<sz) > N)break;
				table[sz][i] = min(table[sz-1][i], table[sz-1][i+(1<<(sz-1))]);
			}
		}
	}
	T get(int i, int j){
		int d = dlog2(j-i+1);
		return min(table[d][i], table[d][j-(1<<d)+1]);
	}
};

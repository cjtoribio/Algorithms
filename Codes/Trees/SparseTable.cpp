#define dlog2(n) (31-__builtin_clz(n))
struct SparseTable {
	int N, L;
	VVI table;
	SparseTable(const vector<int> &V):
		N(V.size()), L(dlog2(N)+1), table(L, VI(N)){
		table[0] = V;
		for(int sz = 1; sz < L; ++sz){
			for (int i = 0; i < N; ++i) {
				if(i + (1<<sz) > N)break;
				table[sz][i] = min(table[sz-1][i], table[sz-1][i+(1<<(sz-1))]);
			}
		}
	}
	int get(int i, int j){
		int d = dlog2(j-i+1);
		return min(table[d][i], table[d][j-(1<<d)+1]);
	}
};
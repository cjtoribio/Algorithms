typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long Long;
// http://codeforces.com/blog/entry/45223
struct SubsetSum {
	int mb;
	VI A, B, F;
	SubsetSum(int mb):mb(mb), A(1<<mb), B(1<<mb), F(1<<mb){}
	void addVal(int n, int v = 1){
		A[n] += v; // for counting subsets
		B[n] += v; // for counting supersets
		F[n] += v; // for original freq
	}
	void build(){
		int ALL = (1<<mb)-1;
		for (int j = 0; j < mb; ++j) {
			for (int i = 0; i < (1<<mb); ++i) {
				if(i & (1<<j) ) {
					A[i] += A[i^(1<<j)];
					B[ALL-i] += B[(ALL-i)^(1<<j)];
				}
			}
		}
	}
	int sumSupersets(int n){
		return B[n];
	}
	int sumSubsets(int n){
		return A[n];
	}
	int bestOr(int n){
		int best = 0;
		for (int i = mb-1; i >= 0; --i) {
			if( n & (1<<i) ) continue; // not required bit
			if(sumSupersets(best ^ (1<<i))) // exist a super set with this bit?
				best ^= (1<<i);
		}
		return best;
	}
};

int main() {
	SubsetSum S(4); // (number of bits)
	vector<int> V = {1, 3, 4 ,2, 1, 4, 2};
	for (int i = 0; i < V.size(); ++i) {
		S.addVal(V[i], 1);
	}
	S.build();
	for (int i = 0; i < V.size(); ++i) {
		cout << S.bestOr(V[i]) << " " << S.sumSubsets(V[i]) << " " << S.sumSupersets(V[i]) << endl;
	}
}

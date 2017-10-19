struct SubsetSum {
	int mb;
	VVI A; // A[j][i] = subset   of "i" on the lower "j" bits and match exacly upper bits
	VVI B; // B[j][i] = superset of "i" on the lower "j" bits and match exacly upper bits;
	SubsetSum(int mb):mb(mb), A(mb+1, VI(1<<mb)), B(mb+1, VI(1<<mb)){}
	void addVal(int n, int v = 1){
		A[0][n] += v; // for counting subsets
		B[0][n] += v; // for counting supersets
	}
	void build(){
		int ALL = (1<<mb)-1;
		for (int i = 0; i < (1<<mb); ++i) {
			for (int j = 0; j < mb; ++j) {
				if(i & (1<<j) ) {
					A[j+1][i] = A[j][i] + A[j][i^(1<<j)];
					B[j+1][ALL-i] = B[j][ALL-i] + B[j][(ALL-i)^(1<<j)];
				}else{
					B[j+1][ALL-i] = B[j][ALL-i];
					A[j+1][i] = A[j][i];
				}
			}
		}
	}
	int sumSupersets(int n){
		return B[mb][n];
	}
	int sumSubsets(int n){
		return A[mb][n];
	}
};
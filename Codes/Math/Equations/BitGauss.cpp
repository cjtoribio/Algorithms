template<int COLS>
vector<int> bitGauss(VVI m){ // last column is arbitratry integer
	int R = m.size(), C = m[0].size();
	vector<bitset<COLS>> B(R);
	for (int i = 0; i < R; ++i) {
		for (int j = 0; j < C; ++j) {
			B[i][j] = m[i][j];
		}
	}
	int pivot = 0;
	for (int r = 0; r < R && pivot+1 < C; ) {
		bool f = 0;
		for (int i = r; i < B.size(); ++i) {
			if(B[i][pivot]){
				f = 1;
				swap(B[i], B[r]);
				break;
			}
		}
		if(f){
			for (int i = r+1; i < B.size(); ++i) {
				if(B[i][pivot]){
					B[i] ^= B[r];
				}
			}
			r++;
		}
		pivot++;
	}
	for (int i = R-1; i >= 0; --i) {
		for (int j = 0; j < C; ++j) {
			if(B[i][j]){
				for (int k = i-1; k >= 0; --k) {
					if(B[k][j]){
						B[k] ^= B[i];
					}
				}
				break;
			}
		}
	}
	for (int i = 0; i < R; ++i) {
		for (int j = 0; j < C; ++j) {
			m[i][j] = B[i][j];
		}
	}
	return m;
}
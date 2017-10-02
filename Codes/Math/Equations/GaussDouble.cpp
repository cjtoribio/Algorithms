vector<vector<double>> gauss(vector<vector<double>> m){
	int pivot = 0;
	for (int r = 0; r < m.size() && pivot+1 < m[0].size(); ) {
		int f = 0;
		for (int i = r; i < m.size(); ++i) {
			if(abs(m[i][pivot]) > 1e-5){
				f = 1;
				swap(m[i], m[r]);
				break;
			}
		}
		if(f){
			for (int j = m[0].size()-1; j >= pivot ; --j) {
				m[r][j] /= m[r][pivot];
			}
			for (int i = r+1; i < m.size(); ++i) {
				if(m[i][pivot]){
					for (int j = m[0].size()-1; j >= pivot; --j) {
						OPS++;
						m[i][j] -= m[i][pivot] * m[r][j];
					}
				}
			}
			r++;
		}
		pivot++;
	}
	for (int i = m.size()-1; i >= 0; --i) {
		for (int j = 0; j+1 < m[0].size(); ++j) {
			if(m[i][j]){
				for (int k = i-1; k >= 0; --k) {
					if(m[k][j]){
						for (int l = m[0].size()-1; l >= j; --l) {
							OPS++;
							m[k][l] -= m[k][j] * m[i][l];
						}
					}
				}
				break;
			}
		}
	}
	return m;
}
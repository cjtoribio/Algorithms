#define DIM 10
typedef bitset<DIM> vec;
struct VectorSpace {
	int cnt, cols;
	vector< vec > R; // rules to apply to that row
	vector< vec > V;
	VectorSpace(): cnt(0), cols(0), R(DIM), V(DIM) {
		for (int i = 0; i < DIM; ++i)
			R[i][i] = 1;
	}
	void add(vec nv) { // DIM^2 / 64
		cnt++;
		if (cols == DIM) return; // space complete
		if (nv.none()) return;
		vec nc = applyRules(nv);
		int firstOne = findFirstOne(nc, cols);
		if (firstOne == -1) return;
		nc[firstOne] = 0; nc[cols] = 1; // swap
		swap(R[firstOne], R[cols]);
		for (int i = cols+1; i < DIM; ++i) {
			if (nc[i]) {
				nc[i] = 0;
				R[i] = R[i] ^ R[cols];
			}
		}
		for (int i = 0; i < DIM; ++i) {
			V[i][cols] = nc[i];
		}
		cols++;
	}
	vec applyRules(const vec &nv) { // DIM^2 / 64
		vec ret;
		for (int i = 0; i < DIM; ++i) { // apply rules for all rows
			ret[i] = (nv & R[i]).count() & 1;
		}
		return ret;
	}
	static int findFirstOne(const vec &nv, int start = 0) { // DIM 
		for (int i = start; i < DIM; ++i) {
			if (nv[i]) return i;
		}
		return -1;
	}
	bool possible(const vec &nv) {
		vec nc = applyRules(nv);
		return findFirstOne(nc, cols) == -1;
		
	}
	int ways(const vec &nv) {
		vec nc = applyRules(nv);
		if (findFirstOne(nc, cols) != -1) return 0;
		return pow(2.0, cnt - cols); // use modPow if needed
	}
};

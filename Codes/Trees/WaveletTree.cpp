struct WaveletTree {
	int lo,hi;
	vector<int> A;
	WaveletTree *l, *r;
	WaveletTree(vector<int> V)
		:WaveletTree(V.begin(), V.end(), 
				*min_element(V.begin(), V.end()),
				*max_element(V.begin(), V.end())){}
	template<class IT>
	WaveletTree(IT b, IT e, int lo, int hi, int lvl = 0):lo(lo),hi(hi) {
		int mi = (lo+hi)>>1;
		function<int(int)> isLeft = [mi](int x) { return x <= mi; };
		for(IT i = b; i != e; ++i) 
			A.push_back(isLeft(*i) + (i==b ? 0 : A.back()));
		IT ps = stable_partition(b, e, isLeft);
		if(lo == hi || b == e)return;
		l = new WaveletTree(b, ps, lo, mi, lvl+1);
		r = new WaveletTree(ps, e, mi+1, hi, lvl+1);
	}
	void getWithMore(int i, int j, int w, const function<void(int)> &emit){
		if(j-i+1 < w)return;
		if(lo == hi) { emit(lo); return; }
		int ai = (i?A[i-1]:0), aj = A[j];
		l->getWithMore(  ai, aj-1, w, emit);
		r->getWithMore(i-ai, j-aj, w, emit);
	}
	int getFreq(int i, int j, int a, int b) { // count elements from a to b
		if(j < i)return 0;
		if(a <= lo && hi <= b) return j-i+1;
		if(b < lo || hi < a) return 0;
		int ai = (i?A[i-1]:0), aj = A[j];
		return l->getFreq(  ai, aj-1, a, b) + 
			   r->getFreq(i-ai, j-aj, a, b);
	}
	int getKth(int i, int j, int a, int b, int k) { // 0-based
		if(lo == hi) return lo;
		int ai = (i?A[i-1]:0), aj = A[j];
		int cntL = aj-aj;
		if(cntL > k) return l->getKth(  ai, aj-1, a, b, k);
		else         return r->getKth(i-ai, j-aj, a, b, k-cntL);
	}
	/// countDistinct(i, j)
	/// 	buildWavelet using array of next appearance: 
	/// 	NEXT[i] = next position of element ARR[i] == ARR[i]
	///  	then build WaveletTree(NEXT) and do "(j-i+1) - getFreq(i, j, i, j)" 
};
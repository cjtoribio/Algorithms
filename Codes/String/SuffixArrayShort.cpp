typedef vector<int> VI;
struct SuffixArray {
	int N;
	string A;
	VI SA, RA, LCP;
	SuffixArray(const string &B) :
		N(B.size()), A(B), SA(B.size()), RA(B.size()), LCP(B.size())  {
		for (int i = 0; i < N; ++i)
			SA[i] = i, RA[i] = A[i];
	}
	void countingSort(int H) {
		auto vrank = [&](int i) { return SA[i]+H<N ? RA[SA[i]+H]+1 : 0; };
		int maxRank = *max_element(RA.begin(), RA.end());
		static VI nSA(N);
		VI freq(maxRank + 2);
		for (int i = 0; i < N; ++i)
			freq[vrank(i)]++;
		for (int i = 1; i < freq.size(); ++i)
			freq[i] += freq[i-1];
		for (int i = N-1, p, m; i >= 0; --i)
			nSA[--freq[vrank(i)]] = SA[i];
		copy(nSA.begin(), nSA.end(), SA.begin());
	}
	void buildSA() {
		VI nRA(N);
		for (int H = 1; H < N; H <<= 1) {
			countingSort(H);
			countingSort(0);
			int rank = nRA[SA[0]] = 0;
			for (int i = 1; i < N; ++i) {
				if (RA[SA[i]] != RA[SA[i - 1]])
					rank++;
				else if (SA[i - 1] + H >= N || SA[i] + H >= N)
					rank++;
				else if (RA[SA[i] + H] != RA[SA[i - 1] + H])
					rank++;
				nRA[SA[i]] = rank;
			}
			copy(nRA.begin(), nRA.end(), RA.begin());
		}
	}
	void buildSA2(){
		VI T(N+3), SA(N+3);
		for(int i = 0; i < A.size(); ++i)
			T[i] = A[i];
		suffixArray(T, SA, N, 256);
		for(int i = 0; i < N; ++i)
			RA[ SA[i] ] = i;
		for(int i = 0; i < N; ++i)
			this->SA[i] = SA[i];
	}
	inline bool leq(int a1, int a2, int b1, int b2) {
		return (a1 < b1 || (a1 == b1 && a2 <= b2));
	}
	inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
		return (a1 < b1 || (a1 == b1 && leq(a2, a3, b2, b3)));
	}
	static void radixPass(VI &a, VI &b, VI::iterator r, int n, int K) {
		VI c(K+1);  
		for (int i = 0; i < n; i++)
			c[r[a[i]]]++;  
		for (int i = 1; i <= K; i++)
			c[i] += c[i-1];
		for (int i = n-1; i >= 0; --i)
			b[--c[r[a[i]]]] = a[i];
	}
	void suffixArray(VI &T, VI &SA, int n, int K) {
		int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
		VI R(n02+3), SA12(n02+3), R0(n0), SA0(n0);
		for (int i = 0, j = 0; i < n + (n0 - n1); i++)
			if (i % 3 != 0)
				R[j++] = i;
		radixPass(R, SA12, T.begin() + 2, n02, K);
		radixPass(SA12, R, T.begin() + 1, n02, K);
		radixPass(R, SA12, T.begin(), n02, K);
		int name = 0, c0 = -1, c1 = -1, c2 = -1;
		for (int i = 0; i < n02; i++) {
			if (T[SA12[i]] != c0 || T[SA12[i] + 1] != c1 || T[SA12[i] + 2] != c2) {
				name++;
				c0 = T[SA12[i]];
				c1 = T[SA12[i] + 1];
				c2 = T[SA12[i] + 2];
			}
			if (SA12[i] % 3 == 1) {
				R[SA12[i] / 3] = name;
			}
			else {
				R[SA12[i] / 3 + n0] = name;
			}
		}
		if (name < n02) {
			suffixArray(R, SA12, n02, name);
			for (int i = 0; i < n02; i++)
				R[SA12[i]] = i + 1;
		} else
			for (int i = 0; i < n02; i++)
				SA12[R[i] - 1] = i;
		for (int i = 0, j = 0; i < n02; i++)
			if (SA12[i] < n0)
				R0[j++] = 3 * SA12[i];
		radixPass(R0, SA0, T.begin(), n0, K);
		for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
			int i = GetI(); // pos of current offset 12 suffix
			int j = SA0[p]; // pos of current offset 0 suffix
			if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
			leq(T[i], R[SA12[t] + n0], T[j], R[j / 3]) : 
			leq(T[i], T[i + 1], R[SA12[t] - n0 + 1], T[j], T[j + 1], R[j / 3 + n0])) { // suffix from SA12 is smaller
				SA[k] = i;
				t++;
				if (t == n02) // done --- only SA0 suffixes left
					for (k++; p < n0; p++, k++)
						SA[k] = SA0[p];
			} else { // suffix from SA0 is smaller
				SA[k] = j;
				p++;
				if (p == n0) // done --- only SA12 suffixes left
					for (k++; t < n02; t++, k++)
						SA[k] = GetI();
			}
		}
	}
	void buildLCP() {
		for (int i = 0, k = 0; i < N; ++i) if (RA[i] != N - 1) {
			for (int j = SA[RA[i] + 1]; A[i + k] == A[j + k];) 
				++k;
			LCP[RA[i]] = k;
			if (k)--k;
		}
	}
	vector<VI> RLCP;
	void BuildRangeQueries() {
		int L = 31 - __builtin_clz(N) + 1;
		RLCP = vector<VI>(L, VI(N));
		RLCP[0] = LCP;
		for (int i = 1; i < L; ++i) {
			for (int j = (1<<(i-1)); j < N; ++j)
				RLCP[i][j] = min(RLCP[i - 1][j], RLCP[i-1][j - (1<<(i-1))]);
		}
	}
	int lcp(int i, int j) {
		if (i == j) return N - SA[i];
		int b = 31 - __builtin_clz(j-i);
		return min(RLCP[b][j], RLCP[b][i+(1<<b)]);		
	}
	long long ops = 0;
	int match(int idx, const string &P){
		for(int i = 0; i < P.size() && i + idx < N; ++i){
			ops++;
			if(A[i+idx] != P[i])
				return i;
		}
		return min(N-idx, (int)P.size());
	}
	int cmp(int idx, const string &P){
		int m = match(idx, P);
		if(m == P.size())return 0;
		if(m == N-idx)return -1;
		return A[idx+m] < P[m] ? -1 : (A[idx+m] == P[m] ? 0 : 1);
	}
	// MlogN (low constant)
	int lowerBound(const string &P){
		int lo = 0, hi = N;
		while(lo < hi){
			int mid = (lo + hi)/2;
			if(cmp(SA[mid], P) < 0)
				lo = mid+1;
			else
				hi = mid;
		}
		return lo;
	}
	// MlogN (low constant)
	int upperBound(const string &P){
		int lo = 0, hi = N;
		while(lo < hi){
			int mid = (lo + hi)/2;
			if(cmp(SA[mid], P) <= 0)
				lo = mid+1;
			else
				hi = mid;
		}
		return lo;
	}
	// M + logN (high constant) (slow in practice)
	int lowerBound2(const string &P, int deb = 0){
		int lo = 0, hi = N;
		int k = match(SA[0], P), pm = 0;
		while(lo < hi){
			int m = (lo + hi)/2;
			int rlcp= lcp(min(pm,m), max(m,pm));
			if(rlcp > k && pm != m){
				if(pm < m)lo = m+1;
				else hi = m;
			}else if(rlcp < k && pm != m){
				if(pm < m)hi = m;
				else lo = m+1;
			}else{
				while(k < P.size() && SA[m]+k < N && P[k] == A[SA[m]+k])
					k++;
				if(k == P.size())
					hi = m;
				else if(SA[m]+k == N)
					lo = m+1;
				else if(A[SA[m]+k] < P[k])
					lo = m+1;
				else 
					hi = m;
				pm = m;
			}
		}
		return lo;
	}
};
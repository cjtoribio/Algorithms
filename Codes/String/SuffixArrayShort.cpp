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
		VI freq(maxRank + 2), nSA(N);
		for (int i = 0; i < N; ++i)
			freq[vrank(i)]++;
		for (int i = 0, sum = 0, t; i < freq.size(); ++i)
			t = freq[i], freq[i] = sum, sum += t;
		for (int i = 0, p, m; i < N; ++i)
			nSA[freq[vrank(i)]++] = SA[i];
		SA = move(nSA);
	}
	void BuildSA() {
		for (int H = 1; H < N; H <<= 1) {
			countingSort(H);
			countingSort(0);
			VI nRA(N);
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
			RA = move(nRA);
		}
	}
	void BuildLCP() {
		VI PLCP(N), PHI(N);
		PHI[SA[0]] = -1;
		for (int i = 1; i < N; ++i)
			PHI[SA[i]] = SA[i - 1];
		for (int i = 0, L = 0; i < N; ++i) {
			if (PHI[i] == -1) {
				PLCP[i] = 0;
				continue;
			}
			while (PHI[i] + L < N && i + L < N && A[i + L] == A[PHI[i] + L])
				L++;
			PLCP[i] = L;
			L = max(L - 1, 0);
		}
		for (int i = 1; i < N; ++i)
			LCP[i] = PLCP[SA[i]];
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
};
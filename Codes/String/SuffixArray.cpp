/*
 * SuffixArray.cpp
 *
 *  Created on: Nov 12, 2011
 *      Author: carlosjosetoribio
 */

#include<iostream>

using namespace std;

// Source: https://www.cs.helsinki.fi/u/tpkarkka/publications/jacm05-revised.pdf
struct SuffixArray {
	string S;
	int *T, *SA, *RA, N, *LCP;
	SuffixArray(string S):S(S),N(S.size()){
		T = new int[N+3];
		SA= new int[N+3];
		RA= new int[N+3];
		for(int i = 0; i < S.size(); ++i)
			T[i] = S[i];
		T[N] = T[N+1] = T[N+3] = 0;
		suffixArray(T, SA, N, 256);
		for(int i = 0; i < N; ++i)
			RA[ SA[i] ] = i;
	}
	inline bool leq(int a1, int a2, int b1, int b2) // lexicographic order
			{
		return (a1 < b1 || a1 == b1 && a2 <= b2);
	} // for pairs
	inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
		return (a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3));
	} // and triples
	// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
	static void radixPass(int* a, int* b, int* r, int n, int K) { // count occurrences
		int* c = new int[K + 1]; // counter array
		for (int i = 0; i <= K; i++)
			c[i] = 0; // reset counters
		for (int i = 0; i < n; i++)
			c[r[a[i]]]++; // count occurrences
		for (int i = 0, sum = 0; i <= K; i++) // exclusive prefix sums
				{
			int t = c[i];
			c[i] = sum;
			sum += t;
		}
		for (int i = 0; i < n; i++)
			b[c[r[a[i]]]++] = a[i]; // sort
		delete[] c;
	}
	// find the suffix array SA of T[0..n-1] in {1..K}^n
	// require T[n]=T[n+1]=T[n+2]=0, n>=2
	void suffixArray(int* T, int* SA, int n, int K) {
		int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
		int* R = new int[n02 + 3];
		R[n02] = R[n02 + 1] = R[n02 + 2] = 0;
		int* SA12 = new int[n02 + 3];
		SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
		int* R0 = new int[n0];
		int* SA0 = new int[n0];
		//******* Step 0: Construct sample ********
		// generate positions of mod 1 and mod 2 suffixes
		// the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
		for (int i = 0, j = 0; i < n + (n0 - n1); i++)
			if (i % 3 != 0)
				R[j++] = i;
		//******* Step 1: Sort sample suffixes ********
		// lsb radix sort the mod 1 and mod 2 triples
		radixPass(R, SA12, T + 2, n02, K);
		radixPass(SA12, R, T + 1, n02, K);
		radixPass(R, SA12, T, n02, K);
		// find lexicographic names of triples and
		// write them to correct places in R
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
			} // write to R1
			else {
				R[SA12[i] / 3 + n0] = name;
			} // write to R2
		}
		// recurse if names are not yet unique
		if (name < n02) {
			suffixArray(R, SA12, n02, name);
			// store unique names in R using the suffix array
			for (int i = 0; i < n02; i++)
				R[SA12[i]] = i + 1;
		} else
			// generate the suffix array of R directly
			for (int i = 0; i < n02; i++)
				SA12[R[i] - 1] = i;
		//******* Step 2: Sort nonsample suffixes ********
		// stably sort the mod 0 suffixes from SA12 by their first character
		for (int i = 0, j = 0; i < n02; i++)
			if (SA12[i] < n0)
				R0[j++] = 3 * SA12[i];
		radixPass(R0, SA0, T, n0, K);
		//******* Step 3: Merge ********
		// merge sorted SA0 suffixes and sorted SA12 suffixes
		for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
	#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
			int i = GetI(); // pos of current offset 12 suffix
			int j = SA0[p]; // pos of current offset 0 suffix
			if (SA12[t] < n0 ? // different compares for mod 1 and mod 2 suffixes
			leq(T[i], R[SA12[t] + n0], T[j], R[j / 3]) : leq(T[i], T[i + 1],
					R[SA12[t] - n0 + 1], T[j], T[j + 1], R[j / 3 + n0])) { // suffix from SA12 is smaller
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
		delete[] R;
		delete[] SA12;
		delete[] SA0;
		delete[] R0;
	}
	void BuildLCP() {
		int *PLCP = new int[N];
		int *PHI = new int[N];
		LCP = new int[N];
		PHI[SA[0]] = -1;
		for (int i = 1; i < N; ++i)
			PHI[SA[i]] = SA[i - 1];
		for (int i = 0, L = 0; i < N; ++i) {
			if (PHI[i] == -1) {
				PLCP[i] = 0;
				continue;
			}
			while (PHI[i] + L < N && i + L < N && T[i + L] == T[PHI[i] + L])
				L++;
			PLCP[i] = L;
			L = max(L - 1, 0);
		}
		for (int i = 1; i < N; ++i)
			LCP[i] = PLCP[SA[i]];
		delete[] PLCP;
		delete[] PHI;
	}
	int **RLCP, *LOG2;
	void BuildRangeQueries(){
		int L = 0; while((1<<L) <= N)L++;
		RLCP = new int*[N];
		for(int i = 0; i < N; ++i){
			RLCP[i] = new int[L];
			RLCP[i][0] = LCP[i];
		}
		for(int i = 1; i < L; ++i){
			for(int j = 0; j < N; ++j)
				RLCP[j][i] = min( RLCP[j][i-1] , j<(1<<i)?0:RLCP[j-(1<<(i-1))][i-1] );
		}
		LOG2 = new int[N];
		memset(LOG2, -1, sizeof(int) * N);
		for(int i = 0; (1<<i) < N; ++i){
			LOG2[1<<i] = i;
		}
		for(int i = 1; i < N; ++i){
			if(LOG2[i] == -1){
				LOG2[i] = LOG2[i-1];
			}
		}
	}
	int lcp(int i,int j){
		if(i == j)return N - SA[i];
		int l = LOG2[j-i];
		return min( RLCP[j][l] , RLCP[i+(1<<l)][l] );
	}
	int lower_bound(string &B) {
		int lo = 0, hi = N - 1, idx = 0, las = -1;
		while (lo < hi) {
			int mid = (lo + hi) / 2, greater, inc = 0, skip = 0;
			if (idx) {
				if (las == 0 && lcp(lo, mid) < idx) {
					greater = 0;
					skip = 1;
				}
				if (las == 1 && lcp(mid, hi) < idx) {
					greater = 1;
					skip = 1;
				}
			}
			if (!skip) {
				while (SA[mid] + idx < N && T[SA[mid] + idx] == B[idx]) {
					idx++;
					inc = 1;
				}
				greater =
						B.size() == idx ?
								0 :
								(SA[mid] + idx >= N || T[SA[mid] + idx] < B[idx]);
			}
			if (greater) {
				lo = mid + 1;
				if (inc)
					las = 0;
			} else {
				hi = mid;
				if (inc)
					las = 1;
			}
		}
		return lo;
	}
	int count(string &B){
		int st = lower_bound(B);
		if(N-SA[st] < B.size())return 0;
		for(int i = SA[st], j = 0; j < B.size(); ++i, j++){
			if(T[i] != B[j])
				return 0;
		}
		int lo = st, hi = N;
		while(lo < hi){
			int mid = (lo+hi)/2;
			if( lcp(st,mid) >= B.size() ){
				lo = mid+1;
			}else{
				hi = mid;
			}
		}
		return lo - st;
	}
};

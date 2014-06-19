const int MAXN = 200010;
struct SuffixArray
{
	 string A;
	 int N;
	 int SA[MAXN] , RA[MAXN] , LCP[MAXN];
	 SuffixArray(string &B)
	 {
	 	 A = B; N = A.size();
	 	 for(int i = 0; i < N; ++i)
		 	 SA[i] = i , RA[i] = A[i];
	 }
	 void countingSort(int H)
	 {
	 	 int maxn = max(N,300)+N;
	 	 int freq[MAXN*2+300] , nSA[N]; memset(freq,0,sizeof(freq));
	 	 for(int i = 0; i < N; ++i)
		 	 freq[ SA[i]+H<N ? RA[SA[i]+H]+N : N-1-SA[i] ]++;
	 	 for(int i = 0,sum = 0,t; i < maxn; ++i)
		 	 t = freq[i] , freq[i] = sum , sum += t;
	 	 for(int i = 0,p,m; i < N ; ++i)
		 	 nSA[ freq[ (SA[i]+H<N)?(RA[SA[i]+H]+N):(N-1-SA[i])]++ ] =SA[i];
	 	 memcpy(SA,nSA,sizeof(nSA));
	 }
	 void BuildSA()
	 {
	 	 for(int H = 1; H < A.size() ; H<<=1)
	 	 {
		 	 countingSort(H);
		 	 countingSort(0);
		 	 int nRA[N] , rank = nRA[ SA[0] ] = 0;
		 	 for(int i = 1; i < N; ++i)
		 	 {
		 	 	 if(RA[ SA[i] ] != RA[ SA[i-1] ])rank++;
		 	 	 else if(SA[i-1]+H >= N || SA[i]+H >= N)rank++;
		 	 	 else if(RA[ SA[i]+H ] != RA[ SA[i-1]+H ])rank++;
		 	 	 nRA[ SA[i] ] = rank;
		 	 }
		 	 memcpy(RA,nRA,sizeof(nRA));
	 	 }
	 }
	 void BuildLCP()
	 {
	 	 int PLCP[MAXN];
	 	 int PHI[MAXN];
	 	 PHI[ SA[0] ] = -1;
	 	 for(int i = 1; i < N; ++i)
		 	 PHI[ SA[i] ] = SA[i-1];
	 	 for(int i = 0, L = 0; i < N; ++i)
	 	 {
		 	 if(PHI[i]==-1){PLCP[i] = 0;continue;}
		 	 while(PHI[i]+L < N && i+L < N && A[i+L] == A[ PHI[i]+L ])L++;
		 	 PLCP[i] = L;
		 	 L = max(L-1 , 0);
	 	 }
	 	 for(int i = 1; i < N; ++i)
		 	 LCP[i] = PLCP[ SA[i] ];
	 }
	 pair<int,int> Match(string &B)
	 {
	 	 int lo = 0, hi = N-1;
	 	 for(int idx = 0; idx < B.size(); ++idx)
	 	 {
		 	 while(SA[lo]+idx>=N || A[SA[lo]+idx]<B[idx])
		 	 {
		 	 	 if(lo<N && LCP[lo+1] > idx-1)lo++;
		 	 	 else break;
		 	 }
		 	 while(SA[hi]+idx>=N || A[SA[hi]+idx]>B[idx])
		 	 {
		 	 	 if(hi>0 && LCP[hi] > idx-1)hi--;
		 	 	 else break;
		 	 }
	 	 }
	 	 return pair<int,int>(lo,hi);
	 }
};

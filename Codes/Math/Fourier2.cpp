namespace Fourier {
	#define lowbit(x) (((x) ^ (x-1)) & (x))
	typedef complex<long double> Complex;
	
	void FFT(vector<Complex> &A, int s){
		int n = A.size();
		int p = __builtin_ctz(n);
		
		vector<Complex> a = A;
		
		for(int i = 0;i<n;++i){
			int rev = 0;
			for(int j = 0;j<p;++j){
				rev <<= 1;
				rev |= ((i >> j) & 1);
			}
			A[i] = a[rev];
		}
		
		Complex w,wn;
		
		for(int i = 1;i<=p;++i){
			int M = (1<<i), K = (M>>1);
			wn = Complex(cos(s*2.0*M_PI/(long double)M), sin(s*2.0*M_PI/(long double)M));
			
			for(int j = 0;j<n;j += M){
				w = Complex(1.0, 0.0);
				for(int l = j;l<K+j;++l){
					Complex t = w;
					t *= A[l + K];
					Complex u = A[l];
					A[l] += t;
					u -= t;
					A[l + K] = u;
					w *= wn;
				}
			}
		}
		
		if(s==-1){
			for(int i = 0;i<n;++i)
				A[i] /= (long double)n;
		}
	}
	vector<Complex> FFT_Multiply(vector<Complex> &P, vector<Complex> &Q){
		int n = P.size()+Q.size();
		while(n!=lowbit(n)) n += lowbit(n);
		
		P.resize(n,0);
		Q.resize(n,0);
		
		FFT(P,1);
		FFT(Q,1);
		
		vector<Complex> R;
		for(int i=0;i<n;i++) R.push_back(P[i]*Q[i]);
		
		FFT(R,-1);
		
		return R;
	}
	vector<Long> FFT_Multiply(vector<Long> &A, vector<Long> &B){
		vector<Complex> AF(A.size());
		for(int i = 0; i < A.size(); ++i){
			AF[i] = Complex(A[i], 0);
		}
		vector<Complex> BF(B.size());
		for(int i = 0; i < B.size(); ++i){
			BF[i] = Complex(B[i], 0);
		}
		vector<Complex> CF = FFT_Multiply(AF,BF);
		vector<Long> C;
		for (int i = 0; i < CF.size(); ++i) {
			C.push_back(CF[i].real()+0.1);
		}
		return C;
	}
}
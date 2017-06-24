namespace EducationalFFT {
	/*
	X0,...,N−1 ← ditfft2(x, N, s):             DFT of (x0, xs, x2s, ..., x(N-1)s):
		if N = 1 then
			X0 ← x0                                      trivial size-1 DFT base case
		else
			X0,...,N/2−1 ← ditfft2(x, N/2, 2s)             DFT of (x0, x2s, x4s, ...)
			XN/2,...,N−1 ← ditfft2(x+s, N/2, 2s)           DFT of (xs, xs+2s, xs+4s, ...)
			for k = 0 to N/2−1                           combine DFTs of two halves into full DFT:
				t ← Xk
				Xk ← t + exp(−2πi s k/N) Xk+N/2
				Xk+N/2 ← t − exp(−2πi s k/N) Xk+N/2
			endfor
		endif
	 */
	typedef complex<long double> Complex;
	long double PI = acos(-1.0L);
	struct ExpBucket {
		vector<Complex> E, I;
		int N;
		ExpBucket(int N):E(N), I(N),N(N){
			for(int i = 0; i < N; ++i){
				E[i] = exp(Complex(0, -2.0L*PI*i/N));
				I[i] = exp(Complex(0,  2.0L*PI*i/N));
			}
		}
		Complex get(int i, int n, int inv){
			i *= N / n;
			return inv == 1 ? E[i] : I[i];
		}
	};
	vector<Complex> dft(ExpBucket &E, const vector<Complex> &x, int n, int inv = 1, int st = 0, int s = 1) {
		if(n == 1){
			return vector<Complex>(1, x[st]);
		}
		vector<Complex> X0 = dft(E, x, n/2, inv, st, 2*s);
		vector<Complex> X1 = dft(E, x, n/2, inv, st + s, 2*s);
		vector<Complex> X;
		X.insert(X.end(), X0.begin(), X0.end());
		X.insert(X.end(), X1.begin(), X1.end());
		for(int k = 0; k <= n/2-1; ++k){
			Complex a = X[k];
			Complex b = X[k + n/2];
			/* sin(2*pi*s*i/n) */
			long double ang = -2*PI*k/n*inv;
			Complex eb = exp(Complex(0, ang)) * b;
			X[k] = a + eb ;
			X[k+n/2] = a - eb;
		}
		return X;	
	}
	vector<Complex> convolution(vector<Complex> A, vector<Complex> B){
		int n = max(A.size(), B.size());
		reverse(B.begin(),B.end());
		while( n != (n & -n))n++;
		n <<= 1;
		A.resize(n);
		B.resize(n);
		ExpBucket E(n);
		A = dft(E, A, A.size(), 1);
		B = dft(E, B, B.size(), 1);
		for (int i = 0; i < A.size(); ++i) {
			A[i] = A[i] * B[i];
		}
		vector<Complex> C = dft(E, A, A.size(), -1);
		for (Complex &v : C) {
			if(abs(v.real()) < 1e-9)v.real(0);
			if(abs(v.imag()) < 1e-9)v.imag(0);
			v.real(v.real() / A.size());
			v.imag(v.imag() / A.size());
			assert(abs(v.imag()) < 0.2);
			assert(abs(v.real() - round(v.real())) < 0.2);
		}
		return C;
	}
	vector<Complex> convolution2(vector<Complex> A, vector<Complex> B){
		int n = max(A.size(), B.size());
		reverse(B.begin(),B.end());
		while( n != (n & -n))n++;
		n <<= 1;
		A.resize(n);
		B.resize(n);
		ExpBucket E(n);
		vector<Complex> T(n);
		for (int i = 0; i < n; ++i) {
			T[i] = Complex(A[i].real(), B[i].real());
		}
		T = dft(E, T, T.size(), 1);
		vector<Complex> T2(n);
		for (int i = 0; i < n; ++i) {
			Complex X = (T[i] + conj(T[(n - i) % n])) / Complex(2, 0);
			Complex Y = (T[i] - conj(T[(n - i) % n])) / Complex(0, 2);
			T2[i] = X*Y;
		}
		vector<Complex> C = dft(E, T2, T2.size(), -1);
		for (Complex &v : C) {
			if(abs(v.real()) < 1e-9)v.real(0);
			if(abs(v.imag()) < 1e-9)v.imag(0);
			v.real(v.real() / A.size());
			v.imag(v.imag() / A.size());
			assert(abs(v.imag()) < 0.2);
			assert(abs(v.real() - round(v.real())) < 0.2);
		}
		return C;
	}
}
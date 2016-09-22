namespace FFT {
	typedef long double DOUBLE;
	typedef long long Long;
	struct Complex {
		DOUBLE x,y;
		Complex(DOUBLE x=0, DOUBLE y=0):x(x),y(y){}
		Complex operator*(const Complex &C){
			return Complex(x*C.x-y*C.y, x*C.y+y*C.x);
		}
		Complex operator+(const Complex &C)const{
			return Complex(x+C.x,y+C.y);
		}
		Complex operator-(const Complex &C)const{
			return Complex(x-C.x,y-C.y);
		}
		Complex operator/=(const DOUBLE &d){
			x /= d; y /= d; return (*this);
		}
		Complex operator/(const Complex &C)const{
			DOUBLE m = C.x*C.x + C.y*C.y;
			return Complex(x/m,y/m)*Complex(C.x,-C.y);
		}
		Complex conjugate(){
			return Complex(x,-y);
		}
	};
	ostream& operator<<(ostream &o , const Complex c){
		o <<"("<< c.x << " " << c.y << ")";
		return o;
	}
	Long mround(DOUBLE A){
		Long F = A;
		if(A < 0)F = (Long)(A-0.5);
		if(A > 0)F = (Long)(A+0.5);
		return F;
	}
	typedef Complex COMPLEX;
	typedef vector<DOUBLE> VD;
	typedef vector<Long> VL;
	typedef vector<COMPLEX> VC;
	int cat[22];
	COMPLEX mem[22];
	DOUBLE MPI = 3.1415926535897932384626433832795028841971693993751058209749445923L;
	COMPLEX exp2(int i){
		if(cat[i])return mem[i];
		cat[i] = 1;
		DOUBLE a = 2.0L*MPI/(1LL<<i);
		return mem[i] = Complex(cos(a), sin(a));
	}
	struct FFT {
		int n, L;
		VC A;
		int ReverseBits(int k) {
			int ret = 0;
			for (int i = 0; i < L; i++) {
				ret = (ret << 1) | (k & 1);
				k >>= 1;
			}
			return ret;
		}
		void BitReverseCopy(const VC &a) {
			for (n = 1, L = 0; n < a.size(); n <<= 1, L++);
			A.resize(n);
			for (int k = 0; k < n; k++)
				A[ReverseBits(k)] = a[k];
		}
		VC DFT(const VC &a, bool inverse) {
			BitReverseCopy(a);
			for (int s = 1; s <= L; s++) {
				int m = 1 << s;
				COMPLEX wm = exp2(s);
				if (inverse)
					wm = COMPLEX(1, 0) / wm;
				for (int k = 0; k < n; k += m) {
					COMPLEX w = 1;
					for (int j = 0; j < m / 2; j++) {
						COMPLEX t = w * A[k + j + m / 2];
						COMPLEX u = A[k + j];
						A[k + j] = u + t;
						A[k + j + m / 2] = u - t;
						w = w * wm;
					}
				}
			}
			if (inverse)
				for (int i = 0; i < n; i++)
					A[i] /= n;
			return A;
		}
	
	} f;

	VD Convolution(const VD &a,const VD &b) {
		int L = 1;
		while ((1 << L) < a.size())
			L++;
		while ((1 << L) < b.size())
			L++;
		int n = 1 << (L + 1);
		VC p(n);
		for (size_t i = 0; i < n; i++){
			p[i] = Complex(
				i < a.size() ? a[i] : 0, 
				i < b.size() ? b[i] : 0
			);
		}
		VC P = f.DFT(p, false);
		VC CC(n);
		for (size_t i = 0; i < n; i++){
			Complex A = (P[i]+P[(n-i)%n].conjugate())/Complex(2,0);
			Complex B = (P[i]-P[(n-i)%n].conjugate())/Complex(0,2);
			CC[i] = A*B;
		}
		
		VC cc = f.DFT(CC, true);

		VD c(a.size()+b.size()-1);
		for (int i = 0; i < a.size() + b.size() - 1; i++)
			c[i] = (cc[i].x);
		return c;
	}
	VI BigConvolution(const VI &a, const VI &b, Long MOD){
		int ssss = a.size()+b.size()-1;
	    if (ssss <= 1024) {
	    	vector<int> bf(ssss);
	    	for(int i = 0; i < a.size(); ++i){
	    		for(int j = 0; j < b.size(); ++j){
	    			bf[i+j] = (bf[i+j] + 1LL*a[i]*b[j])%MOD;
	    		}
	    	}
	    	return bf;
	    }
		int L = 1;
		while ((1 << L) < a.size()) L++;
		while ((1 << L) < b.size()) L++;
		int n = 1 << (L + 1);
		Long SH  = 16;
		Long M  = (1<<SH)-1, S = 1<<SH;
		Long S1 = S;
		Long S2 = S*S % MOD;
		VC xy1(n),xy0(n);
		for(int i = 0; i < a.size(); ++i){
			xy1[i].x = a[i] >> SH;
			xy0[i].x = a[i]  &  M;
		}
		for(int i = 0; i < b.size(); ++i){
			xy1[i].y = b[i] >> SH;
			xy0[i].y = b[i]  &  M;
		}
		VC XY1 = f.DFT(xy1,false);
		VC XY0 = f.DFT(xy0,false);
		VC Z2(n), Z1(n), Z0(n);
	    for(int i = 0; i < n; i++)
	    {
	        Complex X0 = (XY0[i] + XY0[(n - i) % n].conjugate()) / Complex(2, 0);
	        Complex X1 = (XY1[i] + XY1[(n - i) % n].conjugate()) / Complex(2, 0);
	        Complex Y0 = (XY0[i] - XY0[(n - i) % n].conjugate()) / Complex(0, 2);
	        Complex Y1 = (XY1[i] - XY1[(n - i) % n].conjugate()) / Complex(0, 2);
	        Z2[i] = X1*Y1;
	        Z1[i] = X0*Y1 + Y0*X1;
	        Z0[i] = X0*Y0;
	    }
		
		
		VC z2  = f.DFT(Z2,true);
		VC z1  = f.DFT(Z1,true);
		VC z0  = f.DFT(Z0,true);
		
		VI z(a.size() + b.size() - 1);
		for(int i = 0; i < z.size(); ++i){
			Long dz2 = mround(z2[i].x) % MOD;
			Long dz1 = mround(z1[i].x) % MOD;
			Long dz0 = mround(z0[i].x) % MOD;
			Long l = (dz2 * S2) + (dz1 * S1) + dz0;
			z[i] = l % MOD;
		}
		return z;
	}
}
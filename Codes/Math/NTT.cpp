namespace NTT{
	struct NTTMod {
		int mod, root, root_1, root_pw;
	};
	static int modPow(int b, int e, int m){
		if(e == 0)return 1;
		int sq = modPow(b,e>>1,m);
		sq = (1LL*sq*sq) % m;
		if(e&1)sq = (1LL*sq*b)%m;
		return sq;
	}
	static int modInv(int b,int m){
		return modPow(b,m-2,m);
	}
	static const NTTMod suggested_fft_mods[5] = {
		{ 7340033, 5, 4404020, 1 << 20 },
		{ 415236097, 73362476, 247718523, 1<<22 },
		{ 463470593, 428228038, 182429, 1<<21},
		{ 998244353, 15311432, 469870224, 1 << 23 },
		{ 918552577, 86995699, 324602258, 1 << 22 }
	};
	static void ntt (vector<int> & a, bool invert, const NTTMod &mod) {
		int n = (int) a.size();
		for (int i=1, j=0; i<n; ++i) {
			int bit = n >> 1;
			for (; j>=bit; bit>>=1)
				j -= bit;
			j += bit;
			if (i < j)
				swap (a[i], a[j]);
		}
		for (int len=2; len<=n; len<<=1) {
			int wlen = invert ? mod.root_1 : mod.root;
			for (int i=len; i<mod.root_pw; i<<=1)
				wlen = int (wlen * 1ll * wlen % mod.mod);
			for (int i=0; i<n; i+=len) {
				int w = 1;
				for (int j=0; j<len/2; ++j) {
					int u = a[i+j],  v = int (a[i+j+len/2] * 1ll * w % mod.mod);
					a[i+j] = u+v < mod.mod ? u+v : u+v-mod.mod;
					a[i+j+len/2] = u-v >= 0 ? u-v : u-v+mod.mod;
					w = int (w * 1ll * wlen % mod.mod);
				}
			}
		}
		if (invert) {
			int nrev = modInv(n, mod.mod);
			for (int i=0; i<n; ++i)
				a[i] = int (a[i] * 1ll * nrev % mod.mod);
		}
	}
	// this function is for Chinese Remainder Theorem it can be extracted if needed.
	// pair<int,int> means { value, smallMod }
	static long long crt(const std::vector< std::pair<int, int> >& a, int mod) {
		long long res = 0;
		long long mult = 1;

		int SZ = a.size();
		std::vector<int> x(SZ);
		for (int i = 0; i<SZ; ++i) {
			x[i] = a[i].first;
			for (int j = 0; j<i; ++j) {
				long long cur = (x[i] - x[j]) * 1ll * modInv(a[j].second,a[i].second);
				x[i] = (int)(cur % a[i].second);
				if (x[i] < 0) x[i] += a[i].second;
			}
			res = (res + mult * 1ll * x[i]);
			mult = (mult * 1ll * a[i].second);
			if (mod != -1) {
				res %= mod;
				mult %= mod;
			}
		}

		return res;
	}
	static vector<int> ntt_mult(const vector<int>& left, const vector<int>& right, const NTTMod& mod_data) {
	    vector<int> left1 = left, right1 = right;
	    ntt(left1, false, mod_data);
	    ntt(right1, false, mod_data);

	    for (int i = 0; i < left.size(); i++) {
	        left1[i] = (left1[i] * 1ll * right1[i]) % mod_data.mod;
	    }

	    ntt(left1, true, mod_data);
	    return left1;
	}
	static vector<int> mult(vector<int>& left, vector<int>& right, int mod) {
	    int ssss = left.size() + right.size() - 1;
	    if (ssss <= 1024) {
	    	vector<int> bf(ssss);
	    	for(int i = 0; i < left.size(); ++i){
	    		for(int j = 0; j < right.size(); ++j){
	    			bf[i+j] = (bf[i+j] + 1LL*left[i]*right[j])%mod;
	    		}
	    	}
	    	return bf;
	    }
	    
	    int pot2 , fsz = left.size() + right.size() - 1;
	    for (pot2 = 1; pot2 < ssss; pot2 <<= 1);

	    left.resize(pot2);
	    right.resize(pot2);

	    vector<int> res[3];
	    for (int i = 0; i < 3; i++) {
	        res[i] = ntt_mult(left, right, suggested_fft_mods[i]);
	    }

	    vector<int> ret(pot2);
	    for (int i = 0; i < pot2; i++) {
	        vector< pair<int,int> > mod_results;
	        for (int j = 0; j < 3; j++) {
	            mod_results.emplace_back(res[j][i], suggested_fft_mods[j].mod);
	        }
	        ret[i] = crt(mod_results, mod);
	    }
	    ret.resize(fsz);
	    return ret;
	}
};

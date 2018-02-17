template<Long MOD> 
struct ModInt {
	int n;
	ModInt(const ModInt<MOD> &v):n(v.n){ }
	ModInt():n(0){}
	ModInt(Long nn){
		if(nn < -MOD || nn >= MOD)nn %= MOD;
		if(nn < 0)nn += MOD;
		n = nn;
	}
	ModInt<MOD> operator+(const ModInt<MOD> &M)const{
		int r = (n + M.n);
		if(r >= MOD)r -= MOD;
		return ModInt::safe(r);
	}
	ModInt<MOD> operator-(const ModInt<MOD> &M)const{
		int r = (n - M.n);
		if(r < 0)r += MOD;
		return ModInt::safe(r);
	}
	ModInt<MOD> operator*(const ModInt<MOD> &M)const{
		return ModInt::safe(((Long)n * M.n) % MOD);
	}
	ModInt<MOD> operator+=(const ModInt<MOD> &M){
		return ModInt::safe(n = ((*this)+(M)).n);
	}
	ModInt<MOD> operator-=(const ModInt<MOD> &M){
		return ModInt::safe(n = ((*this)-(M)).n);
	}
	ModInt<MOD> operator/(const ModInt<MOD> &B)const
	{
		Long a = B.n, b = MOD;
		Long r = a , o_r = b;
		Long s = 0 , o_s = 1;
		Long t = 1 , o_t = 0;
		while(r != 0)
		{
			Long q = o_r / r;
			Long tem;
			
			tem = r;
			r = o_r - r * q;
			o_r = tem;
			
			tem = o_s;
			o_s = o_s - s * q;
			o_s = tem;
			
			tem = t;
			t = o_t - t * q;
			o_t = tem;
		}
		return (*this) * ModInt(o_t);
	}
	ModInt<MOD> power(const ModInt<MOD-1> &B){
		if(B.n == 0)return 1;
		ModInt<MOD> sq = power(B.n/2);
		sq = sq * sq;
		if(B.n & 1)sq = sq * (*this);
		return sq;
	}
	inline static ModInt<MOD> safe(Long n){
		ModInt<MOD> m;
		m.n = n;
		return m;
	}
};
typedef ModInt<1000000007> mint;

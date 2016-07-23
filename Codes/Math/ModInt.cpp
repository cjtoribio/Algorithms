template<Long MOD> 
struct ModInt {
	Long n;
	ModInt(Long n = 0){
		this->n = n;
		if(this->n < 0 || this->n >= MOD)this->n %= MOD;
		if(this->n < 0)this->n = (this->n + MOD) % MOD;
	}
	ModInt<MOD> operator+(const ModInt<MOD> &M)const{
		ModInt r = (n + M.n);
		if(r.n >= MOD)r.n -= MOD;
		return r;
	}
	ModInt<MOD> operator-(const ModInt<MOD> &M)const{
		ModInt<MOD> r = (n - M.n) % MOD;
		if(r.n < 0)r.n += MOD;
		return r;
	}
	ModInt<MOD> operator*(const ModInt<MOD> &M)const{
		return (1LL * n * M.n) % MOD;
	}
	ModInt<MOD> operator+=(const ModInt<MOD> &M){
		return this->n = ((*this)+(M)).n;
	}
	ModInt<MOD> operator-=(const ModInt<MOD> &M){
		return this->n = ((*this)-(M)).n;
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
};
typedef ModInt<1000000007> mint;
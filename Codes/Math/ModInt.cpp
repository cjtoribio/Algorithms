
template<Long MOD> 
struct ModInt {
	Long n;
	ModInt(Long n = 0){
		this->n = n;
		if(this->n < 0 || this->n >= MOD)this->n %= MOD;
		if(this->n < 0)this->n = (this->n + MOD) % MOD;
	}
	ModInt operator+(const ModInt &M)const{
		ModInt r = (n + M.n);
		if(r.n >= MOD)r.n -= MOD;
		return r;
	}
	ModInt operator-(const ModInt &M)const{
		ModInt r = (n - M.n) % MOD;
		if(r.n < 0)r.n += MOD;
		return r;
	}
	ModInt operator*(const ModInt &M)const{
		return (1LL * n * M.n) % MOD;
	}
	ModInt operator+=(const ModInt &M){
		return this->n = ((*this)+(M)).n;
	}
	ModInt operator-=(const ModInt &M){
		return this->n = ((*this)-(M)).n;
	}
};
typedef ModInt<1000000007> mint;
 
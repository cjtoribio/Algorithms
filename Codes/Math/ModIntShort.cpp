template<int MOD>
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
        return ModInt::safe(n + M.n >= MOD ? (n + M.n - MOD) : (n + M.n));
    }
    ModInt<MOD> operator-(const ModInt<MOD> &M)const{
        return ModInt::safe(n >= M.n ? (n - M.n) : (n - M.n + MOD));
    }
    ModInt<MOD> operator*(const ModInt<MOD> &M)const{
        return ModInt::safe(((Long)n * M.n) % MOD);
    }
    ModInt<MOD> operator+=(const ModInt<MOD> &M) {
        return ModInt::safe(n = ((*this)+(M)).n);
    }
    ModInt<MOD> operator-=(const ModInt<MOD> &M) {
        return ModInt::safe(n = ((*this)-(M)).n);
    }
    ModInt<MOD> operator*=(const ModInt<MOD> &M) {
        return ModInt::safe(n = ((*this)*(M)).n);
    }
    inline static ModInt<MOD> safe(int n) {
        ModInt<MOD> m;
        m.n = n;
        return m;
    }
};
typedef ModInt<1000000007> mint;

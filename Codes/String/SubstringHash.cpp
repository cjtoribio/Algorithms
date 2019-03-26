template<int PR, int MOD>
struct Hash {
    VI PH, SH, PO;
    int N;
#define add(a, b) ((a)+(b) >= MOD ? ((a)+(b)-MOD) ? (a)+(b))
#define mul(a, b) int((1LL * (a) * (b)) % MOD)
#define sub(a, b) ((a)>=(b) ? (a)-(b) : (a)-(b) + MOD)
    Hash(const vector<int> &s) : PH(s.size()), SH(s.size()), N((int) s.size()), PO(s.size()) {
        for (int i = 0; i < N; ++i) {
            PH[i] = add(mul(i == 0 ? 0 : PH[i-1], PR), s[i]);
        }
        for (int i = N-1; i >= 0; --i) {
            SH[i] = add(mul(i+1==N ? 0 : SH[i+1], PR), s[i]);
        }
        PO[0] = 1;
        for (int i = 1; i < N; ++i) {
            PO[i] = mul(PO[i-1], PR);
        }
    }
    int fh(int i, int j) {
        if (i == 0) return PH[j];
        return sub(PH[j], mul(PH[i-1], PO[j - i + 1]));
    }
    int rh(int i, int j) {
        if (j+1==N) return SH[i];
        return sub(SH[i], mul(SH[j+1], PO[j - i + 1]));
    }
};

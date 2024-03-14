template<class T, class U>
struct SegmentTree {
    T *V;
    U *D;
    int *SZ;
    bool *L;
    int N, H;
    SegmentTree(int N) : V(new T[2*N]), D(new U[N]), L(new bool[N]), SZ(new int[2*N]), N(N), H(sizeof(int) * 8 - __builtin_clz(N)) {
        memset(L, 0, sizeof(bool) * N);
        for (int i = N-1; i > 0; --i) D[i] = U();
        for (int i = 2*N-1; i > 0; --i) SZ[i] = i < N ? SZ[i<<1] + SZ[i<<1|1] : 1;
    }
    ~SegmentTree() { delete[] V; delete[] L; delete[] D; }
    void calc(int p) {
        V[p] = V[p<<1] + V[p<<1|1];
        if (L[p]) V[p] += D[p] * SZ[p];
    }

    void apply(int p, const U &value) {
        V[p] += value * SZ[p];
        if (p < N) D[p] = value;
    }
    inline void build(int l, int r) {
        for (l += N, r += N-1; l > 1;) {
            l >>= 1, r >>= 1;
            for (int i = r; i >= l; --i) {
//                cout << i << endl;
                calc(i);
            }
        }
    }

    inline void pushToChildren(int i) {
        if (i < N && L[i]) apply(i<<1, D[i]), apply(i<<1|1, D[i]), D[i] = U(), L[i] = false;
    }
    void push(int l, int r) {
        int s = H, k = 1 << (H-1);
        for (l += N, r += N-1; s > 0; --s, k >>= 1)
            for (int i = l >> s; i <= r >> s; ++i) if (L[i]) {
                    pushToChildren(i);
                }
    }
    template<class I>
    void create(const vector<I> &init) {  // build the tree
        for (int i = 0; i < N; ++i) V[i+N] = init[i];
        for (int i = N - 1; i > 0; --i) V[i] = V[i << 1] + V[i << 1 | 1];
    }

    void update(int l, int r, const U &value) {
        int l0 = l, r0 = r;
        push(l, l+1); if (l != r) push(r, r+1);
        r = min(r+1, N)+N; l = max(l, 0)+N;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l&1) apply(l, value), l++;
            if (r&1) r--, apply(r, value);
        }
        build(l0, l0 + 1); if (l0 != r0) build(r0, r0 + 1);
    }

    T query(int l, int r) {  // sum on interval [l, r]
        static int pos[60];
        int st = 0, en = 60;
        push(l, l+1); if (l != r) push(r, r+1);
        r = min(r+1, N)+N; l = max(l, 0)+N;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) pos[st++] = l++;
            if (r & 1) pos[--en] = --r;
        }
        T res = st == 0 ? V[ pos[en++] ] : V[ pos[0] ];
        for (int i = 1; i < st; ++i) res = res + V[ pos[i] ];
        for (int i = en; i < 60; ++i) res = res + V[ pos[i] ];
        return res;
    }
    int findOkPrefix(int l, const function<bool(T)> &isOk, bool deb = false) {
        if (l >= N) return 0;
        T test = T();
        push(l, l+1);
        int sz = 0;
        auto goDown = [&](int i, int csz) {
            while (csz > 0) {
                pushToChildren(i);
                T ntest = sz ? test + V[i] : V[i];
                if (isOk(ntest)) test = ntest, i = (i|1)<<1, sz += csz;
                else i = (i<<1);
                csz >>= 1;
            }
            return sz;
        };
        int r = N - 1, csz = 1;
        for (l += N, r += N; l < r; l >>= 1, r >>= 1, csz <<= 1) {
            if (l & 1) {
                T ntest = sz ? test + V[l] : V[l];
                if (!isOk(ntest)) {
                    pushToChildren(l);
                    return goDown(l << 1, csz >> 1);
                }
                sz += csz; test = ntest; l++;
            }
            if (r & 1) --r;
        }
        pushToChildren(r);
        if (isOk(sz == 0 ? V[r] : test + V[r])) return sz + csz;
        return goDown(r << 1, csz >> 1);
    }
};


#define INF 2100000000

#define copy(src,dest, sz) for (int i = 0; i < sz; ++i) dest[i] = src[i]


struct MinGood {
    int totalOr, totalMax, minGood, v;
    int szA, szB;
    PII anchorLeft[30], anchorRight[30];

    MinGood(int a=0, int b=0, int v=0)
            : totalOr(b), totalMax(a), v(v) {
        minGood = b >= v ? a : INF;
        szA = szB = 0;
        anchorLeft[szA++] = PII(b, a);
        anchorRight[szB++] = PII(b, a);
    }

    MinGood operator+(const MinGood &o) const {
        MinGood r(max(totalMax, o.totalMax), totalOr | o.totalOr, v);
        r.minGood = min(min(minGood, o.minGood), mergeAnchors(anchorRight, szB, o.anchorLeft, o.szA));
        copy(anchorLeft, r.anchorLeft, szA); r.szA = szA;
        addNonSuboptimal(r.anchorLeft, r.szA, o.anchorLeft, o.szA, totalOr, totalMax);

        copy(o.anchorRight, r.anchorRight, o.szB); r.szB = o.szB;
        addNonSuboptimal(r.anchorRight, r.szB, anchorRight, szB, o.totalOr, o.totalMax);
        return r;
    }

    static void addNonSuboptimal(PII A[30], int &szA, const PII B[30], const int &szB, int tOr, int tMax) {
        for (int i = 0; i < szB; ++i) {
            if (szA == 0 || A[szA-1].first != (B[i].first | tOr))
                A[szA++] = PII(B[i].first | tOr, max(B[i].second, tMax));
        }
    }

    int mergeAnchors(const PII left[30], int szA, const PII right[30], int szB) const {
        int best = INF;
        int j = szB - 1;
        for (int i = 0; i < szA; ++i) {
            while (j > 0 && (right[j-1].first | left[i].first) >= v) j--;
            if ((right[j].first | left[i].first) >= v) {
                best = min(best, max(right[j].second, left[i].second));
            }
        }
        return best;
    }
};

struct Replace {
    int v;
    Replace(int v=0) : v(v) {}
    Replace operator+=(const Replace &o) {
        v = o.v;
        return *this;
    }
    Replace operator*(const int &n) const {
        return *this;
    }
    friend void operator+=(MinGood &t, const Replace &u) {
        t = MinGood(t.totalMax, u.v, t.v);
    }
};

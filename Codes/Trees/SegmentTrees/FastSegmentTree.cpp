template<class T, class U>
struct SegmentTree {
    T *V;
    U *D;
    bool *L;
    int N, H;
    SegmentTree(int N) : V(new T[2*N]), D(new U[N]), L(new bool[N]), N(N), H(sizeof(int) * 8 - __builtin_clz(N)) {
        memset(L, 0, sizeof(bool) * N);
        for (int i = 0; i < N; ++i) D[i] = U();
    }
    ~SegmentTree() { delete[] V; delete[] L; delete[] D; }
    inline void apply(int p, const U &value) {
        V[p] = V[p] + value;
        if (p < N) D[p] += value, L[p] = true;
    }
    inline void build(int p) {
        while (p > 1) p >>= 1, V[p] = V[p<<1] + V[p<<1|1];
    }
    inline void pushToChildren(int i) {
        if (i < N && L[i]) apply(i<<1, D[i]), apply(i<<1|1, D[i]), D[i] = U(), L[i] = false;
    }
    inline void push(int p) {
        for (int s = H; s > 0; --s) pushToChildren(p >> s);
    }
    template<class I>
    void create(const vector<I> &init) {  // build the tree
        for (int i = 0; i < N; ++i) V[i+N] = init[i];
        for (int i = N - 1; i > 0; --i) V[i] = V[i << 1] + V[i << 1 | 1];
    }

    void update(int l, int r, const U &value) {
        r = min(r+1, N)+N; l = max(l, 0)+N;
        push(l); if (l != r-1) push(r - 1);
        int l0 = l, r0 = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l&1) apply(l++, value);
            if (r&1) apply(--r, value);
        }
        build(l0);
        if (l != r) build(r0 - 1);
    }

    T query(int l, int r) {  // sum on interval [l, r]
        static int pos[60];
        int st = 0, en = 60;
        r = min(r+1, N)+N; l = max(l, 0)+N;
        push(l); if (l != r-1) push(r - 1);
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
        push(l);
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

// SegmentTree<int,int> would do range sum and point update.

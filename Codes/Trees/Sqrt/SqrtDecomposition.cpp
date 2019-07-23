template<class E, class G, class Q, class U>
struct SqrtDecomp {
    int GSZ;
    int N;
    vector<G> group;
    vector<U> ups;
    vector<E> elem;
    template<class I>
    SqrtDecomp(const vector<I> &V) : N(V.size()) {
        GSZ = sqrt(max((int) V.size() / 6, 1));
        for (int i = 0; i < V.size(); ++i) {
            elem.push_back(E(V[i]));
        }
        for (int i = 0, j = 0; i < V.size(); i += GSZ, j++) {
            group.push_back(G(getGroup(j)));
            ups.push_back(U());
        }
    }
    vector<E> getGroup(int i) {
        return vector<E>(
                elem.begin() + getBegin(i),
                elem.begin() + getEnd(i) + 1);
    }
    int getBegin(int i) { return i*GSZ; }
    int getEnd(int i) { return min((i+1)*GSZ, N) - 1; }
    Q query(int l, int r) {
        Q ans = Q();
        for (int i = 0; i < group.size(); ++i) {
            int s = getBegin(i), e = getEnd(i);
            if (e < l || r < s) continue;
            if (l <= s && e <= r) {
                ans = ans + group[i].query();
            } else {
                bool nu = false;
                for (int k = s; k <= e; ++k) {
                    assert(0 <= k && k < elem.size());
                    nu |= ups[i].applyOne(elem[k]);
                    if (l <= k && k <= r) ans = ans + elem[k].query();
                }
                ups[i] = U();
                if(nu) group[i].rebuild(getGroup(i));
            }
        }
        return ans;
    }
    void update(int l, int r, const U &u) {
        for (int i = 0; i < group.size(); ++i) {
            int s = getBegin(i), e = getEnd(i);
            if (e < l || r < s) continue;
            if (l <= s && e <= r) {
                u.applyMany(s, e, group[i]);
                ups[i] += u;
            } else {
                for (int k = s; k <= e; ++k) {
                    ups[i].applyOne(elem[k]);
                    if (l <= k && k <= r) u.applyOne(elem[k]);
                }
                ups[i] = U();
                group[i].rebuild(getGroup(i));
            }
        }
    }
};
struct Line {
    Long m, b;
    Line(Long m = 0, Long b = 0) : m(m), b(b) {}
    Long query() {
        return max(b, -b);
    }
    Long eval(Long x) {
        return m * x + b;
    }
    bool pitty(Line l, Line r) {
        Long d0 = (l.b - r.b)/(r.m - l.m);
        Long d1 = (l.b - b)/(m - l.m);
        if (d0 != d1) return d0 <= d1;
//        return 1.0L*(l.b - r.b)/(r.m - l.m) <= 1.0L*(l.b - b)/(m - l.m);
//        Long d0 = 0;
//        assert(dlog2(abs(l.b - r.b)) + dlog2(abs(m - l.b)) < 63);
//        assert(dlog2(abs(l.b - b)) + dlog2(abs(r.m - l.b)) < 63);
        return 1LL*(l.b - r.b - d0*(m  -l.m))*(m   - l.m) <=
               1LL*(l.b - b   - d0*(r.m-l.m))*(r.m - l.m);
    }
};
struct CHT {
    vector<int> INC, DEC;
    vector<int> INCR, DECR;
    vector<Line> L;
    int x, pi, pd;
    CHT(vector<Line> L) : L(L), x(0), pi(0), pd(0) {
        for (int i = 0; i < L.size(); ++i) {
            INC.push_back(i);
            DEC.push_back(i);
        }
        sort(INC.begin(), INC.end(), [&L](int a, int b) {
            return L[a].m != L[b].m ? L[a].m < L[b].m : (L[a].b < L[b].b);
        });
        sort(DEC.begin(), DEC.end(), [&L](int a, int b) {
            return L[a].m != L[b].m ? L[a].m > L[b].m : (L[a].b > L[b].b);
        });
        process(INCR, INC);
        process(DECR, DEC);
    }
    void process(vector<int> &R, vector<int> T) {
        R.clear();
        for (int v : T) {
            if (R.size() >= 2 && L[R.back()].m == L[v].m && L[v].pitty(L[R[R.size()-2]], L[R.back()]))
                continue;
            while (R.size() >= 1 && L[R.back()].m == L[v].m) R.pop_back();
            while (R.size() >= 2 && L[R.back()].pitty(L[R[R.size()-2]], L[v]))
                R.pop_back();
            R.push_back(v);
        }
    }
    void rebuild(vector<Line> L) {
        this->L = L;
        x = pi = pd = 0;
        process(INCR, INC);
        process(DECR, DEC);
    }
    Long query() {
        while (pi + 1 < INCR.size() && L[INCR[pi]].eval(x) < L[INCR[pi + 1]].eval(x)) pi++;
        while (pd + 1 < DECR.size() && L[DECR[pd]].eval(x) > L[DECR[pd + 1]].eval(x)) pd++;
        return max(L[INCR[pi]].eval(x), -L[DECR[pd]].eval(x));
    }
};
struct Add {
    Long v;
    Add(Long v = 0) : v(v) { }
    void applyMany(int b, int e, CHT &s) const {
        s.x += v;
    }
    bool applyOne(Line &s) const {
        if (v == 0) return false;
        s.b += s.m * v;
        return true;
    }
    void operator+=(const Add &o) {
        v += o.v;
    }
};
struct Max {
    Long v;
    Max(Long v = 0) : v(v) {}
    Max operator+(Max o) {
        return Max(max(v, o.v));
    }
};

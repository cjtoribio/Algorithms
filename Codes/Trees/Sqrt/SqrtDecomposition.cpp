template<class E, class G, class Q, class U>
struct SqrtDecomp {
    int GSZ;
    int N;
    vector<G> group;
    vector<U> ups;
    vector<E> elem;
    template<class I>
    SqrtDecomp(const vector<I> &V) : N(V.size()) {
        GSZ = sqrt(V.size());
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
                for (int k = s; k <= e; ++k) {
                    ups[i].applyOne(elem[k]);
                    if (l <= k && k <= r) ans = ans + elem[k].query();
                }
                ups[i] = U();
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
    int x, m, b;
    Line(int x, int m, int b) : m(m), b(b) {}
    int query() {
        return m * x + b;
    }
    int eval(int x) {
        return m * x + b;
    }
};
struct CHT {
    vector<int> INC, DEC;
    vector<Line> L;
    int x, pi, pd;
    CHT(vector<Line> L) : L(L), x(0), pi(0), pd(0) {
        for (int i = 0; i < L.size(); ++i) {
            INC.push_back(i);
            DEC.push_back(i);
        }

        sort(INC.begin(), INC.end(), [&L](int a, int b) { return L[a].m < L[b].m; });
        sort(DEC.begin(), DEC.end(), [&L](int a, int b) { return L[a].m > L[b].m; });
    }
    void rebuild(vector<Line> L) {
        this->L = L;
        this->x = 0;
    }
    int query() {
        while (pi + 1 < INC.size() && L[INC[pi]].eval(x) < L[INC[pi + 1]].eval(x)) pi++;
        while (pd + 1 < DEC.size() && L[DEC[pd]].eval(x) > L[DEC[pd + 1]].eval(x)) pd++;
        return max(L[INC[pi]].eval(x), -L[DEC[pd]].eval(x));
    }
};
struct Add {
    int v;
    Add(int v = 0) : v(v) { }
    void applyMany(int b, int e, CHT &s) const {
        s.x += v;
    }
    void applyOne(Line &s) const {
        s.x += v;
    }
    void operator+=(const Add &o) {
        v += o.v;
    }
};
struct Max {
    int v;
    Max(int v = 0) : v(v) {}
    Max operator+(Max o) {
        return Max(max(v, o.v));
    }
};

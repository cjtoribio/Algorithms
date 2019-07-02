#define INF 1e200
template<class Curve, class cmp>
struct ConvexHull {
    struct LineX {
        Curve l;
        long double x;
        bool operator<(const LineX &o) const {
            return (o.x == -INF || x == -INF) ? cmp()(l, o.l) : (x + 1e-9 < o.x);
        }
    };
    set<LineX> S;
    template<class T> static T prev(T a) { a--; return a; }
    template<class T> static T next(T a) { a++; return a; }
    void addLine(Curve l) {
        auto it = S.lower_bound({l, -INF});
        if (it != S.end() && it->l.m == l.m) return;
        if (it != S.end() && it != S.begin() && l.pitty(prev(it)->l, it->l)) return;
        vector<Curve> tr;
        if (it != S.begin())
            for (auto r = prev(it); r != S.begin() && r->l.pitty(prev(r)->l, l); --r) {
                tr.push_back(r->l);
            }
        if (it != S.begin() && prev(it) == S.begin() && prev(it)->l.m == l.m)
            tr.push_back(prev(it)->l);
        for (auto f = it; f != S.end() && next(f) != S.end() && f->l.pitty(l, next(f)->l); f++) {
            tr.push_back(f->l);
        }
        for (auto t : tr) S.erase({t, -INF});
        vector<Curve> ta;
        auto pr = S.lower_bound({l, -INF});
        if (pr != S.begin()) { pr--; ta.push_back(pr->l), S.erase(pr); }
        safeAdd(l);
        if (ta.size()) safeAdd(ta[0]);
    }
    void safeAdd(Curve l) {
        auto it = S.lower_bound({l, -INF});
        S.insert({l, it == S.end() ? INF : l.xinter(it->l)});
    }
    long double getBest(long double x) {
        auto it = S.lower_bound({{0, 0}, x});
        return x * it->l.m + it->l.b;
    }
};
struct Curve {
    double m, b;
    long double pitty(Curve le, Curve ri) const {
        return ((b - le.b)*(le.m - ri.m) >= (ri.b - le.b)*(le.m - m));
    }
    long double xinter(Curve o) const {
        return 1.0L * (o.b - b) / (m - o.m);
    }
};
struct IncSlope {
    bool operator()(const Curve &a, const Curve &b) {
        return a.m != b.m ? a.m < b.m : a.b < b.b;
    }
};
struct DecSlope {
    bool operator()(const Curve &a, const Curve &b) {
        return a.m != b.m ? a.m > b.m : a.b > b.b;
    }
};

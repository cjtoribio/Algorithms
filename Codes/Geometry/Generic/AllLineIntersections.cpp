template <class F>
struct Line {
    Point<F> a, ab;
    Line() : a(), ab() {}
    Line(Point<F> a, Point<F> b, bool twoPoints = true)
            : a(a), ab(twoPoints ? b - a : b) {}

    Point<F> b() const { return a + ab; }
    operator bool () const { return ab != Point<F>(); }
    // careful with doubles
    bool aligned(Point<F> o)const{ return ((o - a)^(ab)) == 0; }
    bool contains(Point<F> o)const {
        return min(a.x, b().x) <= o.x && o.x <= max(a.x, b().x) && min(a.y, b().y) <= o.y && o.y <= max(a.y, b().y);
    }
    Point<F> eval(F x) {
        if (abs(ab.x) < 1e-9) return a;
        return a + ab * (x - a.x) / ab.x;
    }
};

enum EC { R, C, O }; // Ray, Closed, Open
template<class F>
bool check(EC t, F a, F b){
    if(t == EC::R) return true;
    if(t == EC::C) return a <= b + 1e-9;
    if(t == EC::O) return a < b - 1e-9;
}
// res: is where the intersection is placed in case intersections exists
// LA and LB are the endpoint types of A and B of the line 'lhs'
// RA and RB are the endpoint types of A and B of the line 'lhs'
template <EC LA, EC LB, EC RA, EC RB, class F1, class F2, class F3>
bool intersectLines(Line<F1> lhs, Line<F2> rhs, Point<F3>& res) {
    auto s = lhs.ab ^ rhs.ab;
    if (s == 0) {
        // just return false here if parallel lines dont intersect by definition
        vector<Point<F1>> C = {rhs.a, rhs.b(), lhs.a, lhs.b()};
        sort(C.begin(), C.end());
        for (auto p : C) if (lhs.aligned(p) && lhs.contains(p) && rhs.aligned(p) && rhs.contains(p))
            return res = p, true;
        return false;
    }
    auto ls = (rhs.a - lhs.a) ^ rhs.ab;
    auto rs = (rhs.a - lhs.a) ^ lhs.ab;
    if (s < 0) s = -s, ls = -ls, rs = -rs;
    bool intersect = check(LA, decltype(ls)(0), ls) && check(LB, ls, s) &&
                     check(RA, decltype(rs)(0), rs) && check(RB, rs, s);
    if (intersect) res = lhs.a + lhs.ab * static_cast<F3>(ls) / s;
    return intersect;
}

template <class F1, class F2>
bool parallelDist(Line<F1> lhs, Line<F2> rhs, int &type) {
    auto s = lhs.ab ^ rhs.ab;
    if (s != 0) return false;
    if ((rhs.ab ^ (lhs.a - rhs.a)) != 0) return false;
    if (rhs.ab * lhs.ab > 0) return type = ((rhs.a - lhs.a)*lhs.ab > 0 ? 1 : 2), true;
    return type = 3, (rhs.a - lhs.a)*lhs.ab > 0;
}



vector<PII> intersections(vector<Line<Double>> lines) {
    static double cx, cy;
    static int LOWER, UPPER;
    static vector<Line<Double>> glines;
    for (auto &l : lines) {
        l = Line(min(l.a, l.b()), max(l.a, l.b()));
    }
    glines = lines;
    int N = glines.size();
    LOWER = N + 0, UPPER = N + 1;
    glines.push_back(Line(Point<Double>(0, -1e20), Point<Double>(1, 0), false)); // LOWER
    glines.push_back(Line(Point<Double>(0, +1e20), Point<Double>(1, 0), false)); // UPPER
    cx = -1e20;
    cy = -1e20;
    struct Event {
        Point<Double> p; int lineId; int action; // (0 - add, 1 - touch, 2 - out)
        bool operator<(const Event e) const {
            if (abs(p.x - e.p.x) > 1e-9) return p.x < e.p.x;
            if (action != e.action) return action < e.action;
            if (abs(p.y - e.p.y) > 1e-9) return p.y < e.p.y;
            return lineId < e.lineId;
        }
    };
    struct cmp {
        bool operator()(int a, int b) const {
            Double ay = glines[a].eval(cx).y;
            Double by = glines[b].eval(cx).y;
            if (abs(ay - by) > 1e-9) return ay < by;
            if (a == LOWER || b == LOWER) return a == LOWER;
            if (a == UPPER || b == UPPER) return b == UPPER;
            if (abs(glines[a].ab^glines[b].ab) < 1e-9) return a < b;
            if (cy < ay - 1e-9) return (glines[a].ab^glines[b].ab) < 1e-9;
            return (glines[a].ab^glines[b].ab) > 1e-9;
        }
    };
    set<int, cmp> activeLines;
    set<Event> PQ;
    vector<set<int, cmp>::iterator> pointers(glines.size(), activeLines.end());
    for (int i = 0; i < N; ++i) {
        if (glines[i].a.x == glines[i].b().x) {
            PQ.insert((Event) {min(glines[i].a, glines[i].b()), i, 2});
        } else {
            PQ.insert((Event) {min(glines[i].a, glines[i].b()), i, 0});
            PQ.insert((Event) {max(glines[i].a, glines[i].b()), i, 3});
        }
    }
    auto addLine = [&](int lineId, Event e) {
        auto it = pointers[lineId] = activeLines.insert(lineId).first;
        if (activeLines.begin() != it) {
            Point<Double> inter;
            if (intersectLines<EC::C,EC::C,EC::C,EC::C>(glines[*prev(it)], glines[lineId], inter)) {
                if (e < (Event){inter, -1, 1}) {
                    PQ.insert({inter, -1, 1});
                }
            }
        }
        if (next(it) != activeLines.end()) {
            Point<Double> inter;
            if (intersectLines<EC::C,EC::C,EC::C,EC::C>(glines[lineId], glines[*next(it)], inter)) {
                if (e < (Event){inter, -1, 1}) {
                    PQ.insert({inter, -1, 1});
                }
            }
        }
    };
    auto remLine = [&](int lineId, Event e) {
        auto ne = activeLines.erase(pointers[lineId]);
        pointers[lineId] = activeLines.end();
        if (ne != activeLines.end() && ne != activeLines.begin()) {
            Point<Double> inter;
            if (intersectLines<EC::C,EC::C,EC::C,EC::C>(glines[*ne], glines[*prev(ne)], inter)) {
                if (e < (Event){inter, -1, 1}) {
                    PQ.insert({inter, -1, 1});
                }
            }
        }
    };
    vector<pair<int,int>> I;
    set<int> PJ;
    while (PQ.size()) {
        auto e = *PQ.begin();
        cx = e.p.x;
        glines[N+0].a.x = glines[N+1].a.x = cx;
        cy = -1e20;
        while (PQ.size() && PQ.begin()->p.x == e.p.x && PQ.begin()->action == 0) {
            auto add = *PQ.begin(); PQ.erase(PQ.begin());
            addLine(add.lineId, add);
        }
        while (PQ.size() && PQ.begin()->p.x == e.p.x && PQ.begin()->action == 1) {
            auto inter = *PQ.begin(); PQ.erase(PQ.begin());
            glines[N+0].a.y = glines[N+1].a.y = inter.p.y;
            auto l = activeLines.lower_bound(LOWER);
            auto r = activeLines.upper_bound(UPPER);
            cy = inter.p.y;
            vector<int> TI;
            for (auto t = l; t != r; ++t) TI.push_back(*t);
            Point<Double> temp;
            for (int i = 0; i < TI.size(); ++i)
                for (int j = i+1; j < TI.size(); ++j)
                    if (intersectLines<EC::C, EC::C, EC::C, EC::C>(glines[TI[i]], glines[TI[j]], temp)) {
                        I.emplace_back(min(TI[i], TI[j]), max(TI[i], TI[j]));
                    }
            for (auto v : TI) remLine(v, inter);
            for (auto v : TI) addLine(v, inter);
        }
        set<pair<Double, int>> endings;
        while (PQ.size() && PQ.begin()->p.x == e.p.x && PQ.begin()->action == 2) {
            auto rem = *PQ.begin(); PQ.erase(PQ.begin());
            // this for matches all overlapping vertical lines
            for (auto it = endings.lower_bound(make_pair(rem.p.y, -1)); it != endings.end(); ++it)
                I.emplace_back(min(it->second, rem.lineId), max(it->second, rem.lineId));
            endings.insert(make_pair(glines[rem.lineId].b().y, rem.lineId));
            glines[LOWER].a.y = glines[rem.lineId].a.y;
            glines[UPPER].a.y = glines[rem.lineId].b().y;
            auto l = activeLines.lower_bound(LOWER);
            auto r = activeLines.upper_bound(UPPER);
            for (auto t = l; t != r; ++t) {
                I.emplace_back(min(*t, rem.lineId), max(*t, rem.lineId));
            }
        }
        while (PQ.size() && PQ.begin()->p.x == e.p.x && PQ.begin()->action == 3) {
            auto rem = *PQ.begin(); PQ.erase(PQ.begin());
            remLine(rem.lineId, rem);
        }
    }
    sort(I.begin(), I.end());
    I.erase(unique(I.begin(), I.end()), I.end());
    return I;
}

vector<PII> bruteForce(vector<Line<Double>> L) {
    vector<PII> I;
    for (int i = 0; i < L.size(); ++i) {
        for (int j = i+1; j < L.size(); ++j) {
            Point<Double> tem;
            if (intersectLines<EC::C, EC::C, EC::C, EC::C>(L[i], L[j], tem)) {
                I.push_back(PII(i, j));
            }
        }
    }
    return I;
}

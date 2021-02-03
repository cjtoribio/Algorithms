#define EPS 1e-9
enum EC { R, C, O }; // Ray, Closed, Open
template<class F>
bool check(EC t, F a, F b){
    if(t == EC::R) return true;
    if(t == EC::C) return a <= b + EPS;
    if(t == EC::O) return a < b - EPS;
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
        for (auto p : C)
            if (lhs.aligned(p) && lhs.contains(p) && rhs.aligned(p) && rhs.contains(p))
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

template<typename F>
vector<PII> intersections(vector<Line<F>> lines) {
    static double cx, cy;
    static int LOWER, UPPER;
    static vector<Line<F>> glines;
    for (auto &l : lines) l = Line(min(l.a, l.b()), max(l.a, l.b())); // normalize segments
    glines = lines;
    int N = glines.size();
    LOWER = N + 0, UPPER = N + 1;
    glines.push_back(Line(Point<F>(0, 0), Point<F>(1, 0), false)); // LOWER
    glines.push_back(Line(Point<F>(0, 0), Point<F>(1, 0), false)); // UPPER
    cx = -1e20;
    cy = -1e20;
    struct Event {
        Point<F> p; int lineId; int action; // (0 - add, 1 - touch, 2 - out)
        Event(Point<F> p, int lineId, int action) : p(p), lineId(lineId), action(action) {}
        bool operator<(const Event e) const {
            if (abs(p.x - e.p.x) > EPS) return p.x < e.p.x;
            if (action != e.action) return action < e.action;
            if (abs(p.y - e.p.y) > EPS) return p.y < e.p.y;
            return lineId < e.lineId;
        }
    };
    struct cmp {
        bool operator()(int a, int b) const {
            auto ay = eval(glines[a]).y;
            auto by = eval(glines[b]).y;
            if (abs(ay - by) > EPS) return ay < by;
            if (a == LOWER || b == LOWER) return a == LOWER;
            if (a == UPPER || b == UPPER) return b == UPPER;
            if (abs(glines[a].ab^glines[b].ab) < EPS) return a < b;
            if (cy < ay - EPS) return (glines[a].ab^glines[b].ab) < EPS;
            return (glines[a].ab^glines[b].ab) > EPS;
        }
        Point<F> eval(const Line<F> &l) const { return abs(l.ab.x) < 1e-9 ? l.a : l.a + l.ab * (cx - l.a.x) / l.ab.x; }
    };
    set<int, cmp> activeLines;
    set<Event> PQ;
    vector<typename set<int, cmp>::iterator> pointers(glines.size(), activeLines.end());
    for (int i = 0; i < N; ++i) {
        if (glines[i].a.x == glines[i].b().x) {
            PQ.emplace(min(glines[i].a, glines[i].b()), i, 2);
        } else {
            PQ.emplace(min(glines[i].a, glines[i].b()), i, 0);
            PQ.emplace(max(glines[i].a, glines[i].b()), i, 3);
        }
    }
    auto addLine = [&](int lineId, Event e) {
        auto it = pointers[lineId] = activeLines.insert(lineId).first;
        if (activeLines.begin() != it) {
            Point<F> inter;
            if (intersectLines<EC::C,EC::C,EC::C,EC::C>(glines[*prev(it)], glines[lineId], inter)) {
                if (e < Event(inter, -1, 1)) {
                    PQ.emplace(inter, -1, 1);
                }
            }
        }
        if (next(it) != activeLines.end()) {
            Point<F> inter;
            if (intersectLines<EC::C,EC::C,EC::C,EC::C>(glines[lineId], glines[*next(it)], inter)) {
                if (e < Event(inter, -1, 1)) {
                    PQ.emplace(inter, -1, 1);
                }
            }
        }
    };
    auto remLine = [&](int lineId, Event e) {
        auto ne = activeLines.erase(pointers[lineId]);
        pointers[lineId] = activeLines.end();
        if (ne != activeLines.end() && ne != activeLines.begin()) {
            Point<F> inter;
            if (intersectLines<EC::C,EC::C,EC::C,EC::C>(glines[*ne], glines[*prev(ne)], inter)) {
                if (e < Event(inter, -1, 1)) {
                    PQ.emplace(inter, -1, 1);
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
            vector<int> TI(activeLines.lower_bound(LOWER), activeLines.upper_bound(UPPER));
            cy = inter.p.y;
            for (auto v : TI) remLine(v, inter);
            for (auto v : TI) addLine(v, inter);
            Point<F> out;
            for (int i = 0; i < TI.size(); ++i)
                for (int j = i+1; j < TI.size(); ++j)
                    if (intersectLines<EC::C, EC::C, EC::C, EC::C>(glines[TI[i]], glines[TI[j]], out)) {
                        if (out.sqdist(inter.p) < EPS)
                            I.emplace_back(min(TI[i], TI[j]), max(TI[i], TI[j]));
                    }
        }
        set<pair<F, int>> endings;
        while (PQ.size() && PQ.begin()->p.x == e.p.x && PQ.begin()->action == 2) {
            auto rem = *PQ.begin(); PQ.erase(PQ.begin());
            // this for matches all overlapping vertical lines
            for (auto it = endings.lower_bound(make_pair(rem.p.y, -1)); it != endings.end(); ++it)
                I.emplace_back(min(it->second, rem.lineId), max(it->second, rem.lineId));
            endings.insert(make_pair(glines[rem.lineId].b().y, rem.lineId));
            glines[LOWER].a.y = glines[rem.lineId].a.y;
            glines[UPPER].a.y = glines[rem.lineId].b().y;
            for (auto t : VI(activeLines.lower_bound(LOWER), activeLines.lower_bound(UPPER)))
                I.emplace_back(min(t, rem.lineId), max(t, rem.lineId));
        }
        while (PQ.size() && PQ.begin()->p.x == e.p.x && PQ.begin()->action == 3) {
            auto rem = *PQ.begin(); PQ.erase(PQ.begin());
            remLine(rem.lineId, rem);
        }
    }
    sort(I.begin(), I.end());
    return I;
}

template<class F>
vector<PII> bruteForce(vector<Line<F>> L) {
    vector<PII> I;
    for (int i = 0; i < L.size(); ++i) {
        for (int j = i+1; j < L.size(); ++j) {
            Point<F> tem;
            if (intersectLines<EC::C, EC::C, EC::C, EC::C>(L[i], L[j], tem)) {
                I.push_back(PII(i, j));
            }
        }
    }
    return I;
}

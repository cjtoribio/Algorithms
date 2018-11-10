struct Line {
	double m, b;
};
#define INF 1e200
template<class cmp>
struct ConvexHull2 {
	struct LineX {
		Line l;
		double x;
		bool operator<(const LineX &o) const {
			return (o.x == -INF || x == -INF) ? cmp()(l, o.l) : (x + 1e-9 < o.x);
		}
	};
	set<LineX> S;
	template<class T> static T prev(T a) { a--; return a; }
	template<class T> static T next(T a) { a++; return a; }
	void addLine(Line l) {
		auto it = S.lower_bound({l, -INF});
		if (it != S.end() && it->l.m == l.m) return;
		if (it != S.end() && it != S.begin() && pitty(prev(it)->l, l, it->l)) return;
		vector<Line> tr;
		if (it != S.begin())
		for (auto r = prev(it); r != S.begin() && pitty(prev(r)->l, r->l, l); --r) {
			tr.push_back(r->l);
		}
		if (it != S.begin() && prev(it) == S.begin() && prev(it)->l.m == l.m) tr.push_back(prev(it)->l);
		for (auto f = it; f != S.end() && next(f) != S.end() && pitty(l, f->l, next(f)->l); f++) {
			tr.push_back(f->l);
		}
		for (auto t : tr) S.erase({t, -INF});
		vector<Line> ta;
		auto pr = S.lower_bound({l, -INF});
		if (pr != S.begin()) {
			pr--;
			ta.push_back(pr->l), S.erase(pr);
		}
		safeAdd(l);
		if (ta.size()) safeAdd(ta[0]);
	}
	void safeAdd(Line l) {
		auto it = S.lower_bound({l, -INF});
		if (it != S.end()) S.insert({l, xinter(l, it->l)});
		else S.insert({l, INF});
	}
	double getBest(double x) {
		auto it = S.lower_bound({{0, 0}, x});
		return x * it->l.m + it->l.b;
	}
	static bool pitty(Line a, Line b, Line c){
		return ((b.b - a.b)*(a.m - c.m) >= (c.b - a.b)*(a.m - b.m));
	}
	static double xinter(Line a, Line b) {
		return 1.0L * (b.b - a.b) / (a.m - b.m);
	}
};
struct IncSlope {
	bool operator()(const Line &a, const Line &b) {
		return a.m != b.m ? a.m < b.m : a.b < b.b;
	}
	double best(double a, double b) const {
		return max(a, b);
	}
};
struct DecSlope {
	bool operator()(const Line &a, const Line &b) {
		return a.m != b.m ? a.m > b.m : a.b > b.b;
	}
	double best(double a, double b) const {
		return min(a, b);
	}
};

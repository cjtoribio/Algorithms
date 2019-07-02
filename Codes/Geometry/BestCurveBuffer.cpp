typedef long double Double;
struct Curve {
	Double a, b, c; // a*(x - b) + c
	long double eval(Double x) {
		return a*(x - b)*(x - b) + c;
	}
	vector<Double> allInter(Curve &o) {
		/* a0 * (x - b0)^2 + c0 = a1 * (x - b1)^2 + c1
		   (a0-a1)*x^2 - 2*(b0*a0-b1*a1)*x + (b0*b0+c0-c1-b1*b1) = 0 */
		Double na = (a - o.a);
		Double nb = -2*(a*b - o.a*o.b);
		Double nc = (a*b*b + c - o.c - o.a*o.b*o.b);
		if (na == 0) return nb == 0 ? vector<Double>() : vector<Double>(1, -nc / nb);
		if (nb*nb - 4*na*nc < 0) return {};
		Double det = sqrt(nb*nb - 4*na*nc);
		vector<Double> R = { (-nb - det) / (2*na), (-nb + det) / (2*na) };
		if (R[0] > R[1]) swap(R[0], R[1]);
		return R;
	}
};

struct Buffer {
#define INF 1e200
#define EPS 1e-9
	struct CurveDominance {
		int id;
		Double st, en;
	};
	vector<Curve> C;
	vector<CurveDominance> D;
	void add(Curve c) {
		int nid = C.size();
		C.push_back(c);
		vector<CurveDominance> ND;
		for (auto p : D) {
			auto inters = c.allInter(C[p.id]);
			inters.push_back(INF);
			Double px = -INF;
			for (Double x : inters) {
				if (c.eval(x-1) >= C[p.id].eval(x-1)) {
					Double st = max(p.st, px);
					Double en = min(p.en, x);
					if (st <= en) {
						ND.push_back({p.id, st, en});
					}
				}
				px = x;
			}
		}
		D.clear();
		Double px = -INF;
		for (int i = 0; i < ND.size(); ++i) {
			if (ND[i].st > px) {
				D.push_back({nid, px, ND[i].st});
			}
			D.push_back(ND[i]);
			px = ND[i].en;
		}
		if (ND.size() == 0 || ND.back().en < INF) {
			D.push_back({nid, px, INF});
		}
	}
	Double query(Double x) {
		int lo = 0, hi = (int)D.size() - 1, ret = -1;
		while (lo <= hi) {
			int mi = (lo + hi) / 2;
			if (D[mi].en < x) {
				lo = mi + 1;
			} else {
				hi = mi - 1;
				ret = mi;
			}
		}
		return ret == -1 ? INF : C[D[ret].id].eval(x);
	}
#undef EPS
#undef INF
};

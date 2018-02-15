typedef long double Double;
typedef long long Long;
struct ConvexHullTrick {
	struct Line {
		Double m,b;
		Line(Double m,Double b):m(m),b(b){}
		Double inter(const Line &L)const{
			return (L.b-b) / (m - L.m);
		}
		Double eval(Double x){
			return m*x + b;
		}
	};
	struct Node {
		Line l = Line(1, 0);
		Double intWithPrev = -1e100;
		vector<int> par;
		vector<int> to;
	};
	vector<Node> nodes;
	ConvexHullTrick(){ };
	void add(Line l, int version = -1){
		int id = nodes.size();
		Node nn; nn.l = l;
		if(nodes.size() != 0) {
			int p = version == -1 ? nodes.size() -1 : version;
			for (int i = nodes[p].par.size(); i >= 0 && p != 0; --i) {
				if (nodes[p].par.size() <= i) continue;
				if (nodes[p].par[i] == 0) continue;
				int pp = nodes[p].par[i];
				int ppp = nodes[pp].par[0];
				if(nodes[ppp].l.inter(l) < nodes[pp].intWithPrev) {
					p = ppp;
				} else {
					break;
				}
			}
			while(p != 0) {
				int pp = nodes[p].par[0];
				if(nodes[pp].l.inter(l) < nodes[p].intWithPrev) {
					p = pp;
				} else {
					break;
				}
			}
			nodes[p].to.push_back(id);
			nn.par.push_back(p);
			nn.intWithPrev = nn.l.inter(nodes[p].l);
			for(int i = 0; i < nodes[ nn.par[i] ].par.size(); ++i) {
				int rp = nn.par[i];
				nn.par.push_back(nodes[rp].par[i]);
			}
			assert(nn.par.size() <= 20);
		}
		nodes.push_back(nn);
	}
	Double eval(Double x, int version = -1){
		int n = version == -1 ? nodes.size() - 1 : version;
//		while(nodes[n].intWithPrev > x)
//			n = nodes[n].par[0];
		for (int i = nodes[n].par.size(); i >= 0; --i) {
			if (nodes[n].par.size() <= i) continue;
			int p = nodes[n].par[i];
			if (x < nodes[p].intWithPrev) {
				n = p;
			}
		}
		if (x < nodes[n].intWithPrev) n = nodes[n].par[0];
		return nodes[n].l.eval(x);
	}
	void print(int n = 0, int lvl = 0) {
		cout << string(lvl * 3, ' ')<< n << " " << nodes[n].intWithPrev << endl;
		for(int v : nodes[n].to)
			print(v, lvl+1);
	}
};

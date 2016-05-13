// algorithms
// vector
struct ConvexHullTrick {
	struct Line {
		double m,b;
		Line(double m,double b):m(m),b(b){}
		double inter(const Line &L)const{
			return (L.b-b) / (m - L.m);
		}
		double eval(double x){
			return m*x + b;
		}
	};
	vector<Line> V;
	vector<double> X;
	int C;
	ConvexHullTrick():C(0){ };
	void add(Line l){
		while(V.size() >= 2){
			if(V[V.size()-2].inter(l) < X.back()){
				V.pop_back();
				X.pop_back();
			} else break;
		}		
		if(V.size() >= 1){
			X.push_back(V.back().inter(l));
		}
		V.push_back(l);
		C = min(C , (int)X.size());
	}
	double eval(double x){
		int p = lower_bound(X.begin(),X.end(), x) - X.begin();
		return V[p].eval(x);
	}
	// only if x are used in increasing order
	double evalConstant(double x){
		while(C < X.size() && x > X[C])C++;
		return V[C].eval(x);
	}
};
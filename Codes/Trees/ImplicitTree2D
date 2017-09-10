template<typename T, typename U, typename Q, typename O>
struct ImplicitTree{
	struct Query {
		int i, j;
		Q q;
	};
	struct Update {
		int i, j;
		U u;
		Update operator*(const int v)const{
			Update nu = *this;
			nu.u = nu.u * v;
			return nu;
		}
	};
	ImplicitTree *l, *r;
	int b, e;
	T val;
	ImplicitTree(int b = 0, int e = 200000):b(b),e(e){
		l = r = NULL;
		val = T();
	}
	void update(const Update &u){
		if(u.i <= b && e <= u.j) val.update(u.u * (e-b+1));
		else if(u.j < b || e < u.i)return;
		else{
			int m = (b+e)/2;
			(l ? l : (l = new ImplicitTree(b, m  )))->update(u);
			(r ? r : (r = new ImplicitTree(m+1, e)))->update(u);
			val.update(u.u * (min(e, u.j) - max(b, u.i) + 1));
		}
	}
	O query(const Query &q){
		if(q.i <= b && e <= q.j) return val.query(q.q);
		else if(q.j < b || e < q.i) O();
		else{
			int m = (b+e)/2;
			if(q.j <= m)return l ? l->query(q) : O();
			if(m <  q.i)return r ? r->query(q) : O();
			return (l?l->query(q):O()) + (r?r->query(q):O());
		}
	}
	void print(int lvl = 0){
		if(l)l->print(lvl+1);
		cout << string(lvl*3,' ')<< "(" << b << "," << e << ")";
		val.print(lvl);
		if(r)r->print(lvl+1);
	}
};

struct Val {
	int val;
	void update(int v){
		val += v;
	}
	int query(int v){
		return val;
	}
	void print(int lvl){
		cout << val << endl;
	}
};


int main() {
	
	typedef ImplicitTree<Val, int, int, int> ITV;
	ImplicitTree<ITV, ITV::Update, ITV::Query, int> t;
}
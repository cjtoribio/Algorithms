template<class T, class U>
struct SegmentTree {
	struct Node {
		int sz = 1;
		bool hasCarry = 0;
		U carry = U(); T val = T();
		Node(){ }
		void join(const Node &l, const Node &r){
			sz = l.sz + r.sz;
			val = l.val + r.val;
			carry = U();
		}
		void update(U u){
			carry += u;
			val += u * sz;
			hasCarry = 1;
		}
		void pushDown(Node &l, Node &r){
			if(!hasCarry)return;
			l.update(carry);
			r.update(carry);
            // r.update(carry << l.sz); // shift the update, for sequences query
			carry = U();
			hasCarry = 0;
		}
	};
	vector<Node> V;
	int N;
	SegmentTree(int N) : V(4*N), N(N){ }
	template<class I>
	void create(const vector<I> &VEC, int n = 1, int b = 0, int e = -1) {
		if(e == -1) e = N-1;
		if (b == e) {
			V[n].val = T(VEC[b]);
		} else {
			create(VEC, 2 * n, b, (e + b) / 2);
			create(VEC, 2 * n + 1, (e + b) / 2 + 1, e);
			V[n].join(V[2*n], V[2*n+1]);
		}
	}
	T query(int i, int j, int n = 1, int b = 0, int e = -1) {
		if(e == -1) e = N-1;
		if (i <= b && e <= j)
			return V[n].val;
		else {
			V[n].pushDown(V[2*n], V[2*n+1]);
			int mid = (b + e) / 2;
			if (i >  mid) return query(i, j, 2 * n + 1, mid + 1, e);
			if (j <= mid) return query(i, j, 2 * n, b, mid);
			return query(i, j, 2 * n, b, mid)
			     + query(i, j, 2 * n + 1, mid + 1, e);
		}
	}
	void update(int i, int j, U v, int n = 1, int b = 0, int e = -1) {
		if(e == -1) e = N-1;
		if (i <= b && e <= j) {
			V[n].update(v);
		} else if (i > e || j < b)
			return;
		else {
			V[n].pushDown(V[2*n], V[2*n+1]);
			int mid = (b + e) / 2;
			update(i, j, v, 2 * n, b, mid);
			update(i, j, v, 2 * n + 1, mid + 1, e);
			V[n].join(V[2*n], V[2*n+1]);
		}
	}
};
struct Add {
	int v;
	Add(int v=0):v(v){} // empty update
	Add& operator*(const int sz){ // apply update to range
		return *this;
	}
	void operator+=(const Add &a){ // acumulate updates
		v += a.v;
	}
};
struct Max {
	int v = 0;
	Max(int v=0):v(v){} // default
	Max operator+(const Max &a)const{ // join
		return (Max){ max(v, a.v) }; 
	}
	void operator+=(const Add &a){ // apply update
		v += a.v;
	}
};
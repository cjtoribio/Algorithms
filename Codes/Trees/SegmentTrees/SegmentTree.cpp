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
		}
		void update(const U &u){
			carry += u;
			val += u * sz;
			hasCarry = 1;
		}
		void pushDown(Node &l, Node &r){
			if(!hasCarry)return;
			l.update(carry);
			r.update(carry);
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
			int m = (b + e) / 2;
			create(VEC, 2 * n, b, m);
			create(VEC, 2 * n + 1, m + 1, e);
			V[n].join(V[2*n], V[2*n+1]);
		}
	}
	T query(int i, int j, int n = 1, int b = 0, int e = -1) {
		if(e == -1) e = N-1;
		if (i <= b && e <= j){
			return V[n].val;
		} else {
			V[n].pushDown(V[n<<1], V[n<<1|1]);
			int m = (b + e) / 2;
			if (i >  m) return query(i, j, 2*n+1, m+1, e);
			if (j <= m) return query(i, j, 2*n, b, m);
			return query(i, j, 2*n, b, m) + query(i, j, 2*n+1, m+1, e);
		}
	}
	void update(int i, int j, const U &v, int n = 1, int b = 0, int e = -1) {
		if(e == -1) e = N-1;
		if (i <= b && e <= j) {
			V[n].update(v);
		} else if (i > e || j < b)
			return;
		else {
			V[n].pushDown(V[n<<1], V[n<<1|1]);
			int m = (b + e) / 2;
			update(i, j, v, 2 * n, b, m);
			update(i, j, v, 2 * n + 1, m + 1, e);
			V[n].join(V[2*n], V[2*n+1]);
		}
	}
};
typedef long long Long;
struct Replace {
	char c[10];
	Replace(int from = 0, int to = 0){
		for (int i = 0; i < 10; ++i) {
			c[i] = i;
		}
		c[from] = to;
	}
	Replace operator*(const int sz)const{
		return *this;
	}
	void operator+=(const Replace &a){
		static char nc[10];
		for (int i = 0; i < 10; ++i) {
			nc[i] = a.c[c[i]];
		}
		copy(nc, nc+10, c);
	}
};
struct DigitSum {
	Long d[10];
	DigitSum(int v=0){
		fill(d,d+10,0);
		int p = 1;
		while(v > 0){
			d[v%10] += p;
			v /= 10;
			p *= 10;
		}
	}
	DigitSum operator+(const DigitSum &a)const{ 
		DigitSum r;
		for (int i = 0; i < 10; ++i) {
			r.d[i] = d[i] + a.d[i];
		}
		return r;
	}
	void operator+=(const Replace &a){
		static Long nd[10];
		fill(nd, nd+10, 0);
		for (int i = 0; i < 10; ++i) {
			nd[ a.c[i] ] += d[i];
		}
		copy(nd, nd+10, d);
	}
	Long get()const{
		Long r = 0;
		for (int i = 0; i < 10; ++i) {
			r += i * d[i];
		}
		return r;
	}
};
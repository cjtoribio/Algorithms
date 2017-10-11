template<class T, class U>
struct SegmentTree {
	struct Node {
		bool hasCarry = 0;
		int b = 0,e = 0;
		U carry = U(); T val = T();
		Node(){ }
		void join(const Node &l, const Node &r){
			val = l.val + r.val;
			b = l.b;
			e = r.e;
		}
		void update(const U &u){
			carry += u;
			u(val);
			hasCarry = 1;
		}
		void pushDown(Node &l, Node &r){
			if(!hasCarry)return;
			l.update(carry.trim(0,r.e-r.b+1));
			r.update(carry.trim(l.e-l.b+1,0));
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
			V[n].val = T(VEC[b]), V[n].b = b, V[n].e = e;
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
			V[n].update(v.trim(max(b-i,0), max(j-e,0)));
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
struct String {
	int f[26];
	int sz;
	String(){
		fill(f, f+26, 0);
		sz = 0;
	}
	String(char c){
		fill(f, f+26, 0);
		f[c - 'a']++;
		sz = 1;
	}
	String operator+(const String &a)const{ 
		String r;
		r.sz = sz + a.sz;
		for (int i = 0; i < 26; ++i) {
			r.f[i] = f[i] + a.f[i];
		}
		return r;
	}
	bool isPal(){
		int o = 0;
		for (int i = 0; i < 26; ++i) {
			o += f[i] & 1;
		}
		return o <= 1;
	}
	char get(){
//		assert(sz == 1);
		return find(f, f+26, 1) - f + 'a';
	}
};
struct Set {
	int v;
	Set(){}
	Set(int c):v(c){ }
	Set trim(int l, int r)const{
		return *this;
	}
	void operator+=(const Set &s){
		v = s.v;
	}
	void operator()(String &a)const{
		fill(a.f, a.f+26, 0);
		a.f[v] = a.sz;
	}
};
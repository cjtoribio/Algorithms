template<class T, class U>
struct ISegmentTree {
	struct Node {
		bool hasCarry = 0;
		U carry = U(); T val = T();
		int s,e;
		Node(){ }
		void join(const Node &l, const Node &r){
			if(hasCarry)return;
			val = l.val + r.val;
			s = l.s;
			e = r.e;
		}
		void update(const U &u){
			carry += u;
			val = u(val);
			hasCarry = 1;
		}
		void pushDown(Node &l, Node &r){
			if(!hasCarry)return;
			l.update(carry.trim(0, r.e-r.s+1));
			r.update(carry.trim(l.e-l.s+1, 0));
			carry = U();
			hasCarry = 0;
		}
	};
	int N, H;
	vector<Node> V;
	ISegmentTree(int N): N(N), H(sizeof(int) * 8 - __builtin_clz(N)), V(2*N){ }
	template<class I>
	void create(const vector<I> &VEC) {
		for (int i = 0; i < N; ++i)
			V[i+N].val = T(VEC[i]), V[i+N].s = i, V[i+N].e = i;
		for (int i = N - 1; i > 0; --i) 
			V[i].join(V[i<<1], V[i<<1|1]);
	}
	void push(int p) {
		for (int s = H; s > 0; --s) {
			int i = p >> s;
			V[i].pushDown(V[i << 1], V[i << 1 | 1]);
		}
	}
	void build(int p) {
		while (p > 1) 
			p >>= 1, V[p].join(V[p << 1], V[p << 1 | 1]);
	}
	T query(int i, int j) {
		i += N, j += N+1;
		push(i); push(j-1);
		T a,b;
		for (int l = i, r = j; l < r; l >>= 1, r >>= 1) {
			if (l & 1) a = a + V[l++].val;
			if (r & 1) b = V[--r].val + b;
		}
		return a + b;
	}
	void update(int i, int j, const U &v) {
		i += N, j += N+1;
		push(i); push(j-1);
		for (int l = i, r = j; l < r; l >>= 1, r >>= 1) {
		    if (l&1) {
		    	int shL = max(V[l].s - i + N, 0);
		    	int shR = max(j-N-1  - V[l].e, 0);
		    	V[l++].update(v.trim(shL, shR));
		    }
		    if (r&1){
		    	int shL = max(V[r-1].s - i + N, 0);
		    	int shR = max(j-N-1  - V[r-1].e, 0);
		    	V[--r].update(v.trim(shL, shR));
		    }
		}
		build(i); build(j-1);
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
	String operator()(const String &a)const{
		String r;
		r.f[v] = a.sz;
		r.sz = a.sz;
		return r;
	}
};
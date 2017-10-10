template<class T, class U>
struct ISegmentTree {
	struct Node {
		int sz = 1;
		bool hasCarry = 0;
		U carry = U(); T val = T();
		Node(){ }
		void join(const Node &l, const Node &r){
			if(hasCarry)return;
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
	int N, H;
	vector<Node> V;
	ISegmentTree(int N): N(N), H(sizeof(int) * 8 - __builtin_clz(N)), V(2*N){ }
	template<class I>
	void create(const vector<I> &VEC) {
		for (int i = 0; i < N; ++i)
			V[i+N].val = T(VEC[i]);
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
		    if (l&1) V[l++].update(v);
		    if (r&1) V[--r].update(v);
		}
		build(i); build(j-1);
	}
};
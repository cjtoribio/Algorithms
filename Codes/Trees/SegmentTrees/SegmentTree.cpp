template<class T, class U>
struct SegmentTree2 {
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
#define idx(b, e) ((b+e)|(b!=e)) // memory optimized index
    int N;
    SegmentTree2(int N) : V(2*N), N(N){ }
    template<class I>
    void create(const vector<I> &VEC, int b = 0, int e = -1) {
        if(e == -1) e = N-1;
        int n = idx(b, e);
        if (b == e) {
            V[n].val = T(VEC[b]), V[n].b = b, V[n].e = e;
        } else {
            int m = (b + e) / 2;
            create(VEC, b, m);
            create(VEC, m + 1, e);
            V[n].join(V[idx(b, m)], V[idx(m+1, e)]);
        }
    }
    T query(int i, int j, int b = 0, int e = -1) {
        if(e == -1) e = N-1;
        int n = (b + e) | (b != e);
        if (i <= b && e <= j){
            return V[n].val;
        } else {
            int m = (b + e) / 2;
            V[n].pushDown(V[idx(b, m)], V[idx(m+1, e)]);
            if (i >  m) return query(i, j, m+1, e);
            if (j <= m) return query(i, j, b, m);
            return query(i, j, b, m) + query(i, j, m+1, e);
        }
    }
    int findOkPrefix(int i, const function<bool(T)> &isOk){
        vector<int> stk;
        stk.reserve(20);
        stk.push_back((N-1) | (N-1 != 0));
        T acum; int sz = 0;
        while(stk.size()) {
            int t = stk.back(); stk.pop_back();
            Node &n = V[t];
            int m = (n.b + n.e) / 2;
            if(n.e < i) continue;
            T newAcum = sz == 0 ? n.val : (acum + n.val);
            if (i <= n.b) {
                if (isOk(newAcum)) {
                    sz += n.e - n.b + 1;
                    acum = newAcum;
                } else {
                    if(n.b == n.e) return sz;
                    stk.push_back(idx(n.b, m));
                    stk.push_back(idx(m+1, n.e));
                }
            } else {
                stk.push_back(idx(m+1, n.e));
                stk.push_back(idx(n.b, m));
            }
        }
    }
    void update(int i, int j, const U &v, int b = 0, int e = -1) {
        if(e == -1) e = N-1;
        int n = idx(b, e);
        if (i <= b && e <= j) {
            V[n].update(v.trim(max(b-i,0), max(j-e,0)));
        } else if (i > e || j < b)
            return;
        else {
            int m = (b + e) / 2;
            int l = idx(b, m), r = idx(m+1, e);
            V[n].pushDown(V[l], V[r]);
            update(i, j, v, b, m);
            update(i, j, v, m + 1, e);
            V[n].join(V[l], V[r]);
        }
    }
};
typedef long long Long;
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
	Long get()const{
		Long r = 0;
		for (int i = 0; i < 10; ++i) {
			r += i * d[i];
		}
		return r;
	}
};
struct Replace {
	char c[10];
	Replace(int from = 0, int to = 0){
		for (int i = 0; i < 10; ++i) {
			c[i] = i;
		}
		c[from] = to;
	}
	Replace trim(int l, int r)const{ return *this; }
	void operator+=(const Replace &a){
		static char nc[10];
		for (int i = 0; i < 10; ++i) {
			nc[i] = a.c[c[i]];
		}
		copy(nc, nc+10, c);
	}
	void operator()(DigitSum &a)const{
		static Long nd[10];
		fill(nd, nd+10, 0);
		for (int i = 0; i < 10; ++i) {
			nd[ c[i] ] += a.d[i];
		}
		copy(nd, nd+10, a.d);
	}
};

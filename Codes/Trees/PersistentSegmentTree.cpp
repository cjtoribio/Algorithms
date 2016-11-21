template<class Data>
struct SegmentTree {
	struct Node {
		Node *l, *r;
		int b,e;
		Data data;
		Node(int i, int v):l(NULL),r(NULL),b(i),e(i),data(v){}
		Node(Node *l, Node *r):l(l),r(r),b(l->b),e(r->e){
			data = l->data + r->data;
		}
		Node *copy(){
			return new Node(*this);
		}
		void print(int L=0){
			if(l)l->print(L+1);
			cout << string(L,'\t'); data.print(); cout << endl;
			if(r)r->print(L+1);
		}
	};
	struct Iterator {
		Node *v;
		Iterator(Node *v):v(v){}
		Data& operator*(){
			return v->data;
		}
		Data* operator->(){
			return &v->data;
		}
		int size(){
			return v->e - v->b + 1;
		}
		Iterator& left(){  
			if(v->l) v = v->l;
			return *this;
		}
		Iterator& right(){ 
			if(v->r) v = v->r;
			return *this;
		}
	};
	vector<Node*> V;
	int N;
	SegmentTree(int N):N(N){}
	template <typename Iterator>
	Node* build(Iterator &T){
		V.push_back(build(T,0,N-1));
	}
	template <typename Iterator>
	Node* build(Iterator &T, int b, int e){
		if(b == e)return new Node(b, T[b]);
		int m = (b+e)>>1;
		return new Node(build(T,b,m) , build(T,m+1,e));
	}
	Node* update(int x, int v, int ver = -1){
		Node *nv = update(ver==-1 ? V.back() : V[ver], x, v);
		V.push_back(nv);
		return nv;
	}
	Data query(int i, int j, int ver = -1){
		return query(ver==-1 ? V.back() : V[ver], i, j);
	}
	Node* root(int ver = -1){
		return ver == -1 ? V.back() : V[ver];
	}
	Node* update(Node *n, int x, int v){
		int b = n->b, e = n->e;
		if(b == e){
			Node* c = n->copy();
			c->data.update(v);
			return c;
		}else{
			int m = (b+e)>>1;
			if(x <= m)return new Node(update(n->l,x,v),n->r);
			else 	  return new Node(n->l,update(n->r,x,v));
		}
	}
	Data query(Node *n, int i, int j){
		int b = n->b, e = n->e;
		if(i <= b && e <= j)return n->data;
		int m = (b+e)>>1;
		if(j <= m)return query(n->l, i, j);
		if(m  < i)return query(n->r, i, j);
		return query(n->l, i, j) + query(n->r, i, j);
	}
	int size(){
		return V.size();
	}
	void print(int ver = -1){
		if(ver == -1){
			V.back()->print();
		}else{
			V[ver]->print();
		}
	}
};



struct Data{
	int tot, left;
	Data(int v=0):tot(tot),left(0){}
	Data operator+(const Data &D)const{
		Data d; 
		d.tot = tot + D.tot;
		d.left= tot;
		return d;
	}
	void print(){
		cout << left;
	}
	void update(int v){
		tot += v;
	}
};
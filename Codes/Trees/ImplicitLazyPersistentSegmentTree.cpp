struct Node {
	Node *l, *r;
	bool hasChild, lazy;
	int b,e;
	int sum, carry;
	Node(int b, int e):l(NULL),r(NULL),hasChild(false),lazy(false),b(b),e(e){
		sum = carry = 0;
	}
	Node* clone(){
		Node *n = new Node(*this);
		n->hasChild = false;
		return n;
	}
	void push(){
		if(!hasChild){
			l = l ? l->clone() : new Node(b, (b+e)/2);
			r = r ? r->clone() : new Node((b+e)/2+1, e);
			hasChild = true;
		}
		if(lazy){
			l->update(carry); r->update(carry);
			lazy = carry = 0;
		}
	}
	Node* update(int val){
		lazy = 1;
		carry += val;
		sum += val * (e - b + 1);
		return this;
	}
	void update(){
		sum = (l?l->sum:0) + (r?r->sum:0);
	}
	Node operator+(const Node &n)const{
		Node r(b, n.e);
		r.sum = sum + n.sum;
		return r;
	}
};

Node* create(int N){
	return new Node(0, N-1);
}
Node* update(Node *v, int i, int j, int val){
	if(i <= v->b && v->e <= j){
		return v->clone()->update(val);
	}else{
		int m = (v->b+v->e)/2;
		v->push();
		Node* n = v->clone();
		if(m >= i) n->l = update(v->l, i, j, val);
		if(m <  j) n->r = update(v->r, i, j, val);
		n->update();
		return n;
	}
}
Node query(Node *v, int i, int j){
	if(i <= v->b && v->e <= j){
		return *v;
	}else{
		int m = (v->b+v->e)/2;
		v->push();
		if(m >= j) return query(v->l, i, j);
		if(m <  i) return query(v->r, i, j);
		return query(v->l, i, j) + query(v->r, i, j);
	}
}
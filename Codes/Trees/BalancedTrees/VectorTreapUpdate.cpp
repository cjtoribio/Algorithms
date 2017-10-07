template<class T, class U>
struct VectorTreap {
	struct Node{
		int y, sz;
		T x, sum;
		U carry;
		Node *l, *r;
		Node(T x):x(x),sum(x){ y = rand(); sz = 1; l = r = NULL; }
		void update(){
			sz = 1 + (r?r->sz:0) + (l?l->sz:0);
			sum= x + (r?r->sum:T()) + (l?l->sum:T());
		}
		void apply(U u){
			carry += u;
			u.apply(x, sum, sz);
		}
		void push(){
			if(l)l->apply(carry);
			if(r)r->apply(carry);
			carry = U();
		}
		int lsz(){ return (l?l->sz:0); }
	};
	Node *root;
	VectorTreap(){ root = NULL; }
	void split (Node *t, int sz, Node *&l, Node *&r) {
		if(t)t->push();
		if (!t)l = r = NULL;
		else if ( sz <= t->lsz() ) split (t->l, sz, l, t->l), r = t;
		else                       split (t->r, sz-t->lsz()-1, t->r, r), l = t;
		if(t)t->update();
	}
	void insertAt(int pos, T x){ insertAt(pos, root, new Node(x)); }
	void insertAt (int sz, Node *&t, Node *it) {
		if(t)t->push();
		if(!t)t = it;
		else if(it->y > t->y)   split(t, sz, it->l, it->r), t = it;
		else if(sz <= t->lsz()) insertAt(sz, t->l, it);
		else                    insertAt(sz - t->lsz() - 1, t->r, it);
		if(t)t->update();
	}
	void eraseAt(int pos){ if(root) eraseAt(pos, root); }
	void eraseAt(int sz, Node *&t){
		if(t)t->push();
		if(sz == t->lsz())     delete merge(t, t->l, t->r);
		else if(sz < t->lsz()) eraseAt(sz, t->l);
		else                   eraseAt(sz - t->lsz()-1, sz, t->r);
		if(t)t->update();
	}
	Node* merge (Node *&t, Node *l, Node *r) {
		if(l)l->push();
		if(r)r->push();
		Node *old = t;
		if (!l || ! r) t = l? l: r;
		else if (l->y > r->y) merge (l->r, l->r, r), t = l;
		else                  merge (r->l, l, r->l), t = r;
		if(t)t->update();
		return old;
	}
	int size(){ return root?root->sz:0; }
	T range(int b, int e){
		Node *l, *m, *r;
		split(root, e+1, l, r); split(l, b, l, m);
		T cnt = m ? m->sum : T();
		merge(l, l, m); merge(root, l, r);
		return cnt;
	}
	void apply(int b, int e, U u){
		Node *l, *m, *r;
		split(root, e+1, l, r); split(l, b, l, m);
		if(m)m->apply(u);
		merge(l, l, m); merge(root, l, r);
	}
	void clear(){ clear(root);}
	void clear(Node *&t){
		if(!t)return;
		clear(t->l); clear(t->r);
		delete t; t = NULL;
	}
	void print(bool asArray = false)const{
		if(root)asArray ? printArray(root) : printTree(root);
		cout << endl;
	}
	void printArray(Node *p, int LVL = 0)const{
		p->push();
		if(p->l)printArray(p->l, LVL+1);
		cout << (p->x) << " ";
		if(p->r)printArray(p->r, LVL+1);
	}
	void printTree(Node *p, int LVL = 0)const{
		p->push();
		if(p->r)printTree(p->r, LVL+1);
		cout <<string(LVL*3,' ')<< (p->x) << "\n";
		if(p->l)printTree(p->l, LVL+1);
	}
};
template<class T>
struct NOP {
	void apply(T &x, T &sum, int sz){ }
	NOP operator+=(const NOP &N)const{ return NOP(); }
};
struct Add {
	int val = 0;
	void apply(int &x, int &sum, int sz){
		x += val;
		sum += val * sz;
	}
	void operator+=(const Add &N){ 
		val += N.val;
	}
};
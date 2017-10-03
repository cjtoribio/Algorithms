template<class T>
struct VectorTreap {
	struct Node{
		int y,sz;
		T x;
		Node *l, *r;
		Node(T x):x(x){ y = rand(); sz = 1; l = r = NULL; }
		void update(){
			sz = 1 + (r?r->sz:0) + (l?l->sz:0);
		}
		int lsz(){ return (l?l->sz:0); }
	};
	Node *root;
	VectorTreap(){ root = NULL; }
	void split (Node *t, int sz, Node *&l, Node *&r) {
		if (!t)l = r = NULL;
		else if ( sz <= t->lsz() ) split (t->l, sz, l, t->l), r = t;
		else                       split (t->r, sz-t->lsz()-1, t->r, r), l = t;
		if(t)t->update();
	}
	void insertAt(int pos, T x){ insertAt(pos, root, new Node(x)); }
	void insertAt (int sz, Node *&t, Node *it) {
		if(!t)t = it;
		else if(it->y > t->y)   split(t, sz, it->l, it->r), t = it;
		else if(sz <= t->lsz()) insertAt(sz, t->l, it);
		else                    insertAt(sz - t->lsz() - 1, t->r, it);
		t->update();
	}
	void eraseAt(int pos){ if(root) eraseAt(pos, root); }
	void eraseAt(int sz, Node *&t){
		if(sz == t->lsz())     delete merge(t, t->l, t->r);
		else if(sz < t->lsz()) eraseAt(sz, t->l);
		else                   eraseAt(sz - t->lsz()-1, sz, t->r);
		if(t)t->update();
	}
	Node* merge (Node *&t, Node *l, Node *r) {
		Node *old = t;
		if (!l || ! r) t = l? l: r;
		else if (l->y > r->y) merge (l->r, l->r, r), t = l;
		else 				  merge (r->l, l, r->l), t = r;
		if(t)t->update();
		return t;
	}
	void clear(){ clear(root);}
	void clear(Node *&t){
		if(!t)return;
		clear(t->l); clear(t->r);
		delete t; t = NULL;
	}
	void print(bool asArray = false){
		asArray ? printArray(root) : printTree(root);
		cout << endl;
	}
	void printArray(Node *p, int LVL = 0){
		if(!p)return;
		printArray(p->l, LVL+1);
		cout << (p->x) << " ";
		printArray(p->r, LVL+1);
	}
	void printTree(Node *p, int LVL = 0){
		if(!p)return;
		printTree(p->r, LVL+1);
		cout <<string(LVL*3,' ')<< (p->x) << "\n";
		printTree(p->l, LVL+1);
	}
};
struct LinkCutTree {
	struct Node {
		Node *p, *r, *l;
		int sz,gid, id,rev;
		Node() {
			static int IDX = 0;
			p = r = l = NULL;
			gid = IDX++;
			rev = 0;
			id = 0;
			sz = 1;
		}
		bool isRoot() {
			return p == NULL;
		}
		void pushDown() {
			if(rev){
				swap(r,l);
				if(r)r->rev ^= 1;
				if(l)l->rev ^= 1;
				rev = 0;
			}
		}
		void update() {
			sz = 1 + (r?r->sz:0) + (l?l->sz:0);
		}
	};
	Node* root;
	vector<Node> V;
	int N;
	LinkCutTree(int N) :
			N(N) {
		V = vector<Node>(N);
		root = &V[0];
		for (int i = 0; i < N; ++i){
			V[i].id = i;
			V[i].sz = N-i;
			if(i+1<N){
				V[i].r = &V[i+1];
				V[i+1].p = &V[i];
			}
		}
	}
	void moveUp(Node *v) {
		if (!v->p)
			return;
		Node *p = v->p;
		p->pushDown();
		v->pushDown();
		Node *g = p->p;
		p->p = v;
		v->p = g;
		if (p->l == v) {
			p->l = v->r;
			v->r = p;
			if (p->l)
				p->l->p = p;
		} else {
			p->r = v->l;
			v->l = p;
			if (p->r)
				p->r->p = p;
		}
		if (g && (g->l == p || g->r == p))
			(g->l == p ? g->l : g->r) = v;
		else{
			root = v;
		}
		p->update();
		v->update();
	}
	void splay(Node *v) {
		v->pushDown();
		while (!v->isRoot())
			moveUp(v);
	}
	Node* operator[](int k){
		Node *v = root;
		v->pushDown();
		while(k != (v->l?v->l->sz:0)){
			if(k < (v->l?v->l->sz:0)){
				v = v->l;
			}else{
				k -= (v->l?v->l->sz:0)+1; 
				v = v->r;
			}
			v->pushDown();
		}
		splay(v);
		return v;
	}
	void print(Node* v, int LVL = 0){
		v->pushDown();
		if(v->r)print(v->r,LVL+1);
		cout <<string(LVL,'\t')<< v->id << "|" << v->sz << endl;
		if(v->l)print(v->l,LVL+1);
	}
	void print(){ print(root); }
	void reverseB(Node *B){
		splay(B);
		B->pushDown();
		if(B->r)
		{
			Node *r = B->r;
			r->pushDown();
			while(r->l){
				r = r->l;
				r->pushDown();
			}
			splay(r);
		}
		B->rev ^= 1;
	}
	void reverse(int i,int j){
		if(i == 0){
			Node* J = (*this)[j];
			reverseB(J);
		}else{
			Node* J = (*this)[j];
//			cout << " -- " << J->id << endl;
			reverseB(J);
			Node* I = (*this)[j-i];
//			cout << " -- " << I->id << endl;
			reverseB(I);
			J = (*this)[j];
//			cout << " -- " << J->id << endl;
			reverseB(J);
//			print();
		}
		
	}
};
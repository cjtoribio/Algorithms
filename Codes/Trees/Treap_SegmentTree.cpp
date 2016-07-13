#define WHITE 3
#define BLACK 2
#define FLIP 1
#define NONE 0
struct Node{
	int y,sz,rev;
	int id, white, black, carr, color;
	bool pending;
	Node *l, *r;
	Node(){
		y = rand() % 1000000000;
		sz = 1;
		l = r = NULL;
		rev = 0;
		y = 0;
		id = 0;
		white = 1;
		black = 0;
		color = WHITE;
		carr = 0;
		pending = false;
	}
	int x(){
		return (l?l->sz:0);
	}
	void push(){
		if(rev){
			swap(r,l);
			if(r)r->rev ^= 1;
			if(l)l->rev ^= 1;
			rev = 0;
		}
		if(pending){
			if(r)r->applyChange(carr);
			if(l)l->applyChange(carr);
			pending = false;
			carr = NONE;
		}
	}
	void update(){
		sz = 1 + (r?r->sz:0) + (l?l->sz:0);
		white = (color == WHITE) + (l?l->white:0) + (r?r->white:0);
		black = sz - white;
	}
	void applyChange(int v){
		if(v == WHITE){
			white = sz;
			black = 0;
			carr = WHITE;
			color = WHITE;
			pending = true;
		} else if(v == BLACK){
			black = sz;
			white = 0;
			carr = BLACK;
			color = BLACK;
			pending = true;
		} else if(v == FLIP){
			swap(black, white);
			color = (color == WHITE) ? BLACK : WHITE;
			switch(carr){
			case NONE:
				carr = v;
				break;
			case WHITE:
				carr = BLACK;
				break;
			case BLACK:
				carr = WHITE;
				break;
			case FLIP:
				carr = NONE;
				break;
			}
			pending = true;
		}
	}
};
struct Treap {
	typedef pair<Node*, Node*> PNN;
	vector<Node> V;
	Node *root;
	Treap(int N){
		root = NULL;
		V = vector<Node>(N);
		build(root, V, 0, N-1);
		
	}
	int build(Node *&t, vector<Node> &V, int i, int j){
		if(j<i){
			t = NULL;
			return 0;
		}else if(i==j){
			t = &V[i];
			t->y = 1;
		}else{
			int m = (i+j)/2;
			t = &V[m];
			t->y = 1 + max(build(t->l, V,i,m-1),build(t->r, V,m+1,j));
		}
		t->update();
		return t->y;
	}
	void split (Node *t, int x, Node *&l, Node *&r) {
		if(t)t->push();
		if (!t)l = r = NULL;
		else if ( x < t->x() )
			split (t->l, x, l, t->l), r = t;
		else
			split (t->r, x - t->x() - 1, t->r, r), l = t;
		if(t)t->update();
	}
//	
	void insert (Node *&t, int x, Node *it) {
		Node *t1, *t2;
		split(t, x, t1, t2);
		merge(t1, t1, it);
		merge(t, t1, t2);
	}
	void merge (Node *&t, Node *l, Node *r) {
		if(l)l->push();
		if(r)r->push();
		if (!l || ! r){
			t = l? l: r;
		}else if (l->y > r->y){
			merge (l->r, l->r, r);
			t = l;
		}else{
			merge (r->l, l, r->l);
			t = r;
		}
		if(t)t->update();
	}
	void reverse(int i, int j){
		Node *l, *m, *r;
		split(root, j, l, r);
		split(l, i-1, l, m);
		m->rev ^= 1;
		merge(l, l, m);
		merge(root, l, r);
	}
	void applyChange(int i, int j, int v){
		Node *l, *m, *r;
		split(root, j, l, r);
		split(l, i-1, l, m);
		m->applyChange(v);
		merge(l, l, m);
		merge(root, l, r);
	}
	Node query(int i, int j){
		Node *l, *m, *r, q;
		split(root, j, l, r);
		split(l, i-1, l, m);
		q = *m;
		merge(l, l, m);
		merge(root, l, r);
		return q;
	}
	void print(){ 
		print(root);
		cout << endl;
	}
	void print(Node *p, int LVL = 0){
		if(!p)return;
		p->push();
		print(p->r, LVL+1);
		cout << (p->color == WHITE) << " ";
		print(p->l, LVL+1);
	}
};
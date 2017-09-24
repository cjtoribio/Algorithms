struct PTrie {
	static const int DEPTH = 2;
	static const int BITS = 2;
	static const int MAXN = 1<<(DEPTH * BITS); // exclusive
	static const int MASK = (1<<BITS)-1; // exclusive
	static const int CHILDS = 1 << BITS;
	int sum = 0;
	PTrie* child[CHILDS];
	PTrie(){ memset(child,0, CHILDS * sizeof(PTrie*)); }
	PTrie* clone()const{
		return new PTrie(*this);
	}
	PTrie* add(int v, int x){
		PTrie *ret = clone();
		PTrie *r = ret;
		r->sum += x;
		for(int i = 0; i < DEPTH; ++i){
			int p = (v >> ((DEPTH-1-i) * BITS)) & MASK;
			if(r->child[p] == NULL){
				r->child[p] = new PTrie();
			}else{
				r->child[p] = r->child[p]->clone();
			}
			r = r->child[p];
			r->sum += x;
		}
		return ret;
	}
	void print(int lvl = 0){
		for(int i = 0; i < CHILDS/2; ++i){
			if(child[i])child[i]->print(lvl+1);
		}
		cout << string(lvl, '\t') << sum << endl;
		for(int i = CHILDS/2; i < CHILDS; ++i){
			if(child[i])child[i]->print(lvl+1);
		}
	}
	void sorted(int p = 0, int lvl = 0){
		if(lvl == DEPTH)cout << p << " " << sum << endl;
		for (int i = 0; i < CHILDS; ++i) {
			if(child[i]){
//				cout << string(lvl+1, '-') << i << endl;
				child[i]->sorted((p << BITS) | i, lvl+1);
			}
		}
	}
	int countLess(int v){
		PTrie *r = *this;
		int cnt = 0;
		for(int i = 0; i < DEPTH; ++i){
			int p = (v >> ((DEPTH-1-i) * BITS)) & MASK;
			for(int j = 0; j < p; ++j)
				if(r->child[j])
					cnt += r->child[j]->sum;
			if(r->child[p] == NULL)break;
			r = r->child[p];
		}
		return cnt;
	}
};
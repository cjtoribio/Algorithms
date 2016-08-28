template<int FAC, int MOD>
struct Hash {
	ModInt<MOD> m;
	int sz;
	Hash():sz(0){}
	void push_back(const int &c){
		m = m*FAC + c;
		sz++;
	}
	void pop_back(const int &c){
		m = (m - c) / FAC;
	}
	void push_front(const int &c){
		ModInt<MOD> t = FAC;
		t = t.power(sz);
		t = t * c;
		m += t;
		sz++;
	}
	void pop_front(const int &c){
		sz--;
		ModInt<MOD> t = FAC;
		t = t.power(sz);
		t = t * c;
		m -= t;
	}
	bool operator==(const Hash<FAC, MOD> &H)const{
		return m.n == H.m.n;
	}	
};
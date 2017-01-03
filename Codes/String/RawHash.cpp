#define HASH_COUNT 1
Long PR[] = {31, 63};
Long MO[] = {1000000007, 1000000009};
VI po[] = {
		VI(1,1),VI(1,1)
};
struct Hash{
	int v[HASH_COUNT];
	Hash(){ 
		memset(v,0,sizeof(int)*HASH_COUNT);
	}
	Hash up(int p){
		Hash r(*this);
		for (int i = 0; i < HASH_COUNT; ++i) {
			while(po[i].size() <= p){
				po[i].push_back( po[i].back() * PR[i] % MO[i] );
			}
			r.v[i] = (1LL * r.v[i] * po[i][p]) % MO[i];
		}
		return r;
	}
	Hash push(int e){
		Hash r = up(1);
		for (int i = 0; i < HASH_COUNT; ++i) {
			r.v[i] = (r.v[i] + e) % MO[i];
		}
		return r;
	}
	Hash operator+(const Hash h)const{
		Hash r = (*this);
		for (int i = 0; i < HASH_COUNT; ++i) {
			r.v[i] = (r.v[i] + h.v[i]);
			if(r.v[i] > MO[i])r.v[i] -= MO[i];
		}
		return r;
	}
	Hash operator-(const Hash h)const{
		Hash r = (*this);
		for (int i = 0; i < HASH_COUNT; ++i) {
			r.v[i] = (r.v[i] - h.v[i]);
			if(r.v[i] < 0)r.v[i] += MO[i];
		}
		return r;
	}
	bool operator<(const Hash &other)const{
		for (int i = 0; i < HASH_COUNT; ++i) {
			if(v[i] != other.v[i]){
				return v[i] < other.v[i];
			}
		}
		return false;
	}
};
ostream& operator<<(ostream &out, const Hash &H){
	out << "{ ";
	for (int i = 0; i < HASH_COUNT; ++i) {
		out << H.v[i] << " ";
	}
	out << "}";
	return out;
}
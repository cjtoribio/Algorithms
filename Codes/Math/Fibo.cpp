// If F[0] == 0 then b     or mult(1, 0).second
// If F[0] == 1 then get() or mult(1, 0).first
#define MOD 1000000007
#define add(a,b) (a+b>=MOD ? a+b-MOD : a+b)
struct Fibo{
	int b,c;
	Fibo(int b=0,int c=0):b(b),c(c){ }
	static Fibo one(){ // first fibo
		return Fibo(1,0);
	}
	static Fibo none(){ // neg-first fibo
		return Fibo(1,MOD-1);
	}
	static Fibo empty(){
		return Fibo(0,0);
	}
	static Fibo identity(){
		return Fibo(0,1);
	}
	static Fibo get(long long n){
		return n >= 0 ? Fibo::one().pow(n) : Fibo::none().pow(-n);
	}
	int get(){
		return add(b,c);
	}
	pair<int,int> mult(int B, int A){
		return make_pair((1LL*B*get()+1LL*A*b)%MOD,  (1LL*B*b+1LL*A*c)%MOD);
	}
	Fibo operator*(const Fibo &F)const{
		Fibo r;
//		r.a = (1LL*a*F.a + 1LL*b*F.b)%MOD;
		r.b = (1LL*(b+c)*F.b + 1LL*b*F.c)%MOD;
		r.c = (1LL*b*F.b + 1LL*c*F.c)%MOD;
		return r;
	}
	Fibo operator+(const Fibo &F)const{
		return Fibo(add(b,F.b), add(c,F.c));
	}
	Fibo pow(long long n){
		Fibo m = *this;
		Fibo r = identity();
		while(n){
			if(n & 1)r = r * m;
			m = m * m;
			n >>= 1;
		}
		
		return r;
	}
};

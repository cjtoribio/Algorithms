namespace BigPrimes {
	typedef long long ll;
	ll gcd(ll a, ll b) {
		return a ? gcd(b % a, a) : b;
	}
	
	ll mulmod(ll a, ll b, ll c) { //returns (a*b)%c, and minimize overfloor
		return ((__int128_t ) a * b) % c;
		ll x = 0, y = a % c;
		while (b > 0) {
			if (b % 2 == 1)
				x = (x + y) % c;
			y = (y * 2) % c;
			b /= 2;
		}
		return x % c;
	}
	
	ll expmod(ll b, ll e, ll m) { //O(log b)
		if (!e)
			return 1;
		ll q = expmod(b, e / 2, m);
		q = mulmod(q, q, m);
		return e % 2 ? mulmod(b, q, m) : q;
	}
	
	bool es_primo_prob(ll n, int a) {
		if (n == a)
			return true;
		ll s = 0, d = n - 1;
		while (d % 2 == 0)
			s++, d /= 2;
	
		ll x = expmod(a, d, n);
		if ((x == 1) || (x + 1 == n))
			return true;
		for (int i = 0; i < s-1; ++i) {
			x = mulmod(x, x, n);
			if (x == 1)
				return false;
			if (x + 1 == n)
				return true;
		}
		return false;
	}
	
	bool rabinPrime(ll n) { //devuelve true si n es primo
		if (n == 1)
			return false;
		const int ar[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
		for(int j = 0; j < 12; ++j)
			if (!es_primo_prob(n, ar[j]))
				return false;
		return true;
	}
	
	ll rho(ll n) {
		if ((n & 1) == 0)
			return 2;
		ll x = 2, y = 2, d = 1;
		ll c = rand() % n + 1;
		while (d == 1) {
			x = (mulmod(x, x, n) + c) % n;
			y = (mulmod(y, y, n) + c) % n;
			y = (mulmod(y, y, n) + c) % n;
			if (x - y >= 0)
				d = gcd(x - y, n);
			else
				d = gcd(y - x, n);
		}
		return d == n ? rho(n) : d;
	}
	void factRho(ll n, map<long long, int> &prim) { // O( (logN)^3 )
		if (n == 1)
			return;
		if (rabinPrime(n)) {
			prim[n]++;
			return;
		}
		ll factor = rho(n);
		factRho(factor, prim);
		factRho(n / factor, prim);
	}
}

int modPow(int b, int e, int m) {
	if (e == 0) return 1;
	int sq = modPow(b, e >> 1, m);
	sq = 1LL * sq * sq % m;
	return (e & 1) ? (sq * 1LL * b % m) : sq;
}
// a*x = b (mod m)
int solveLinearCongruence(int a, int b, int m) {
	if (b == 0) return 0;
	if (a == 0) return -1;
	if (a == 1) return b;
	int y = solveLinearCongruence(m % a, (-b % a + a) % a, a);
	return y < 0 ? y : (1LL*m*y+b) / a % m;
}
// a^output = b
int discreteLogarithm2(int a, int b, int m) {
	int o = m-1;
	auto sub = [] (int a, int b, int m) { return ((a - b) % m + m) % m; };
	auto gen = [&](Long &x, Long &p, Long &q) {
		switch (x % 3) {
			case 0: x = x * x % m, p = p * 2 % o, q = q * 2 % o; break;
			case 1: x = x * a % m, p = (p == o-1) ? 0 : p+1; break;
			case 2: x = x * b % m, q = (q == o-1) ? 0 : q+1; break;
		}
	};
	Long x = 1, p = 0, q = 0;
	Long X = x, P = p, Q = q;
	for (int i = 1; ; ++i) {
		gen(x, p, q);
		gen(X, P, Q); gen(X, P, Q); // double step
		if (x == X) {
			int r = sub(q, Q, o);
			int num = sub(P, p, o);
			if (r == 0) return -1;
			int gc = __gcd(r, o);
			int s = solveLinearCongruence(r, num, o);
			if (s == -1) return -1;
			for (int t = 0; t < gc; ++t) {
				int cn = s + o / gc * t;
				if (modPow(a, cn, m) == b) {
					return cn;
				}
			}
			return -1;
		}
	}
}

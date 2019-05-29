// a*x = b (mod m)
int solveLinearCongruence(int a, int b, int m) {
	if (b == 0) return 0;
	if (a == 0) return -1;
	if (a == 1) return b;
	int y = solveLinearCongruence(m % a, (-b % a + a) % a, a);
	return y < 0 ? y : (1LL*m*y+b) / a % m;
}
VI allSolutions(int a, int b, int m) {
	int x = solveLinearCongruence(a, b, m);
	if (x == -1) return {};
	VI V;
	int gc = __gcd(a, m);
	for (int i = 0; i < gc; ++i) V.push_back(x + m / gc * i);
	return V;
}

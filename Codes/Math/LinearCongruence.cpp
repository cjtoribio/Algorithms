// https://www.johndcook.com/blog/2008/12/10/solving-linear-congruences/
// a*x = b (mod m) 
// -1 (no solution if gcd(a, m) dont divide b) or x (a solution)
// there exist gcd(a, m) solutions all of which are x + m/gcd(a,m)*[0...gcd-1]
int solveLinearCongruence(int a, int b, int m) {
	if (b == 0) return 0;
	if (a == 0) return -1;
	if (a == 1) return b;
	int y = solveLinearCongruence(m % a, a - b % a, a);
	return y < 0 ? y : (1LL*m*y+b) / a % m;
}

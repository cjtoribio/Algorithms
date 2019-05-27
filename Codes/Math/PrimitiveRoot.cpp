// https://math.stackexchange.com/questions/124408/finding-a-primitive-root-of-a-prime-number
int modPow(int b, int e, int m) {
	if (e == 0) return 1;
	int sq = modPow(b, e >> 1, m);
	sq = 1LL * sq * sq % m;
	return (e & 1) ? (sq * 1LL * b % m) : sq;
}
vector<int> getPrimeFactors(int x) {
	vector<int> PRIMES;
	for (int i = 2; i * i <= x; ++i) {
		if (x % i == 0) {
			PRIMES.push_back(i);
			while (x % i == 0) x /= i;
		}
	}
	if (x > 1) PRIMES.push_back(x);
	return PRIMES;
}
int phi(int x) {
	auto pr = getPrimeFactors(x);
	int n = x;
	for (int p : pr) {
		n = n / p * (p-1);
	}
	return n;
}
int getPrimitiveRoot(int x) {
	if (x <= 4) {
		vector<int> ans = {-1, 0, 1, 2, 3};
		return ans[x];
	}
	auto pr = getPrimeFactors((x & 1) ? x : (x / 2));
	if (pr.size() > 1 || pr[0] == 2) {
		return -1;
	}
	int s = phi(x);
	vector<int> PRIMES = getPrimeFactors(s);
	// it should randomly find a primitive root since probability is high
	for (int i = 0; i < 100; ++i) {
		int r = rand() % (x - 1) + 1;
		bool ok = 1;
		if (__gcd(r, x) != 1) ok = 0;
		for (int j = 0; ok && j < PRIMES.size(); ++j) {
			if (modPow(r, s / PRIMES[j], x) == 1) {
				ok = 0;
			}
		}
		if (ok) return r;
	}
	assert(false);
	return -1;
}
vector<int> getAllPrimitiveRoots(int x) {
	int s = phi(x);
	int r = getPrimitiveRoot(x);
	if (r == -1) return {};
	vector<int> R;
	for (int i = 1; i <= s; ++i) {
		if (__gcd(i, s) == 1) {
			R.push_back(modPow(r, i, x));
		}
	}
	sort(R.begin(), R.end());
	return R;
}

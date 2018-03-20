typedef long long Long;
typedef long double Double;
typedef long long Long;
typedef vector<int> VI;
struct point {
	Double x, y;
	point(Double x = 0, Double y = 0) : x(x), y(y) { }
	point conjugate() const { return point(x, -y); }
};
point operator+(const point &a, const point &b) { return {a.x + b.x, a.y + b.y}; }
point operator-(const point &a, const point &b) { return {a.x - b.x, a.y - b.y}; }
point operator*(const point &a, const point &b) { return {a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x}; }
point operator/(const point &a, Double d) { return {a.x / d, a.y / d}; }
void fft(vector<point> &a, int sign = +1) {
	int n = a.size();
	for (int i = 1, j = 0; i < n - 1; ++i) {
		for (int k = n >> 1; (j ^= k) < k; k >>= 1);
		if (i < j) swap(a[i], a[j]);
	}
	Double theta = 2 * atan2(0, -1) * sign;
	for (int m, mh = 1; (m = mh << 1) <= n; mh = m) {
		point wm(cos(theta / m), sin(theta / m)), w(1, 0);
		for (int i = 0; i < n; i += m, w = point(1, 0))
			for (int j = i, k = j + mh; j < i + mh; ++j, ++k, w = w * wm) {
				point x = a[j], y = a[k] * w;
				a[j] = x + y;
				a[k] = x - y;
			}
	}
	if (sign == -1) for (point &p : a) p = p / n;
}
VI convolution(const VI &a, const VI &b, int isStraight = true) {
	vector<point> ca(a.size()), cb(b.size());
	for (int i = 0; i < ca.size(); ++i) ca[i] = a[i];
	for (int i = 0; i < cb.size(); ++i) cb[i] = b[i];
	int L = 1;
	if (isStraight) reverse(cb.begin(), cb.end());
	while (L < a.size() + b.size() - 1) L *= 2;
	ca.resize(L); cb.resize(L);
	vector<point> c(L);
	fft(ca, 1); fft(cb, 1);
	for (int i = 0; i < L; ++i) c[i] = ca[i] * cb[i];
	fft(c, -1);
	c.resize(a.size() + b.size() - 1);
	vector<int> cc(c.size());
	for (int i = 0; i < c.size(); ++i)
		cc[i] = round(c[i].x); // dont truncate use round
	return cc;
}
// add code below for BigConvolution
point operator/(const point &a, const point &b) {
	Double m = b.x * b.x + b.y * b.y;
	return point(a.x / m, a.y / m) * point(b.x, -b.y);
}
Long mround(Double A) {
	Long F = A;
	if (A < 0) F = (Long) (A - 0.5);
	if (A > 0) F = (Long) (A + 0.5);
	return F;
}
VI BigConvolution(const VI &a, const VI &b, Long MOD) {
	int ssss = a.size() + b.size() - 1;
	if (ssss <= 1024) { // remove this code if ans never less than 1024
		vector<int> bf(ssss);
		for (int i = 0; i < a.size(); ++i) {
			for (int j = 0; j < b.size(); ++j) {
				bf[i + j] = (bf[i + j] + 1LL * a[i] * b[j]) % MOD;
			}
		}
		return bf;
	}
	int L = 1;
	while ((1 << L) < a.size()) L++;
	while ((1 << L) < b.size()) L++;
	int n = 1 << (L + 1);
	Long SH = 16;
	Long M = (1 << SH) - 1, S = 1 << SH;
	Long S1 = S;
	Long S2 = S * S % MOD;
	vector<point> xy1(n), xy0(n);
	for (int i = 0; i < a.size(); ++i) {
		xy1[i].x = a[i] >> SH;
		xy0[i].x = a[i] & M;
	}
	for (int i = 0; i < b.size(); ++i) {
		xy1[i].y = b[i] >> SH;
		xy0[i].y = b[i] & M;
	}
	fft(xy1, 1); fft(xy0, 1);
	vector<point> z2(n), z1(n), z0(n);
	for (int i = 0; i < n; i++) {
		point X0 = (xy0[i] + xy0[(n - i) % n].conjugate()) / point(2, 0);
		point X1 = (xy1[i] + xy1[(n - i) % n].conjugate()) / point(2, 0);
		point Y0 = (xy0[i] - xy0[(n - i) % n].conjugate()) / point(0, 2);
		point Y1 = (xy1[i] - xy1[(n - i) % n].conjugate()) / point(0, 2);
		z2[i] = X1 * Y1;
		z1[i] = X0 * Y1 + Y0 * X1;
		z0[i] = X0 * Y0;
	}
	fft(z2, -1); fft(z1, -1); fft(z0, -1);
	VI z(a.size() + b.size() - 1);
	for (int i = 0; i < z.size(); ++i) {
		Long dz2 = mround(z2[i].x) % MOD;
		Long dz1 = mround(z1[i].x) % MOD;
		Long dz0 = mround(z0[i].x) % MOD;
		Long l = (dz2 * S2) + (dz1 * S1) + dz0;
		z[i] = l % MOD;
	}
	return z;
}

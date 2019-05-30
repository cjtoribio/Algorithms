struct BitVector {
	typedef unsigned long long ULong;
	vector<ULong> bits;
	vector<ULong> acu;
	struct reference {
		BitVector &o; int p, b;
		reference(BitVector &o, int p) : o(o), p(p/64), b(p&63) { }
		bool operator=(bool v) const {
			if (v) o.bits[p] |= 1ULL << b;
			else   o.bits[p] &= ~(1ULL << b);
			return v;
		}
	};
	BitVector(ULong n = 0) : bits(1, n), acu(1, __builtin_popcountll(n)) { recompute(); }
	BitVector(string s, char one = '1') {
		reverse(s.begin(), s.end());
		bits = vector<ULong>(s.size() / 64 + 1);
		for (int i = 0; i < s.size(); ++i)
			if (s[i] == one)
				bits[i / 64] |= 1ULL << (i & 63);
		while (bits.size() > 1 && bits.back() == 0) bits.pop_back();
		recompute();
	}
	BitVector& operator<<=(const int n) {
		int osz = bits.size();
		int whole = n / 64;
		int extra = n % 64;
		bits.resize(bits.size() + n / 64 + 1);
		for (int i = osz - 1; i >= 0; --i) {
			ULong x = bits[i]; bits[i] = 0;
			bits[i + whole] |= x << extra;
			bits[i + whole + 1] |= x >> (64 - extra);
		}
		while (bits.size() > 1 && bits.back() == 0) bits.pop_back();
		recompute();
	}
	BitVector operator<<(const int n) {
		BitVector o = *this; o <<= n; return o;
	}
	BitVector& operator>>=(const int n) {
		int whole = n / 64;
		int extra = n % 64;
		for (int i = 0; i < whole; ++i) bits[i] = 0;
		for (int i = whole; i < bits.size(); ++i) {
			ULong x = bits[i]; bits[i] = 0;
			bits[i - whole] |= x >> extra;
			bits[i - whole - 1] |= x << (64 - extra);
		}
		while (bits.size() > 1 && bits.back() == 0) bits.pop_back();
		recompute();
	}
	BitVector operator>>(const int n) {
		BitVector o = *this; o >>= n; return o;
	}
	BitVector operator&(const BitVector o) const {
		BitVector r;
		r.bits.resize(min(bits.size(), o.bits.size()));
		for (int i = 0; i < r.bits.size(); ++i) r.bits[i] = bits[i] & o.bits[i];
		return r;
	}
	reference operator[](int p) { return reference(*this, p); }
	bool operator[](int p) const { return (bits[p / 64] >> (p & 63)) & 1; }
	void recompute() {
		acu.resize(bits.size());
		for (int i = 0; i < bits.size(); ++i) {
			acu[i] = __builtin_popcountll(bits[i]);
			if (i > 0) acu[i] += acu[i-1];
		}
	}
	friend ostream &operator<<(ostream &os, const BitVector &bitVector) {
		for (int i = bitVector.bits.size() - 1; i >= 0; --i) {
			os << bitset<64>(bitVector.bits[i]);
		}
		return os;
	}
};

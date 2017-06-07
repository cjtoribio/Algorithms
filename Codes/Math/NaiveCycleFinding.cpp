struct Cycle {
	function<int(int)> func;
	vector<int> seq, pos;
	int cycleSize, offset;
	vector<int> freq;
	Cycle(int domain, int st, function<int(int)> func):func(func),pos(domain,-1),freq(domain){
		do{
			pos[st] = seq.size();
			seq.push_back(st);
			st = func(st);
		}while(pos[st] == -1);
		cycleSize = seq.size() - pos[st];
		offset = seq.size() - cycleSize;
	}
	int get(int n) { // zero-based
		if(n < offset)return seq[n];
		n -= offset;
		n %= cycleSize;
		return seq[n + offset];
	}
	void computeTo(int n) {
		for(int i = 0; i < min(n, offset); ++i) {
			freq[seq[i]]++;
		}
		if(n < offset)return;
		n -= offset;
		int comp = n / cycleSize;
		int res = n % cycleSize;
		for(int i = offset; i < cycleSize + offset; ++i) {
			freq[seq[i]] = comp;
			if(res > (i-offset))
				freq[seq[i]]++;
		}
		for (int i = 1; i < freq.size(); ++i) {
			freq[i] += freq[i-1];
		}
	}
};
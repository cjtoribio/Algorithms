struct Line {
	Long m, b;
	bool operator<(const Line &l)const{
		return m != l.m ? m > l.m : b < l.b;
	}
	static bool best(Long a, Long b){
		return min(a, b);
	}
};
struct ConvexHull {
	vector<vector<Line>> lines;
	ConvexHull():lines(1){ }
	void addLine(Line line){
		lines[0].insert(lower_bound(lines[0].begin(), lines[0].end(), line), line);
		clean(lines[0]);
		for(int i = 0; i < lines.size(); ++i){
			if(lines[i].size() > (1<<i)){
				if(i+1 == lines.size()){
					lines.push_back(lines[i]);
				}else{
					lines[i+1] = join(lines[i], lines[i+1]);
				}
				lines[i].clear();
			}else{
				break;
			}
		}
	}
	Long getBest(Long x){
		bool hasValue = false;
		Long r = 0;
		for(vector<Line> row : lines){
			if(row.size() == 0)continue;
			int lo = 0, hi = row.size()-1, b = -1;
			while(lo <= hi){
				int mi = (lo + hi)/2;
				if(mi == 0 || 1.0*(row[mi-1].b-row[mi].b)/(row[mi].m-row[mi-1].m) <= x){
					b = mi;
					lo = mi+1;
				}else{
					hi = mi-1;
				}
			}
			if(!hasValue) r = row[b].m * x + row[b].b;
			else     r = Line::best(r, row[b].m * x + row[b].b);
			hasValue = 1;
		}
		return r;
	}
	static bool pitty(Line a, Line b, Line c){
		return ((b.b - a.b)*(a.m - c.m) >= (c.b - a.b)*(a.m - b.m));
	}
	static vector<Line> join(const vector<Line> &a, const vector<Line> &b){
		vector<Line> nl; nl.resize(a.size() + b.size());
		merge(a.begin(), a.end(), b.begin(), b.end(), nl.begin());
		clean(nl);
		return nl;
	}
	static void clean(vector<Line> &lines){
		vector<Line> nl; 
		for(Line l : lines){
			if(nl.size() && nl.back().m == l.m)continue;
			while(nl.size() >= 2 && pitty(nl[nl.size()-2], nl.back(), l))
				nl.pop_back();
			nl.push_back(l);
		}
		lines = nl;
	}
};
template <class F>
struct Line {
  Point<F> a, ab;
  Line() : a(), ab() {}
  Line(Point<F> a, Point<F> b, bool twoPoints = true) : a(a), ab(twoPoints ? b - a : b) {}

  Point<F> b() const { return a + ab; }
  operator bool () const { return ab != Point<F>(); }
  bool aligned(Point<F> o)const{ return ((o - a)^(ab)) == 0; }
};

/// Method for line and segment intersection intersection
enum EC { R, C, O }; // Ray, Closed, Open
template<class F>
bool check(EC t, F a, F b){
	if(t == EC::R) return true;
	if(t == EC::C) return a <= b;
	if(t == EC::O) return a < b;
}
template <EC LA, EC LB, EC RA, EC RB, class F1, class F2, class F3>
bool intersectLines(Line<F1> lhs, Line<F2> rhs, Point<F3>& res, bool deb = 0) {
  auto s = lhs.ab ^ rhs.ab;
  if (s == 0) return false;
  auto ls = (rhs.a - lhs.a) ^ rhs.ab;
  auto rs = (rhs.a - lhs.a) ^ lhs.ab;
  if (s < 0) s = -s, ls = -ls, rs = -rs;
  bool intersect = check(LA, decltype(ls)(0), ls) && check(LB, ls, s) && 
		  	  	   check(RA, decltype(rs)(0), rs) && check(RB, rs, s);
  if (intersect) res = lhs.a + lhs.ab * static_cast<F3>(ls) / s;
  return intersect;
}

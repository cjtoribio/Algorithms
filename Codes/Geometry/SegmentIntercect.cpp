/*
(A-C + (B-A)*t) ^ (D-C) = 0
(B-A -> m : D-C -> n)
(a.x + t*m.x)*n.y - (a.y + t*m.y)*n.x = 0
a.x*n.y - a.y*n.x + t*(m^n) = 0
t*(m^n) = n^a
t = - ((a-c)^n) / (m^n)
 */
Point segIntercept(const Point &A, const Point &B, const Point &C, const Point &D){
	Point m = (B-A);
	Point n = (D-C);
	double t = - ((A-C)^n) / (m^n);
	return A + m*t;
}
bool hasIntercept(const Point &A, const Point &B, const Point &C, const Point &D){
	Point m = (B-A);
	Point n = (D-C);
	if(Point::similar(m^n, 0))return false; // parallel
	double t = - ((A-C)^n) / (m^n);
	return 0-1e9 <= t && t <= 1+1e9; // lies between
}
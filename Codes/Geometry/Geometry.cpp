#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

template<class INT> struct Poin {
	INT x, y;
	Poin(INT a = 0, INT b = 0) {
		x = a, y = b;
	}
	Poin operator+(const Poin &P) const {
		return Poin(x + P.x, y + P.y);
	}
	Poin operator-(const Poin &P) const {

		return Poin(x - P.x, y - P.y);
	}
	INT operator*(const Poin &P) const {

		return x * P.x + y * P.y;
	}
	INT operator^(const Poin &P) const {

		return x * P.y - y * P.x;
	}

	INT mag() const {

		return sqrt(x * x + y * y);
	}
	Poin scale(INT H, bool div = false) const {

		return div ? Poin(x / H, y / H) : Poin(x * H, y * H);
	}
	Poin unit() const {

		return scale(mag(), 1);
	}
	bool operator<(const Poin &P) const {

		return x != P.x ? x < P.x : y < P.y;
	}
	bool operator==(const Poin &P) const {
		return x == P.x && y == P.y;
	}
};
typedef Poin<double> Point;

//
Point Circle3Points(Point A, Point B, Point C) {
	//Find the Center of a Circle that has points A,B,C 
	//The Circle3Points needs to be different else use Circle2Points
	Point M1 = (A + B).scale(2, true);
	double a1 = (A - B).y, b1 = (A - B).x, c1 = a1 * M1.y + b1 * M1.x;
	Point M2 = (C + B).scale(2, true);
	double a2 = (C - B).y, b2 = (C - B).x, c2 = a2 * M2.y + b2 * M2.x;
	double det = a1 * b2 - b1 * a2;
	double detY = c1 * b2 - b1 * c2; // <- Easy Bug here 
	double detX = a1 * c2 - c1 * a2;
	return Point(detX / det, detY / det);
}

pair<Point, Point> Circle2Points(Point A, Point B, double R) {
	//Find the Centers of the circles which touch A,B and have radius R 
	Point M = (A + B).scale(2, true); // scale 2 for divide
	Point MA = A - M;
	if (R * R < MA * MA || MA * MA == 0) // does not exist
		return pair<Point, Point>(Point(1e9, 1e9), Point(1e9, 1e9));
	double mag = sqrt(R * R - MA * MA);
	Point M1 = MA;
	swap(M1.x, M1.y);
	M1.x *= -1;
	M1 = M1.unit();
	M1 = M1.scale(mag);
	Point M2 = MA;
	swap(M2.x, M2.y);
	M2.y *= -1;
	M2 = M2.unit();
	M2 = M2.scale(mag);
	return pair<Point, Point>(M + M1, M + M2);
}

double AreaOfPolygon(vector<Point> &V) {
	// Area of a Polygon O(N) {
	//negative area means clockwise positive means anticlockwise
	double Area = 0;
	int N = V.size();
	for (int i = 1; i < N - 1; ++i)
		Area += (V[i] - V[0]) ^ (V[i + 1] - V[0]);
	return Area / 2.0;
}

inline bool Collinear(Point A, Point B, Point C) { // O(1) 
		
	return ((A - B) ^ (C - B)) == 0;
}

inline bool between(Point A, Point B, Point C) {// A-B-C	O(1) 

	// B lines in the segment AC
	if (A == B || C == B)
		return true;

	return (A - C) * (B - C) > 0 && (C - A) * (B - A) > 0 && Collinear(A, B, C);
}

bool InsideTriangle(Point A, Point B, Point C, Point P) {
	// Is Point P insde triangle ABC
	bool bt = between(A, P, B) || between(B, P, C) || between(C, P, A);
	if (((A - B) ^ (C - B)) == 0)
		return bt;
	bool cp = ((((A - B) ^ (P - B)) > 0) == (((B - C) ^ (P - C)) > 0)
			&& (((B - C) ^ (P - C)) > 0) == (((C - A) ^ (P - A)) > 0) );
	return bt || cp;
}

double LinePointD(Point A, Point B, Point C, bool seg = false) {

	//Line is AB the point is C
	if (seg && (A - B) * (C - B) < 0)
		return (B - C).mag();
	if (seg && (B - A) * (C - A) < 0)
		return (A - C).mag();
	return fabs((A - B) ^ (C - B)) / (A - B).mag();
}

Point Rotate(Point A, double delta) {
	// Rotating a Vector O(1)
	return Point(A.x * cos(delta) - A.y * sin(delta),
			A.x * sin(delta) + A.y * cos(delta));
}

pair<Point, int> lineIntersection(Point A1, Point B1, Point A2, Point B2) {
	// Intersection between two segments or two lines 
	// ay + bx = c --> (x2-x1)y - (y2-y1)x = (x2-x1)y0-(y2-y1)x0 
	double a1 = A1.x - B1.x, b1 = -(A1.y - B1.y), c1 = a1 * A1.y + b1 * A1.x;
	double a2 = A2.x - B2.x, b2 = -(A2.y - B2.y), c2 = a2 * A2.y + b2 * A2.x;
	double det = a1 * b2 - b1 * a2;
	double detY = c1 * b2 - b1 * c2;
	double detX = a1 * c2 - c1 * a2;
	if (det == 0)
		return make_pair(A1, 1); // Parallel Lines 
	Point C(detX / det, detY / det);
	return pair<Point, int>(C, 0); // Normal Intersection
}

pair<Point, int> segmentIntersection(Point A1, Point B1, Point A2, Point B2, bool seg = false) {

// Intersection between two segments or two lines // ay + bx = c --> (x2-x1)y - (y2-y1)x = (x2-x1)y0-(y2-y1)x0
	double a1 = A1.x - B1.x, b1 = -(A1.y - B1.y), c1 = a1 * A1.y + b1 * A1.x;
	double a2 = A2.x - B2.x, b2 = -(A2.y - B2.y), c2 = a2 * A2.y + b2 * A2.x;
	double det = a1 * b2 - b1 * a2;
	double detY = c1 * b2 - b1 * c2;
	double detX = a1 * c2 - c1 * a2;
	if (det == 0)
		if (between(A1, A2, B1) || between(A1, B2, B1) || between(A2, A1, B2))
			return make_pair(A1, 3); // Segments or lines are conicident
		else if (!seg && Collinear(A1, B2, A2))
			return make_pair(A1, 3); // Lines are coincident
		else
			return make_pair(A1, 1); // Lines are parallells 
	Point C(detX, detY);
	if (seg
			&& (!between(A1.scale(det), C, B1.scale(det))
					|| !between(A2.scale(det), C, B2.scale(det))))
		return make_pair(C, 2); // Lines intersect out of the 
	return pair<Point, int>(C.scale(det, true), 0); // Normal Intersection
}

bool segmentIntersect(Point A, Point B, Point C, Point D) {
	int d1 = (A - C) ^ (D - C);
	int d2 = (B - C) ^ (D - C);
	int d3 = (C - A) ^ (B - A);
	int d4 = (D - A) ^ (B - A); // if( d1 and d2 has different signs && d3 and d4 too) 
	if( (d1^d2) < 0 && (d3^d4) < 0 ) return true;
	if (between(C, A, D) || between(C, B, D) || between(A, C, B)
			|| between(A, D, B))
		return true;
	return false;
}

struct cmpCP {
	Point P;
	cmpCP(Point &_P) {
		P = _P;
	}
	int operator()(const Point &A, const Point &B) const {
		double cp = (A - P) ^ (B - P);
		if(cp != 0)return cp > 0; 
		return (A-P)*(A-P) < (B-P)*(B-P);
	}
};

vector<Point> ConvexHull(vector<Point> &V) // Convex Polygon O(NlogN)
{
	if (V.size() <= 2) return V;
	sort(V.begin(), V.end()); //sort them by X then Y
	sort(V.begin() + 1, V.end(), cmpCP(V[0]));
	vector<Point> R(V.begin(), V.begin() + 2);
	int top = 1;
	for (int i = 2; i < (int) V.size(); ++i)
	{
		while (top >= 1 && ((V[i] - R[top]) ^ (R[top - 1] - R[top])) <= 0) {
			R.pop_back(); top--;
		}
		R.push_back(V[i]); top++;
	}
	return R;
}

struct cmpXY {
	bool operator()(const Point &A, const Point &B) {
		return A.x != B.x ? A.x < B.x : A.y < B.y;
	}
};
struct cmpYX {
	bool operator()(const Point &A, const Point &B) {
		return A.y != B.y ? A.y < B.y : A.x < B.x;
	}
};
double ClosestPoint(vector<Point> &V) // LineSweep O(NlogN) 
		{
	sort(V.begin(), V.end());
	set<Point, cmpYX> S;
	double D = 1e9;
	int tail = 0;
	for (int i = 0; i < (int) V.size(); ++i) {
		while (tail < i && (V[i].x - V[tail].x) > D)
			S.erase(V[tail++]);
		set<Point, cmpYX>::iterator it1, it2;
		it1 = S.lower_bound(Point(V[i].x, V[i].y - D));
		it2 = S.upper_bound(Point(V[i].x, V[i].y + D));
		for (set<Point, cmpYX>::iterator it = it1; it != it2; ++it)
			D = min(D, ((*it) - V[i]).mag());
		S.insert(V[i]);
	}
	return D;
}



// Bugged
double DiameterOfPolygon(vector<Point> &V) // Rotating Caliper O(N) 
{
	// V --> has to be a ConvexPolygon, use Convex Hull
	int idx = 0, N = V.size();
	double dist = 1e9;
	for (int i = 0; i < N; ++i) {
		Point A = V[i], O = V[idx], NE = V[(i + 1) % N];
		while (LinePointD(A, NE, V[(idx + 1) % N]) > LinePointD(A, NE, O))
			O = V[idx = (idx + 1) % N];
		dist = min(dist, LinePointD(V[i], V[(i + 1) % N], V[idx]));
	}
	return dist;
}


double FarthestPoints(vector<Point> &V) //Rotating Caliper O(N) 
{
	// V --> has to be a ConvexPolygon, use Convex Hull
	int j = 0, N = V.size();
	double dist = 0;
	for (int i = 0; i < N; ++i) {
		while (((V[i] - V[(j + 1) % N]) * (V[i] - V[(j + 1) % N]))
				> ((V[i] - V[j]) * (V[i] - V[j])))
			j = (j + 1) % N;
		double nd = (V[i] - V[j]) * (V[i] - V[j]);
		dist = max(dist, nd);
	}
	return sqrt(dist);
}

struct Event {
	int y, x1, x2, id;
	Event(int a = 0, int b = 0, int c = 0, int ac = 0) {
		y = a, x1 = b, x2 = c, id = ac;
	}
	bool operator<(const Event &E) const {
		return (y != E.y) ? (y < E.y) : ((x1 != E.x1) ? (x1<E.x1) :
		((x2!=E.x2) ? (x2<E.x2) : (id < E.id)));
	}
};

int AreaOfRectangles(vector<pair<Point, Point> > &V) {
	vector<Event> EV;
	for (int i = 0; i < (int) V.size(); ++i) {
		int x1 = min(V[i].first.x, V[i].second.x);
		int x2 = max(V[i].first.x, V[i].second.x);
		int y1 = min(V[i].first.y, V[i].second.y);
		int y2 = max(V[i].first.y, V[i].second.y);
		EV.push_back(Event(y1, x1, x2, i));
		EV.push_back(Event(y2, x1, x2, i));
	}
	sort(EV.begin(), EV.end());
	set<Event> S;
	int p = 0;
	int Area = 0;
	for (int i = 0; i < (int) EV.size(); ++i) {
		Event A = EV[i];
		int np = A.y;
		A.y = 0;
		int length = 0;
		int pr = -1, ma = -1;
		for (set<Event>::iterator it = S.begin(); it != S.end(); ++it) {
			int x1 = it->x1, x2 = it->x2;
			if (ma < x1) {
				length += ma - pr;
				pr = x1;
				ma = x2;
			} else
				ma = max(ma, x2);
		}
		length += ma - pr;
		Area += (np - p) * (length);
		if (S.count(A) == 0)
			S.insert(A);
		else 
			S.erase(A);
		p = np;
	}
	return Area;
}


bool insideConvex(vector<Point> V, Point P, bool edge = false) {
	int poscnt = 0, N = V.size();
	for (int i = 0; i < V.size(); ++i) {
		poscnt += ((V[(i + 1) % N] - V[i]) ^ (P - V[i])) > 0;
		if (edge && between(V[i], P, V[(i + 1) % N]))
			return true;
	}
	return poscnt == V.size() || poscnt == 0;
}


bool insideConcave(vector<Point> V, Point P) {
	int N = V.size();
	double sum = 0;
	for (int i = 0; i < N; ++i) {
		int j = (i + 1) % N;
		double m1 = (V[i] - P).mag(), m2 = (V[j] - P).mag();
		double dp = ((V[i] - P) * (V[j] - P)) / (m1 * m2);
		double cp = (V[i] - P) ^ (V[j] - P);
		if (cp == 0 && between(V[i], P, V[j]))
			return true;
		if (dp > 1)
			dp = 1;
		if (dp < -1)
			dp = -1;
		double angle = cp > 0 ? acos(dp) : -acos(dp);
		sum += angle;
	}
	return fabs(sum) > M_PI;
}

Point Centroid(vector<Point> V) {
	Point R(0, 0);
	int N = V.size();
	for (int i = 0; i < N; ++i) {
		int j = (i + 1) % N;
		R.x += (V[i].x + V[j].x) * (V[i] ^ V[j]);
		R.y += (V[i].y + V[j].y) * (V[i] ^ V[j]);
	}
	R.scale(6 * AreaOfPolygon(V), true);
	return R;
}

//THIRD POINT GIVEN 2
Point ThirdPoint(Point A, Point B, double ac, double bc) {
	double ab = (A - B).mag();
	double s = (ac + bc + ab) / 2;
	double AREA = sqrt(s * (s - ac)) * sqrt((s - bc) * (s - ab));
	double h = 2 * (AREA / ab);
	double proyA = sqrt(ac * ac - h * h);
	double proyB = sqrt(bc * bc - h * h);
	Point D = A - B, C;
	if (proyA > proyB) {
		D = D.scale(-1);
		D = (D.unit()).scale(proyA);
		Point D2 = (A - B).unit();
		swap(D2.x, D2.y);
		D2.x *= -1;
		D2 = D2.scale(h);
		C = A + D + D2;
	} else {
		D = (D.unit()).scale(proyB);
		Point D2 = (B - A).unit();
		swap(D2.x, D2.y);
		D2.x *= -1;
		D2 = D2.scale(h);
		C = B + D + D2;
	}
	return C;
}

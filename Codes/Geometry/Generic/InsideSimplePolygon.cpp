// returns -1 (inside), 0 (border), 1 (outside)
template<class F1, class F2>
int pointVsPolygon(const Point<F1>& point, const Polygon<F2>& poly) {
	int n = static_cast<int>(poly.size()), windingNumber = 0;
	for (int i = 0; i < n; ++i) {
		if (point == poly[i])
			return 0;
		int j = next(i, n);
		if (poly[i].y == point.y && poly[j].y == point.y) {
			if (min(poly[i].x, poly[j].x) <= point.x && point.x <= max(poly[i].x, poly[j].x))
				return 0;
		} else {
			bool below = poly[i].y < point.y;
			if (below != (poly[j].y < point.y)) {
				auto orientation = (poly[j] - poly[i]) ^ (point - poly[i]);
				if (orientation == 0)
					return 0;
				if (below == (orientation > 0))
					windingNumber += below ? 1 : -1;
			}
		}
	}
	return windingNumber == 0 ? 1 : -1;
}

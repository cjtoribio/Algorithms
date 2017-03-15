struct InsideConvexDetection {
  vector<Point> CH;
  vector<Double> A;  // angles in increasing order [0,...
  Point center;
  // accept clockwise
  Wedge(vector<Point> _CH) : CH(_CH) {
    reverse(CH.begin(), CH.end());
    for (Point p : CH) {
      center = center + p;
    }
    center = center.scale(1.0 / CH.size());
    A.push_back(0);
    for (int i = 1; i < CH.size(); ++i) {
      A.push_back((CH[0] - center).angle(CH[i] - center));
    }
  }
  bool isIn(Point p) {
    Double ang = (CH[0] - center).angle(p - center);
    int pos = upper_bound(A.begin(), A.end(), ang) - A.begin();
    pos--;
    if (((CH[(pos + 1) % CH.size()] - CH[pos]) ^ (p - CH[pos])) < 0) {
      return false;
    }
    return true;
  }
};
vector<Point> GetHull(vector<Point> V, int upper, int allow180 = true){
  vector<Point> H;
  int sz = 0, sig = upper ? 1 : -1;
  double eps = allow180 ? 0 : 1e-9;
  for(Point p : V){
    while(H.size() >= 2 && sig * ((H[sz-1]-H[sz-2])^(p-H[sz-2])) + eps > 0){
      H.pop_back();
      sz--;
    }
    H.push_back(p);
    sz++;
  }
  return H;
}
vector<Point> ConvexHullUL(vector<Point> V, int allow180 = true){
  if(V.size() <= 1)return V;
  sort(V.begin(),V.end());
  vector<Point> U = GetHull(V,true, allow180);
  vector<Point> L = GetHull(V,false, allow180);
  for(int i = L.size() - 2; i >= 1; --i){
    U.push_back(L[i]);
  }
  return U;
}
Point thirdPointLeft(Point A,Point B, Double ac, Double bc){
	Point AB = B-A;
	Double ab = AB.mag();
	Double s = (ac+ab+bc)/2;
	Double area = sqrt(s*(s-ab))*sqrt((s-bc)*(s-ac));
	Double h = 2*area / ab;
	Double ax = sqrt(ac*ac - h*h);
	Double bx = sqrt(bc*bc - h*h);
	if(ax > bx){
		return A + AB.unit().scale(ax) + AB.unit().rotate90().scale(h);
	}else{
		return B + AB.unit().scale(-bx) + AB.unit().rotate90().scale(h);
	}
}
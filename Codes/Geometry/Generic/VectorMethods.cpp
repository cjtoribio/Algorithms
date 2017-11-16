/// validates if c is a vector inside the angle aToB inclusive, 
/// for exclusive remove the first line.
template<class F>
bool betweenAngle(Point<F> a, Point<F> b, Point<F> c){
	if( ((a^c) == 0 && (a*c) > 0) || ((b^c) == 0 && (b*c) > 0) )return 1;
	if( (a^b) > 0 )return (a^c) > 0 && (c^b) > 0;
	if( (a^b) < 0 )
		return !((b^c) > 0 && (c^a) > 0);
	return a*b == 0 ? 0 : (a^c) > 0;
}

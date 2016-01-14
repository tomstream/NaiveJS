c=3;
function testScope1(a){
	c=2;
	return c;
}
function testScope2(a){
	return c;
}
print(testScope1(1),testScope2(1));
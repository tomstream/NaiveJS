
function factor(a){
	if(a>0){
		return a*factor(a-1);
	}
	return 1;
}
print(factor(3));
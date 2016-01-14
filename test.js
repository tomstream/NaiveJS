a=0;
while(1){
	if(a>6){
		break;
	}
	a=a+1;
	if(a==2){
		continue;
	}
	k(a);
}
function k(i){
	print(1+i*(i+2));
}
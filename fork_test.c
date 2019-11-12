#include<stdio.h>

int main(){
	int a = 10;
	int b = 100;

	a = a+10;

	int pid = fork();
	//fork() 함수는 현재 구동 중인 프로세스의 복제본을 생성
	//현재 프로세스 = 부모 프로세스
	//fork()에 의해 생성된 프로세스 = 자식 프로세스
	//리턴 값(pid) = 0, if 자식 프로세스 
	//리턴 값 = 자식 프로세스의 pid값 id 부모 프로세스
	//만약 fork() 함수 실패 시 -1값을 리턴

	if(pid > 0){
		printf("부모 프로세스\n");
		a = a+10;
		printf("[Parent] a = %d, b = %d\n",a,b);
	}else if(pid == 0){
		printf("자식 프로세스\n");
		b = b*10;
		printf("[Child] a = %d, b = %d\n",a,b);	
	}else{
		printf("fork() 함수가 실패하였습니다.\n");
	}
	
	return 0;

}

#include "includes.h"

int main(){
int sock = socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in address;
char buf[1024];

address.sin_family = AF_INET;
address.sin_port = htons(PORT);

inet_pton(AF_INET,"127.0.0.1",&address.sin_addr);

std::cout<<"Trying to connect..."<<std::endl;
sleep(2);

if(connect(sock,(struct sockaddr*)&address,sizeof(address))<0){
	std::cout<<"Connect is failed!!..."<<std::endl;
	exit(1);
}

std::cout<<"Success Connect!!"<<std::endl;

while(1){
	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	FD_SET(sock,&read_fds);
	int max_sd = (sock>STDIN_FILENO) ? sock : STDIN_FILENO;
	FD_SET(STDIN_FILENO,&read_fds);

	select(max_sd+1,&read_fds,NULL,NULL,NULL);

	if(FD_ISSET(STDIN_FILENO,&read_fds)){
		int val = read(STDIN_FILENO,buf,1023);
		if(val>0){
			send(sock,buf,val,0);
		}
	}

	if(FD_ISSET(sock,&read_fds)){
		int val = read(sock,buf,1023);
		if(val<=0){
			std::cout<<"The server has shut down!!"<<std::endl;
			break;
		}
		else{
			buf[val] = '\0';
			std::cout<<buf<<std::endl;
		}
	}
	
}
close(sock);
return 0;
}

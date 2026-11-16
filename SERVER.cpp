#include "includes.h"
#define MAX_CLIENTS 3

int main(){
int sock = socket(AF_INET,SOCK_STREAM,0);
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(8080);
addr.sin_addr.s_addr = INADDR_ANY;

int addrlen = sizeof(addr);

char buf[1024];

bind(sock,(struct sockaddr*)&addr,sizeof(addr));
std::cout<<"Listening ..."<<std::endl;
listen(sock,MAX_CLIENTS);


std::vector<int> clients;
fd_set read_fds;
while(1){
FD_ZERO(&read_fds);
FD_SET(sock,&read_fds);
int max_fd = sock;

for(int sd : clients){
	FD_SET(sd,&read_fds);
	if(sd>=max_fd)max_fd = sd;
}

select(max_fd+1,&read_fds,nullptr,nullptr,nullptr);

if(FD_ISSET(sock,&read_fds)){
	int client_fd = accept(sock,(struct sockaddr*)&addr,(socklen_t*)&addrlen);
	std::cout<<"New client with FD: "<<client_fd<<std::endl;
	std::string msg = "===== Hi!!! =====\n==Welcome to our server==\n==Write Something==\n";
	send(client_fd,msg.c_str(),msg.length(),0);
	if(clients.size()<MAX_CLIENTS){
		clients.push_back(client_fd);
	}
	else{
		close(client_fd);
	}
}

for(size_t i = 0;i<clients.size();++i){
	int sd = clients[i];
	if(FD_ISSET(sd,&read_fds)){
		int rvalue = read(sd,buf,1023);
		if(rvalue <= 0){
			std::cout<<"Client disconnected with FD: "<<sd<<std::endl;
			close(sd);
			clients.erase(clients.begin()+i);
			--i;
		}
		else{
			buf[rvalue] = '\0';
			std::string msg = "[CLIENT: "+std::to_string(sd)+"] ---> "+std::string(buf);
			for(int other : clients){
				if(other != sd){
					send(other,msg.c_str(),msg.length(),0);
				}
			}
		}
	}
}

} 
return 0;
}

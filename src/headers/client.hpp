#pragma once
#include <netdb.h>
#include <string>

void except(std::string msg, int exit_code = 1);
std::string getName();
addrinfo* getAddr();
int connectServ(addrinfo* info);
void processMsg(std::string msg);
void recvLoop(int sockfd);
void sendLoop(int sockfd);
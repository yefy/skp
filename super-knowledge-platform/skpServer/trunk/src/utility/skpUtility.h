#ifndef SKPUTILITY_H
#define SKPUTILITY_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <condition_variable>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include<functional>
#include<string>
#include <thread>
#include <mutex>
#include<iostream>
#include<algorithm>
#include <deque>

#include "skp_public_utility.h"


int64 skp_get_system_time_ms();
int64 skp_get_system_time_ms_ex();
struct timeval *skp_get_system_time_tv();
int64 skp_get_system_startup_time_ms();
int64 skp_update_system_time_ms(bool isGetTime);
struct timeval skp_gettimeofday();
int64 skp_diff_time(struct timeval &tv);


std::string skp_itos(int64 number);
int64 skp_stoi(char *str, int size);

bool setNoblock(int fd);
bool setBlock(int fd);

#endif // SKPUTILITY_H

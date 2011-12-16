#ifndef SSHLIB_H_
#define SSHLIB_H_

#include <libssh2.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <pthread.h>

class Cssh_dll {
private:
	int m_sock;
	LIBSSH2_SESSION * p_m_session;
    std::string recmesg; // if return error,this is saved  the error infomation
    int stat;
    long default_wait_time_sec;
public:
	Cssh_dll(const char *target_ip,int port);
	virtual ~Cssh_dll();
	int login(const char* user, const char* password);
    int cmd(const char *p_cmd,const char *waitstr,long time_sec=default_wait_time_sec);
    const char * getmesg(void);
	int disconnect(void);
    int wait_str(const char* str,long time_sec=default_wait_time_sec);
};
#endif /* SSHLIB_H_ */

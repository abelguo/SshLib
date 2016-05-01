// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CSSH_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CSSH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include <libssh2.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#define DEFAULT_TIME 3000
class CCssh {
public:
	~CCssh(void);
	 CCssh(const char *target_ip,short port=22,long default_wait_times=DEFAULT_TIME);
	int is_connect(void);
	int Connect(const char *user_name,const char *password);
	int Create_channel(const char* pyType="vanilla");
	int cmd(const char *cmd_buf,const char *wait_str=NULL,long wait_time_msec=0);
	const char *getmesg(void)
	{
		return recv_mesg.c_str();
	}
	int wait_str(const char *str,long wait_time_msec=0);
	int disconnect(void);
	static void S_KbdCallback(const char *name, int name_len,
		const char *instruction, int instruction_len,
		int num_prompts,
		const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
		LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
		void **abstract);//imcomplete
private:
	int stat;
    long default_wait_time;
	std::string recv_mesg;
	std::string host_ip;
	short  ssh_port;
	LIBSSH2_SESSION *p_session;
	LIBSSH2_CHANNEL *p_channel;	
	int m_socket;
private:
};



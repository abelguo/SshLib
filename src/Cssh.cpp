// Cssh.cpp : Defines the entry point for the DLL application.
//
#ifdef WIN32
#include "stdafx.h"
#endif
#include "Cssh.h"

#ifdef WIN32
#ifdef _MANAGED
#pragma managed(push, off)
#endif
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
#ifdef _MANAGED
#pragma managed(pop)
#endif
#endif




CCssh::CCssh(const char *target_ip,short port,long m_default_wait_time)
{
	recv_mesg.reserve(1024);
	stat=0;
	p_channel=NULL;
	host_ip=target_ip;
	ssh_port=port;
	p_session=NULL;
	m_socket=-1;
	if(m_default_wait_time>0)
		default_wait_time=m_default_wait_time;
	libssh2_init (0);
	return;
}
int CCssh::Connect(const char *username,const char* password)
{

#ifdef WIN32

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD( 2, 0 );

	int err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 )
	{
		stat = -1;
		return -1;
	}

	if (LOBYTE( wsaData.wVersion ) != 2 ||
		HIBYTE( wsaData.wVersion ) != 0 ) 
	{
		WSACleanup();
		stat=-1;
		return stat;
	}
#endif
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in sin;
	sin.sin_addr.s_addr=inet_addr(host_ip.c_str());
	sin.sin_port=htons(ssh_port);
	sin.sin_family=AF_INET;
	int stat=connect(m_socket,(sockaddr*)(&sin),sizeof(sockaddr_in));
	if(stat)
	{
		recv_mesg="connect failed";
		return -1;
	}
	 p_session = libssh2_session_init();
	 if(p_session==NULL)
	 {
		 recv_mesg="libssh2_sesion_init failed";
		 return -1;
	 }
	 stat= libssh2_session_handshake(p_session, m_socket);
	 if (stat) 
	 {
		 recv_mesg="establishing SSH session\n";
		 return -1;
	 } 
	 int auth_pw = 0;
	 std::string fingerprint = libssh2_hostkey_hash(p_session, LIBSSH2_HOSTKEY_HASH_SHA1);
	 std::string userauthlist = libssh2_userauth_list( p_session,username,strlen(username));
	 if ( strstr( userauthlist.c_str(), "password") != NULL )
	 {
		 auth_pw |= 1;
	 }
	 if ( strstr( userauthlist.c_str(), "keyboard-interactive") != NULL )
	 {
		 auth_pw |= 2;
	 }
	 if ( strstr(userauthlist.c_str(), "publickey") != NULL)
	 {
		 auth_pw |= 4;
	 }

	 if (auth_pw&1)
	 {
		 if (libssh2_userauth_password(p_session,username,password))
		 {
			 disconnect();
			 recv_mesg="password key failed";
			 return -1;
		 }
	 }
	 else if (auth_pw&2)
	 {
		 if (libssh2_userauth_keyboard_interactive(p_session, username,S_KbdCallback))
		 {
			 disconnect();
			 recv_mesg="keyboard_interactive by public key failed";
			  return -1;
		 }
	 }
	 else if(auth_pw&4)
	 {
		 int rc;
		 while ((rc = libssh2_userauth_publickey_fromfile(p_session, username,
	        ".ssh/id_rsa.pub",
			 ".ssh/id_rsa",
			 password)) ==
			 LIBSSH2_ERROR_EAGAIN);
		 if (rc) 
		 {
			recv_mesg="revtAuthentication by public key failed";
			 return -1;
		 }

	 }
	 else 
	 {
		 disconnect();
		 return -1;
	 }
	 return 0;
}

int CCssh::Create_channel(const char *pytType)
{
	if( NULL == p_session )
        {
			recv_mesg="you must frist create session";
            return -1;
        }
        if ( !(p_channel = libssh2_channel_open_session(p_session)) )
        {
			recv_mesg="create channel failed";
            return -1;
        }

        if ( libssh2_channel_request_pty(p_channel,pytType))
        {
            libssh2_channel_free(p_channel);
            return -1;
        }
        
        if ( libssh2_channel_shell(p_channel) )
        {
            
            libssh2_channel_free(p_channel);
            return -1;
		}
	return 0;
}
int CCssh::cmd(const char *cmd_buf,const char *str,long wait_time_msec/* =default_wait_time */)
{
	if( NULL == p_channel )
	{
		return -1;
	}
	std::string send_data = std::string(std::move(cmd_buf)) + "\r\n";
	size_t len=send_data.length();
	int tmp_size=0;
	while(1)
	{
		tmp_size+=libssh2_channel_write_ex( p_channel, 0, &send_data[tmp_size],len-tmp_size);
		if(tmp_size==len)
		{
			break;
		}

	}
	return wait_str(str,wait_time_msec);
}
int CCssh::wait_str(const char*str,long wait_time_msec)
{
	recv_mesg="";
	if( NULL == p_channel )
	{
		return -1;
	}
    if(wait_time_msec==0)
        wait_time_msec=default_wait_time;

	LIBSSH2_POLLFD *fds = new LIBSSH2_POLLFD;
	fds->type = LIBSSH2_POLLFD_CHANNEL;
	fds->fd.channel = p_channel;
	fds->events = LIBSSH2_POLLFD_POLLIN;// | LIBSSH2_POLLFD_POLLOUT;

	if( wait_time_msec % 50 )
	{
		wait_time_msec +=50;
	}
	while(wait_time_msec>0)
	{
		int rc = (libssh2_poll(fds, 1, 50));
		if (rc < 1)
		{
			wait_time_msec -= 50;
			continue;
		}

		if ( fds->revents & LIBSSH2_POLLFD_POLLIN )
		{
			char buffer[64*1024] = "";
			size_t n = libssh2_channel_read( p_channel, buffer, sizeof(buffer) );
			if ( n == LIBSSH2_ERROR_EAGAIN )
			{
				//fprintf(stderr, "will read again\n");
			}
			else if (n <= 0)
			{
				return -1;
			}
			else
			{
				recv_mesg += buffer;
				if( NULL == str )
				{
					return 0;
				}
				size_t pos = recv_mesg.rfind(str);
				if( pos != std::string::npos)
				{
					return 0;
				}
			}
		}
	}
	recv_mesg= "read data timeout";
	return -1;
}
int CCssh::disconnect(void)
{
	if(!p_channel)
	{
	    libssh2_channel_free(p_channel);
	    p_channel = NULL;
	}
	if(!p_session)
	{
		libssh2_session_disconnect(p_session,"Normal Shutdown, Thank you for playing");
		libssh2_session_free(p_session);
		p_session=NULL;
	}
    if(m_socket!=-1)
	{
		close(m_socket);
		m_socket=-1;
	}
	return 0;
}
CCssh::~CCssh(void)
{
	disconnect();
	libssh2_exit();
}
void CCssh::S_KbdCallback(const char *name, int name_len,
						 const char *instruction, int instruction_len,
						 int num_prompts,
						 const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
						 LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
						 void **abstract)
{
	(void)name;
	(void)name_len;
	(void)instruction;
	(void)instruction_len;
	if (num_prompts == 1)
	{
		//responses[0].text   = strdup(password.c_str());
		//responses[0].length = (int)password.size();
	}
	(void)prompts;
	(void)abstract;
}

#include "SshLib.h"
#ifdef __APPLE__
#include <arpa/inet.h>

#endif
#ifdef WIN32

#endif

#include <iostream>

using namespace std;

void * RunExecCmd(void * data);
struct wrapperData {
	LIBSSH2_CHANNEL * channel;
	string cmd;
	void (*ptrCallback)(std::string stdout);
};

namespace nsSshLib {

SshLib::SshLib(const std::string host, const int port) throw (SshExc){
	if(libssh2_init (0) != 0)
		throw SshExc("Libssh2 initialization failed");
}

void SshLib::connectToHost(void) throw (SshExc) {
	cout << "Connect to host" << endl;
	//	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	//	struct sockaddr_in sin;
	//	sin.sin_family = AF_INET;
	//	sin.sin_port = htons(m_port);
	//	sin.sin_addr.s_addr = inet_addr(m_host.c_str());
	//	if (!connect(m_sock, (struct sockaddr*)(&sin),
	//			sizeof(struct sockaddr_in))) {
	//		throw SshExc("Failed to connect");
	//	}
	unsigned long hostaddr = inet_addr("192.168.1.2");
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(22);
	sin.sin_addr.s_addr = hostaddr;
	if (connect(m_sock, (struct sockaddr*)(&sin),
			sizeof(struct sockaddr_in)) != 0) {
		fprintf(stderr, "failed to connect!\n");
		throw SshExc("Session handshake failed");
	}
	if(!(m_session = libssh2_session_init()))
		throw SshExc("Session initialization failed");
	//libssh2_session_set_blocking(m_session, 0);
	int rc;
	rc = libssh2_session_startup(m_session, m_sock);
	if(rc) throw SshExc("Session handshake failed");
	//if(libssh2_session_handshake(m_session, m_sock))
	//	throw SshExc("Session handshake failed");

}


int SshLib::authWithPassword(const std::string user, const std::string password)
throw (SshExc) {
	this->connectToHost();
	std::string User = user;
	std::string Password = password;
	if(libssh2_userauth_password(m_session, User.c_str(), Password.c_str()))
		throw SshExc("Authentication by password failed");
	libssh2_trace(m_session, ~0 ); //ACTIVE DEBUG

}

int SshLib::authWithPubKey(const std::string privatekey,
		const std::string publickey) throw (SshExc) {

}

sshthread_t SshLib::asyncAuthWithPassword(const std::string user,
		const std::string password,
		void (*ptrCallback)(SshExc *))
throw (SshExc) {

}
//TODO delete SshExc * after ptrCallback
sshthread_t SshLib::authWithPubKey(const std::string privatekey,
		const std::string publickey,
		void (*ptrCallback)(SshExc *)) throw (SshExc) {

}

//TODO delete SshExc * after ptrCallback
std::string SshLib::execCmd(const std::string cmd) throw (SshExc) {
	try {
		LIBSSH2_CHANNEL * channel = libssh2_channel_open_session(m_session);
		if(channel == NULL)
			throw SshExc("libssh2_channel_open_session() failed");
		if(libssh2_channel_exec(channel, cmd.c_str()))
			throw SshExc("libssh2_channel_exec() failed");
		int rc;
		std::string Res;
		do
		{
			char buffer[0x4000];
			rc = libssh2_channel_read( channel, buffer, sizeof(buffer) );
			if( rc > 0 )
			{
				for( int i = 0; i < rc; ++i )
					Res += buffer[i];
				//Res += '\n';
			}
		} while( rc > 0 );

		if(libssh2_channel_close(channel)) throw SshExc("Channel close error");
		libssh2_channel_free(channel);

		channel = NULL;
		return Res;
	}
	catch (SshExc & e) {
		cout << "Exc in thread : " << e.what() << endl;
	}

}

std::string SshLib::execCmd(const std::string cmd, const std::string & stderr)
throw (SshExc) {

}

sshthread_t SshLib::asyncExecCmd(const std::string cmd,
		void (*ptrCallback)(std::string stdout, std::string stderr))
throw (SshExc) {

}

sshthread_t SshLib::asyncExecCmd(const std::string cmd,
		void (*ptrCallback)(std::string stdout))
throw (SshExc) {
	sshthread_t thread;
	wrapperData * data = new wrapperData;
	data->channel = libssh2_channel_open_session(m_session);
	if(data->channel == NULL) cerr << "Erreur lors de l'init channel" << endl;
	data->cmd = cmd;
	data->ptrCallback = ptrCallback;
	pthread_create(thread, NULL, RunExecCmd, (void *) data);
	cout << "On retourne le thread" << endl;
	return thread;
}

int SshLib::disconnect() throw (SshExc) {

}

SshLib::~SshLib() {
	// TODO Auto-generated destructor stub
}



std::string SshLib::speedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd) throw (SshExc) {

	std::string unusedStdErr;
	return speedCmd(host, port, user, password, cmd, unusedStdErr);
}

std::string SshLib::speedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd, std::string & stdErr) throw (SshExc) {
	SshLib con(host, port);
	con.authWithPassword(user, password);
	std::string res = con.execCmd(cmd, stdErr);
	return res;
}

sshthread_t SshLib::asyncSpeedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd, void (*ptrCallback)(std::string stdout,
				std::string & stderr)) throw (SshExc) {
	// TODO
	return NULL;
}
} //namespace

void * RunExecCmd(void * data) {
	wrapperData * dataRcv = (wrapperData *) data;
	cout << "On lance la commande : " << dataRcv->cmd << endl;
	LIBSSH2_CHANNEL * channel = dataRcv->channel;
	if(channel == NULL)
		cerr << "libssh2_channel_open_session() failedddd" << endl;
	if(libssh2_channel_exec(channel, dataRcv->cmd.c_str()))
		cerr << "libssh2_channel_exec() faileddddd" << endl;
	int rc;
	std::string Res;
	do
	{
		char buffer[0x4000];
		rc = libssh2_channel_read( channel, buffer, sizeof(buffer) );
		if( rc > 0 )
		{
			for( int i = 0; i < rc; ++i )
				Res += buffer[i];
			//Res += '\n';
		}
	} while( rc > 0 );

	if(libssh2_channel_close(channel));//throw SshExc("Channel close error");
	libssh2_channel_free(channel);

	channel = NULL;
	dataRcv->ptrCallback(Res);
}

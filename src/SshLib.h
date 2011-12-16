#ifndef SSHLIB_H_
#define SSHLIB_H_

#include <libssh2.h>
#include <string>

#include "SshExc.h"
#include <pthread.h>

namespace nsSshLib {

typedef pthread_t * sshthread_t;

class SshLib {
private:
	int m_sock;
	LIBSSH2_SESSION * m_session;
	std::string m_host;
	int m_port;
	void connectToHost(void) throw (SshExc);
	pthread_mutex_t verrou;
public:
	SshLib(const std::string host, const int port) throw (SshExc);

	int authWithPassword(const std::string user, const std::string password)
															throw (SshExc);

	int authWithPubKey(const std::string privatekey,
					   const std::string publickey) throw (SshExc);

	sshthread_t asyncAuthWithPassword(const std::string user,
									  const std::string password,
									  void (*ptrCallback)(SshExc *))
														throw (SshExc);
	//TODO delete SshExc * after ptrCallback
	sshthread_t authWithPubKey(const std::string privatekey,
					   	       const std::string publickey,
					   	       void (*ptrCallback)(SshExc *)) throw (SshExc);

	//TODO delete SshExc * after ptrCallback
	std::string execCmd(const std::string cmd) throw (SshExc);

	std::string execCmd(const std::string cmd, const std::string & stderr)
															throw (SshExc);

	sshthread_t asyncExecCmd(const std::string cmd,
					void (*ptrCallback)(std::string stdout, std::string stderr))
															throw (SshExc);

	sshthread_t asyncExecCmd(const std::string cmd,
			void (*ptrCallback)(std::string stdout)) throw (SshExc);


	int disconnect() throw (SshExc);


	virtual ~SshLib();

	static std::string speedCmd(const std::string host, const int port,
			const std::string user, const std::string password,
			const std::string cmd) throw (SshExc);

	static std::string speedCmd(const std::string host, const int port,
			const std::string user, const std::string password,
			const std::string cmd, std::string & stdErr) throw (SshExc);

	static sshthread_t asyncSpeedCmd(const std::string host, const int port,
			const std::string user, const std::string password,
			const std::string cmd, void (*ptrCallback)(std::string stdout,
					std::string & stderr)) throw (SshExc);


};

} /* namespace nsSshLib */
#endif /* SSHLIB_H_ */

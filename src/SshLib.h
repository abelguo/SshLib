#ifndef SSHLIB_H_
#define SSHLIB_H_

#include <libssh2.h>
#include <string>

#include "SshExc.h"

namespace nsSshLib {

typedef pthread_t * sshthread_t;

class SshLib {
private:
	int sock;
	LIBSSH2_SESSION *session;
	std::string host;
	int port;
public:
	SshLib(const std::string host, const int port) throw (SshExc);
	int authWithPassword(const std::string user, const std::string password)
																throw (SshExc);

	sshthread_t asyncConnect(void (*ptrCallback)(SshExc *)) throw (SshExc);
	//TODO delete SshExc * after ptrCallback
	int execCmd(const std::string cmd) throw (SshExc);
	sshthread_t asyncExecCmd(void (*ptrCallback)(std::string stdout,
									std::string stderr)) throw (SshExc);

	virtual ~SshLib();

	speedConnect();
};

} /* namespace nsSshLib */
#endif /* SSHLIB_H_ */

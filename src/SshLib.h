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
	int sock;
	LIBSSH2_SESSION *session;
	std::string host;
	int port;
public:
	SshLib(const std::string host, const int port) throw (SshExc);
	int authWithPassword(const std::string user, const std::string password)
															throw (SshExc &);

	int authWithPubKey(const std::string privatekey,
					   const std::string publickey) throw (SshExc &);

	sshthread_t asyncAuthWithPassword(const std::string user,
									  const std::string password,
									  void (*ptrCallback)(SshExc *))
														throw (SshExc &);
	//TODO delete SshExc * after ptrCallback
	sshthread_t authWithPubKey(const std::string privatekey,
					   	       const std::string publickey,
					   	       void (*ptrCallback)(SshExc *)) throw (SshExc &);

	//TODO delete SshExc * after ptrCallback
	std::string & execCmd(const std::string cmd) throw (SshExc);

	std::string & execCmd(const std::string cmd, const std::string & stderr)
															throw (SshExc &);

	sshthread_t asyncExecCmd(const std::string cmd,
					void (*ptrCallback)(std::string stdout, std::string stderr))
															throw (SshExc &);

	int disconnect() throw (SshExc &);


	virtual ~SshLib();
};

} /* namespace nsSshLib */
#endif /* SSHLIB_H_ */

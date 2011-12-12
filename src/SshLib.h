#ifndef SSHLIB_H_
#define SSHLIB_H_

#include <libssh2.h>
#include <string>

namespace nsSshLib {

typedef pthread_t * sshthread_t;

class SshLib {
private:
	int sock;
	LIBSSH2_SESSION *session;
	std::string host;
	int port;
public:
	SshLib(const std::string host, const int port);
	int connect();
	//sshthread_t asyncConnect(
	virtual ~SshLib();
};

} /* namespace nsSshLib */
#endif /* SSHLIB_H_ */

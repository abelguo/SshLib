#include "SshLib.h"

namespace nsSshLib {

SshLib::SshLib(const std::string host, const int port) throw (SshExc){
	// TODO Auto-generated constructor stub

}

SshLib::~SshLib() {
	// TODO Auto-generated destructor stub
}



std::string SshLib::speedCmd(const std::string host, const int port,
			const std::string user, const std::string password,
			const std::string cmd) throw (SshExc &) {

	std::string unusedStdErr;
	return speedCmd(host, port, user, password, cmd, unusedStdErr);
}

std::string SshLib::speedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd, std::string stdErr) throw (SshExc &) {

	SshLib connexion(host, port);
	connexion.authWithPassword(user, password);
	return execCmd(cmd, stdErr);
}

sshthread_t SshLib::asyncSpeedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd, void (*ptrCallback)(std::string stdout,
				std::string stderr)) throw (SshExc &) {
	// TODO
	return NULL;
}


} /* namespace nsSshLib */

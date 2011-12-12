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
	SshLib connexion(host, port);
	connexion.connect(user, password);
}

std::string SshLib::speedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd, std::string) throw (SshExc &) {

}

sshthread_t SshLib::asyncSpeedCmd(const std::string host, const int port,
		const std::string user, const std::string password,
		const std::string cmd, void (*ptrCallback)(std::string stdout,
				std::string stderr)) throw (SshExc &) {

}


} /* namespace nsSshLib */

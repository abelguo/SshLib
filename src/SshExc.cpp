#include "SshExc.h"

namespace nsSshLib {

SshExc::SshExc(std::string errorMessage)
						: std::exception(), message(errorMessage) {}

SshExc::SshExc(unsigned int errorNumber) : std::exception() {
	switch(errorNumber) {
	case 0: message = "Echec de la connexion";
		break;
	default: message = "Erreur inconnue.";
		break;
	}
}


SshExc::~SshExc() throw() {}

const char* SshExc::what() const throw() {
	return message.c_str();
}


std::string SshExc::getMessage() const {
	return message;
}

} /* namespace nsSshLib */

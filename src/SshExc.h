#ifndef SSHEXC_H_
#define SSHEXC_H_

#include <exception>
#include <string>

namespace nsSshLib {

class SshExc : public std::exception {
public:
	/** Constructeur de l'exception
	 * @param errorMessage un message d'erreur a afficher */
	SshExc(std::string errorMessage);

	/** Constructeur de l'exception
	 * @param errorNumber le numero de l'erreur concernee,
	 * le message defini pour cette erreur sera affiche. */
	SshExc(unsigned int errorNumber);

	/** Destructeur */
	virtual ~SshExc() throw();

	virtual const char* what() const throw();

	/** @return le message d'erreur */
	std::string getMessage() const;

protected:

	/** Message d'erreur a afficher lorsque l'exception est levee. */
	std::string message;
};

} /* namespace nsSshLib */
#endif /* SSHEXC_H_ */

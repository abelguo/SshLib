#include "SshLib.h"
#include "SshExc.h"
#include <pthread.h>
#include <iostream>
#include <unistd.h>
using namespace std;
using namespace nsSshLib;

void ReadRes (std::string stdout) {
	cout << "Async read : " << stdout << endl;
}

int main(int argc, char **argv) {
	try {
		SshLib con("192.168.1.2", 22);
		con.authWithPassword("jereak", "happybouh");
		con.asyncExecCmd("sleep 1; echo sleep 1", ReadRes);
		sleep(1);
		con.asyncExecCmd("sleep 2; echo sleep 2", ReadRes);
		sleep(1);
		con.asyncExecCmd("sleep 3; echo sleep 3", ReadRes);
		sleep(1);
		con.asyncExecCmd("sleep 4; echo sleep 4", ReadRes);
		sleep(1);
		cout << "On continue le programme ..." << endl;
		for( ; ;) {
			cout << "Age ? " << endl;
			int i;
			cin >> i;
			cout << "Vous avez " << i << "ans" << endl;
		}
	}

	catch(SshExc & e) {
		cout << "Error : " << e.getMessage() << endl;
	}
}


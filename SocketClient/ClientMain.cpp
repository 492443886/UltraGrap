#include <iostream>
#include <string>
#include <vector>
#include "ClientSocket.hpp"
#include "ThreadManagerAdatpter.hpp"
#include "Command.hpp"
using namespace std;



size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos + 1));
	return strs.size();
}
ThreadManagerAdapter Tmanager;

void print(string rec) {
	std::shared_ptr<PrintCommand> ee2(new PrintCommand(rec));
	auto c = std::static_pointer_cast<Command>(ee2);
	Tmanager.addTaskHelper(c);
}


int main(int argc, char *argv[]) {
	string mainIp = "127.0.0.1";
	if (argc > 1) {
		mainIp = string(argv[1]);
	}
	
	SocketClient socket;
	Tmanager.setup();

	print("Wellcome to ultraGrep!!") ;
	print("**************************************************************************");
	print("@ grep [-v] remotefolder regex .exts   @ stopserver");
	print("@ drop                                 @ connect address");
	print("**************************************************************************");
	print("Input: ");

	while (true){

		string forSend;
		getline(cin, forSend);

		std::vector<std::string> forParse;
		split(forSend, forParse, ' ');
		if (forParse.at(0) == "grep") {
			if (!socket.isConnected()) {
				print("No connection!!");
				continue;
			}

			socket.sends(forSend); 
			string rec = socket.receive();
			socket.sends("got");
			if (rec == "ver") {
				for (;;) {
					rec = socket.receive();
					socket.sends("got");
					print(rec);
					if (rec == "success")
						break;
				}
			}
			if (rec == "success") {

				string line;
				for (;;) {
					string line = socket.receive();
					socket.sends("got");
					print(line);
					if (line == "done")
						break;
				};
			}
			else {
				print(rec);
			}
		}
		else if (forParse.at(0) == "drop") {
			if (!socket.isConnected()) {
				print("No connection!!");
				continue;
			}
			
			socket.sends(forSend);
			socket.drop();
			print("Droped Connection to server !");

		}
		else if (forParse.at(0) == "connect") {
			if (socket.isConnected()) {
				print("Already connected!!");
				continue;
			}
			if (socket.connects(forParse.at(1))) {
				print("Connected to server successfully!");
			}
			else {

				print("connect fail");
			}
		}
		else if (forParse.at(0) == "stopserver") {

			if (!socket.isConnected()) {
				print("No connection!!");
				continue;
			}

			print("Sever stoped!");
			socket.sends(forSend);
			socket.drop();
		}
		else {


			print("Invalid Input!");
		}

	}
	
	Tmanager.finish();
	getchar();
};


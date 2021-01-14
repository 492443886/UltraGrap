
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <filesystem>
#include <regex>
#include <map>
#include <fstream>  
#include <memory>
#include <sstream> 
#include "SocketWraper.hpp"
#include "Command.hpp"
#include "GrepCommandVerbose.hpp"
#include "ThreadManagerAdatpter.hpp"
using namespace std;
using namespace std::filesystem;

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}


vector <path> addFolder(string folder) {
	recursive_directory_iterator dir(folder);
	recursive_directory_iterator end;

	vector <path> folders;
	folders.push_back(folder);

	for (; dir != end; ++dir)
	{
		if (is_directory(dir->status()))
			folders.push_back(dir->path());

	}
	return folders;
}


void grep(FileMagnager * fileM, string p) {

	
	ThreadManagerAdapter manager;
	manager.setup();

	vector <path> folders = addFolder(p);
	for (auto path : folders) {

		std::shared_ptr<GrepCommand> ee2(new GrepCommand);
		ee2->setFileMagnager(fileM);
		ee2->p = path.u8string();
		auto c = std::static_pointer_cast<Command>(ee2);
		manager.addTaskHelper(c);
	}

	manager.finish();
}


void grepV(FileMagnager * fileM, string p, SocketWraper * socket) {

	ThreadManagerAdapter manager;
	manager.setup();

	vector <path> folders = addFolder(p);
	for (auto path : folders) {

		std::shared_ptr<GrepCommandVerbose> ee2(new GrepCommandVerbose);
		ee2->socket = socket;
		ee2->setFileMagnager(fileM);
		ee2->p = path.u8string();

		auto c = std::static_pointer_cast<Command>(ee2);
		manager.addTaskHelper(c);
	}
	manager.finish();
}


int main(int argc, char *argv[]) {
	string mainIp = "127.0.0.1";
	if (argc > 1) {
		 mainIp = string(argv[1]);
	}


	SocketWraper socket;

	socket.setUpServer(mainIp);
	socket.connect(mainIp);

	bool isShut = false;

	while (!isShut) {

		string received = socket.receive();

		std::vector<std::string> forParse;

		split(received, forParse, ' ');
		

		if (forParse.at(0) == "grep") {

			FileMagnager fileM;
			string file;

			size_t argc = forParse.size();
			if (argc < 2) {
				cerr << "wrong arguments number" << endl;
				socket.sends("wrong arguments number");
			}
			size_t index = 1;

			string temp = forParse[index];
			if (temp == "-v") {
				fileM.verdose = true;
				index++;
			}
			else {
				fileM.verdose = false;
			}

			file = forParse[index];

			path p(file);

			if (!exists(p)) {
				cerr << "wrong folder name" << endl;
				socket.sends("wrong folder name");
			}

			if (++index > (argc - 1)) {
				cerr << "wrong arguments number" << endl;
				socket.sends("wrong arguments number");
			}

			fileM.exprRegex = forParse[index];

			if (++index == argc - 1) {
				fileM.extentionsRegex = forParse[index];
			}
			else {
				fileM.extentionsRegex = ".txt";
			}


			if (fileM.verdose) {
				socket.sends("ver");
				grepV(&fileM, file, &socket);
			}
			else
				grep(&fileM, file);

			socket.sends("success");
			socket.receive();
			
			stringstream ss = fileM.report();
			string line;

			while (ss) {

				getline(ss, line);
				//cout << line<<endl;
				socket.sends(line);
				socket.receive();
			}

			socket.sends("done");
			socket.receive();

		}
		else if (forParse.at(0) == "drop") {
			cout << "client droped." << endl;
			socket.drop();
		}
		else if (forParse.at(0) == "connect") {
			cout << "client connected." << endl;
		}
		else if (forParse.at(0) == "stopserver") {
			cout << "sever shuting down." << endl;
			socket.shutDown();
			isShut = true;

		}
		
	}
	
	getchar();

};
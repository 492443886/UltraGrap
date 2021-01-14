#include "GrepCommandVerbose.hpp"
using namespace std;
using namespace std::filesystem;
void GrepCommandVerbose::execute()
{

	{
		LOCK
			socket->sends("Scan: " + p);
		socket->receive();
	}

	auto result = fileManager->scanV(p);


	vector<File> realResult;
	for (auto r : result) {
		{
			LOCK

			socket->sends("Greping: " + r.name);
			socket->receive();
		}

		r.lines = fileManager->analyse(r.name);
		if (r.lines.size() != 0) {

			for (auto l : r.lines) {
				stringstream ss;
				ss << "Matched " << l.match << ": [" << l.number << "]" << l.content;
				{
					LOCK
					socket->sends(ss.str());
					socket->receive();
				}

			}

			realResult.push_back(r);
		}
	}

	{

		fileManager->files[p] = realResult;
	}


}
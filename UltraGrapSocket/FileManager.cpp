#include "FileManager.hpp"

using namespace std;
using namespace std::filesystem;

bool FileMagnager::IsMatch(path p) {

	cmatch match;
	regex_match(p.extension().u8string().c_str(), match, extentionsRegex);
	if (match.size() == 0)
		return false;
	return true;
}

vector<Line> FileMagnager::analyse(path p) {

	ifstream filein(p);
	vector<Line> lines;

	int number = 0;
	for (string line; getline(filein, line); )
	{
		number++;
		Line temp;

		smatch match;
		temp.content = line;
		temp.match = 0;
		while (regex_search(line, match, exprRegex)) {
			temp.match += match.size();
			line = match.suffix().str();
		}
		temp.number = number;


		if (temp.match > 0) {
			lines.push_back(temp);
		}

	}
	return lines;
}

vector<File> FileMagnager::scan(string fd) {

	directory_iterator dir(fd);
	directory_iterator end;
	vector<File> filesTemp;

	for (; dir != end; ++dir)
	{
		if (!is_directory(dir->status()))
		{
			File file;
			path p(dir->path());
			file.name = p.u8string();
			if (IsMatch(p)) {
				file.lines = analyse(p);
				if (file.lines.size() > 0)
					filesTemp.push_back(file);
			}
		}
	}
	return filesTemp;
}

std::vector<File> FileMagnager::scanV(std::string fd)
{

	directory_iterator dir(fd);
	directory_iterator end;
	vector<File> filesTemp;

	for (; dir != end; ++dir)
	{
		if (!is_directory(dir->status()))
		{
			File file;

			path p(dir->path());

			file.name = p.u8string();
			if (IsMatch(p)) {
				filesTemp.push_back(file);
			}

		}
	}
	return filesTemp;
}



stringstream FileMagnager::report()
{
	stringstream sss;
	sss << "Grep Report:" << std::endl;
	int countFile = 0;
	size_t matchNumber = 0;

	for (auto const&[key, val] : files)
	{

		for (auto f : val) {
			sss << endl;
			countFile++;

			sss << f.name << endl;
			for (auto l : f.lines) {


				sss << "[" << l.number;
				if (l.match > 1)
					sss << ":" << l.match;
				sss << "]" << l.content << endl;
				matchNumber += l.match;
			}
		}
	}
	sss << endl;
	sss << "Files with matches = " << countFile << endl;
	sss << "Total number of matches = " << matchNumber << endl;

	return sss;
}


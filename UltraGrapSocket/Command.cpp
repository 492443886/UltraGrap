#include "Command.hpp"
using namespace std;
void GrepCommand::execute() {
	auto result = fileManager->scan(p);
	{
		fileManager->files[p] = result;
	}
}
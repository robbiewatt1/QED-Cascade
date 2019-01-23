#include "FileParser.hh"

int main(int argc, char* argv[])
{
	FileParser parse = FileParser(argv[1]);

	GeneralParameters test =  parse.GetGeneral();

	return 0;
}

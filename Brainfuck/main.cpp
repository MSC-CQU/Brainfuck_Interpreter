#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include "include/declared.h"
using namespace std;

int main(int argc, char *argv[])
{
	try
	{
		ParseResult prprpr = ParseCommand(argc, argv);

		string content = ReadSource(prprpr.m_strSource);

		InterpreterBF(prprpr, content);
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return 0;
}
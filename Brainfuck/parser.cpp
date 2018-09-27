#include <iostream>
#include <string>
#include <fstream>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include "declared.h"
#include <utility>
#include "throw_if_failed.hpp"
#include <filesystem>
using namespace std;

struct BooleanPre
{
	constexpr bool operator()(bool hs) const
	{
		return hs;
	}
};

TRACE_RETURN_TYPE(bool, BooleanPre())

ParseResult ParseCommand(int argc, char *argv[])
{
	boost::program_options::options_description commands("execute brainfuck source file(*.bf) in the terminal.\nUsage");
	commands.add_options()
		("help", "Print this help message")
		("source,s", boost::program_options::value<string>(), "Specify the source file")
		("time,t", "Print overhead at the end")
		("version,v", "Print version information");

	boost::program_options::positional_options_description pod;
	pod.add("source", -1);

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(commands).positional(pod).run(), vm);
	boost::program_options::notify(vm);

	auto help_formatter = boost::format("For example:\n"
		"brainfuck -s example.bf\n"
		"brainfuck --help\n"
		"\n"
		"This brainfuck interpreter is open source under MIT license.\n"
		"Author: MSC@CQU, our offical website at <https://microsoftstudent.club>\n"
		"Email: <cqu@microsoftstudent.club>\n"
		"If you have anything to report, welcome to contact us.");

	ParseResult result;

	if (vm.size() == 0)
	{
		cout << commands << endl
			<< help_formatter << endl;
		exit(EXIT_FAILURE);
	}

	if (vm.count("version"))
	{
		cout << "Brainfuck interpreter version 1.3" << endl;
		exit(EXIT_FAILURE);
	}

	if (vm.count("help"))
	{
		cout << commands << endl
			<< help_formatter << endl;
		exit(EXIT_FAILURE);
	}

	if (vm.count("time"))
	{
		result.m_bTime = true;
	}
	else
	{
		result.m_bTime = false;
	}

	if (vm.count("source"))
	{
		result.m_strSource = vm["source"].as<string>();
	}
	else
	{
		cerr << "You must specify the brainfuck source file." << endl;
		exit(EXIT_FAILURE);
	}

	return move(result);
}

string ReadSource(const string &filename)
{
	string result;
	std::filesystem::path pat(filename);
	if (!filesystem::exists(pat))
	{
		cerr << "the file you specified is not existed." << endl;
		exit(EXIT_FAILURE);
	}
	ifstream input;
	input.open(filename);
	THROW_IF_FAILED input.fail();
	if (input.fail())
	{
		cerr << "open source code file failed." << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	while (!input.eof())
	{
		input >> line;
		result += line;
	}

	return result;
}
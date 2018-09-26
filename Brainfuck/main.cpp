#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include <exception>
#include <boost/format.hpp>
#include <limits>
using namespace std;

enum OPERATOR
{
	OP_LESS = 60,
	OP_GREATER = 62,
	OP_DOT = 46,
	OP_COMMA = 44,
	OP_LMP = 91,
	OP_RMP = 93,
	OP_PLUS = 43,
	OP_MINUS = 45
};

string ParseCommand(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	auto src = ParseCommand(argc, argv);
	
	string content;
	ifstream input(src);
	string line;
	while (!input.eof())
	{
		input >> line;
		content += line;
	}

	try
	{
		const size_t A_BIG_INTEGER = 32767;
		unsigned char memory[A_BIG_INTEGER];
		memset(memory, 0, A_BIG_INTEGER);

		const size_t MAX_UCHAR = numeric_limits<unsigned char>::max();
		const size_t MIN_UCHAR = numeric_limits<unsigned char>::min();

		int pc = 0;
		int loop_counter = 0;
		const size_t code_length = content.length();
		boost::timer::auto_cpu_timer overhead_timer;
		for (size_t i = 0; i < code_length; i++)
		{
			switch (static_cast<int>(content[i]))
			{
			case OP_LESS:
				pc--;
				if (pc < 0)
				{
					overhead_timer.stop();
					auto temp = boost::format("Cursor has been moved out of range.\n"
						"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%") % pc  % memory[0] % argv[1];
					throw exception(temp.str().data());
				}
				break;
			case OP_GREATER:
				pc++;
				if (pc >= A_BIG_INTEGER)
				{
					overhead_timer.stop();
					auto temp = boost::format("Cursor has been moved out of range.\n"
						"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%") % pc % memory[A_BIG_INTEGER - 1] % argv[1];
					throw exception(temp.str().data());
				}
				break;
			case OP_DOT:
				cout << memory[pc];
				break;
			case OP_COMMA:
			{
				string input;
				cin >> input;
				if (input.length() > 1)
				{
					overhead_timer.stop();
					auto temp = boost::format("Can not input so many characters at the same time.\n"
						"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%\n"
						"You try to input:%4%") % pc%memory[pc] % argv[1] % input;
					throw exception(temp.str().data());
				}
			}
			break;
			case OP_LMP:
				// do nothing if register is equal to 0
				// in other words, skip all of the brackets
				if (memory[pc] == 0)
				{
					loop_counter = 1;
					while (loop_counter != 0)
					{
						i++;
						if (static_cast<int>(content[i]) == OP_LMP) loop_counter++;
						if (static_cast<int>(content[i]) == OP_RMP) loop_counter--;
					}
				}
				break;
			case OP_RMP:
				// loop executes only when register does have a value
				// so when register is legal, loop starts here
				if (memory[pc] != 0)
				{
					loop_counter = 1;
					while (loop_counter != 0)
					{
						i--;
						if (static_cast<int>(content[i]) == OP_LMP) loop_counter--;
						if (static_cast<int>(content[i]) == OP_RMP) loop_counter++;
					}
				}
				break;
			case OP_PLUS:
				if (memory[pc] == MAX_UCHAR)
				{
					overhead_timer.stop();
					auto temp = boost::format("Can not save a value more than 255 in a byte.\n"
						"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%") % pc % memory[pc] % argv[1];
					throw exception(temp.str().data());
				}
				memory[pc]++;
				break;
			case OP_MINUS:
				if (memory[pc] == 0)
				{
					overhead_timer.stop();
					auto temp = boost::format("Can not save a value less than 0 in a byte.\n"
						"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%") % pc % memory[pc] % argv[1];
					throw exception(temp.str().data());
				}
				memory[pc]--;
				break;
			default:
				overhead_timer.stop();
				auto temp = boost::format("Syntax error in the source file.\n"
					"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%") % pc % memory[pc] % argv[1];
				throw exception(temp.str().data());
				break;
			}
		}
		cout << endl;
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		return EXIT_FAILURE;
	}

	return 0;
}

string ParseCommand(int argc, char *argv[])
{
	boost::program_options::options_description commands("execute brainfuck source file(*.bf) in the terminal.\nUsage");
	commands.add_options()
		("help", "Print this help message")
		("source,s", boost::program_options::value<string>(), "specify the source file")
		("time,t", "Print overhead at the end");

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

	if (vm.size() == 0)
	{
		cout << commands << endl
			<< help_formatter << endl;
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
		main_counter.resume();
	}
	else
	{
		main_counter.stop();
	}

	if (vm.count("source"))
	{
		string src = vm["source"].as<string>();
		return src;
	}
	else
	{
		cerr << "You must specify the brainfuck source file." << endl;
		exit(EXIT_FAILURE);
	}
}
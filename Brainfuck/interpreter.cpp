#include "include/declared.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <boost/timer/timer.hpp>
#include <boost/format.hpp>
using namespace std;

void InterpreterBF(const ParseResult &parse, const string &code)
{
	const size_t A_BIG_INTEGER = 32767;
	unsigned char memory[A_BIG_INTEGER];
	memset(memory, 0, A_BIG_INTEGER);

	const size_t MAX_UCHAR = numeric_limits<unsigned char>::max();
	const size_t MIN_UCHAR = numeric_limits<unsigned char>::min();

	int pc = 0;
	int loop_counter = 0;
	const size_t code_length = code.length();
	boost::timer::auto_cpu_timer overhead_timer;
	overhead_timer.stop();
	if (parse.m_bTime)
	{
		overhead_timer.resume();
	}
	for (size_t i = 0; i < code_length; i++)
	{
		switch (static_cast<int>(code[i]))
		{
		case OP_LESS:
			pc--;
			if (pc < 0)
			{
				overhead_timer.stop();
				throw InterpreterException("Cursor has been moved out of range.", pc, memory[pc], parse.m_strSource);
			}
			break;
		case OP_GREATER:
			pc++;
			if (pc >= A_BIG_INTEGER)
			{
				overhead_timer.stop();
				throw InterpreterException("Cursor has been moved out of range.", pc, memory[A_BIG_INTEGER - 1], parse.m_strSource);
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
				/*auto temp = boost::format("Can not input so many characters at the same time.\n"
					"PC:\t%1%\tREGISTER(last):\t%2%\tSOURCE:\t%3%\n"
					"You try to input:%4%") % pc % memory[pc] % parse.m_strSource % input;*/
				throw InterpreterException("Can not input so many characters at the same time.", pc, memory[pc], parse.m_strSource);
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
					if (static_cast<int>(code[i]) == OP_LMP) loop_counter++;
					if (static_cast<int>(code[i]) == OP_RMP) loop_counter--;
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
					if (static_cast<int>(code[i]) == OP_LMP) loop_counter--;
					if (static_cast<int>(code[i]) == OP_RMP) loop_counter++;
				}
			}
			break;
		case OP_PLUS:
			if (memory[pc] == MAX_UCHAR)
			{
				overhead_timer.stop();
				throw InterpreterException("Can not save a value more than 255 in a byte.", pc, memory[pc], parse.m_strSource);
			}
			memory[pc]++;
			break;
		case OP_MINUS:
			if (memory[pc] == 0)
			{
				overhead_timer.stop();
				throw InterpreterException("Can not save a value less than 0 in a byte.", pc, memory[pc], parse.m_strSource);
			}
			memory[pc]--;
			break;
		default:
			overhead_timer.stop();
			throw InterpreterException("Syntax error in the source file.", pc, memory[pc], parse.m_strSource);
			break;
		}
	}
	cout << endl;
}
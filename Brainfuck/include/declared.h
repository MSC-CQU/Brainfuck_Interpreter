#ifndef _8D7CCEC7_BEA5_4D1A_B575_1A95267AC23A_DECLARED_h_
#define _8D7CCEC7_BEA5_4D1A_B575_1A95267AC23A_DECLARED_h_

#include <string>
#include <exception>
#include <boost/format.hpp>

typedef struct tagParseResult
{
	bool m_bTime;
	std::string m_strSource;
}ParseResult;

enum OPERATOR
{
	OP_LESS = 60,		// <
	OP_GREATER = 62,	// >
	OP_DOT = 46,		// .
	OP_COMMA = 44,		// ,
	OP_LMP = 91,		// [
	OP_RMP = 93,		// ]
	OP_PLUS = 43,		// +
	OP_MINUS = 45		// -
};

ParseResult ParseCommand(int argc, char *argv[]);
std::string ReadSource(const std::string &filename);
void InterpreterBF(const ParseResult &parse, const std::string &code);

class InterpreterException : public std::exception
{
public:
	InterpreterException() noexcept
		: m_strMessage("")
	{

	}

	explicit InterpreterException(const std::string &msg, int pc, unsigned char reg, const std::string &file) noexcept
	{
		auto formatter = boost::format(msg + "\n" +
			"PC:\t%1%" + "\t"
			+ "REGISTER(last):\t%2%" + "\t"
			+ "SOURCE:\t%3%") % pc % reg % file;
		m_strMessage = formatter.str();
	}

	virtual const char * what() const
	{
		return m_strMessage.data();
	}

private:
	std::string m_strMessage;
};

#endif // declared_h__

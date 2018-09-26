#ifndef _78D9FFC0_F18A_4599_B2BB_EDA60A726983_THROW_IF_FAILED_hpp_
#define _78D9FFC0_F18A_4599_B2BB_EDA60A726983_THROW_IF_FAILED_hpp_

#ifdef _WIN32
#if _MSC_VER < 1912
#error GranDYu requires Visual Studio 2017 15.5 or later
#endif

#ifdef _DEBUG
#include <exception>
#include <string>
#include <cstdio>
#define TRACE_RETURN_TYPE(type, predicator) \
namespace GranDYu \
{ \
	void operator+(_class_for_THROW_IF_FAILED object, type result) \
	{ \
		if (predicator(result)) \
		{ \
			char buffer[512] = { '\0' }; \
			sprintf_s(buffer, 512, "THROW_IF_FAILED is triggered, FILE: %s, LINE: %d", __FILE__, __LINE__); \
			throw std::exception(buffer); \
		} \
	} \
} \

namespace GranDYu
{
	class _class_for_THROW_IF_FAILED
	{
	public:
		_class_for_THROW_IF_FAILED() {}
		_class_for_THROW_IF_FAILED(const _class_for_THROW_IF_FAILED &) {}
		~_class_for_THROW_IF_FAILED() {}
	};
}

// to use THROW_IF_FAILED, you need to call TRACE_RETURN_TYPE, outside any field
#define THROW_IF_FAILED GranDYu::_class_for_THROW_IF_FAILED() +
#else
#define TRACE_RETURN_TYPE(type, predicator) /* do nothing, only available under DEBUG */
#define THROW_IF_FAILED /* do nothing, only available under DEBUG */
#endif

#elif __linux__
#if __GNUC__ < 5 && __GNUC_MINOR__ < 4
#error GranDYu requires GCC g++ 5.4.0 or later on Linux
#endif

#elif __APPLE__
#error GranDYu has not supported Mac OS X

#else	// __sun __FreeBSD__ __NetBSD__ __OpenBSD__ __hpux __osf__ __sgi __AIX
#error GranDYu has not supported this platform
#endif

#endif // throw_if_failed_hpp__

#include "SRIParg.h"

const AlgorithmType StringToAlgorithmType(std::string e)
{
	if (e.compare("SRIP1") == 0) {
		return AlgorithmType::SRIP1;
	}
	else if (e.compare("SRIP2") == 0) {
		return AlgorithmType::SRIP2;
	}

	return AlgorithmType::SRIP1;

	//throw std::invalid_argument("String not indentifiable with an AlgorithmType");
};

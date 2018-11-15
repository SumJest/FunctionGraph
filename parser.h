#ifndef parser_H
#define parser_H

#include <string>
#include <math.h>
#include <ctype.h>
#include <QString>

#include "stack.h"


	double degrees_to_radian(double deg);

	std::string to_string(const double &f);

	int getfloatlength(float number);
	
	void replace(std::string& str, const std::string& from, const std::string& to);

	int getPriority(char op);
		
	void popFunction();
		
	bool canPop(char op);
		
    float parse(QString s);



#endif

/*
 * ForestFireOpts.cpp
 *
 *  Created on: Sep 19, 2017
 *      Author: kaess
 */

#include "ForestFireOpts.h"
using namespace po;

ForestFireOpts::ForestFireOpts(const int argc, const char ** argv) : variables_map {}, options_description("Options") {
		this->add_options()
				("help,h", "print this help message")
				("length,L",value<int>(&L)->default_value(1000), "length of side in square grid")
				("err,e",value<double>(&err)->default_value(0.05), "goal fractional error on mean.  If not specified, ForestFire will iterate a constant (n) number of times. If n is also specified, ForestFire will satisfy both. Must be positive.")
				("n,n",value<int>(&n)->default_value(5),"number of iterations to run. If err is also specified, ForestFire will satisfy both. ForestFire will always iterate at least once.");
		store(parse_command_line(argc, argv, *this), *this);
		this->notify();
}

ForestFireOpts::~ForestFireOpts() {
	// TODO Auto-generated destructor stub
}


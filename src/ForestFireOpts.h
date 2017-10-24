/*
 * ForestFireOpts.h
 *
 *  Created on: Sep 19, 2017
 *      Author: kaess
 */

#ifndef FORESTFIREOPTS_H_
#define FORESTFIREOPTS_H_

#include <vector>
#include <boost/program_options.hpp>
namespace po = boost::program_options;


class ForestFireOpts : public po::variables_map, public po::options_description {
public:
	ForestFireOpts(const int argc, const char ** argv);
	virtual ~ForestFireOpts();
	int n;
	int L;
	double err;
	std::vector<double> p_values;
};

#endif /* FORESTFIREOPTS_H_ */

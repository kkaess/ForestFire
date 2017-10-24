/*
 * SimpleStats.cpp
 *
 *  Created on: Aug 9, 2017
 *      Author: kaess
 */

#include "SimpleStats.h"
#include <cmath>

SimpleStats::SimpleStats() :sum {0},sum2 {0}, n {0} {

}

double SimpleStats::getMean() {
	mpf_class output {sum/n};
	return output.get_d();
}

double SimpleStats::getSVAR() {
	mpf_class output {(sum2-sum*sum/n)/(n-1)};
	return output.get_d();
}

double SimpleStats::getFracMeanDev() {
	return std::sqrt(getSVAR()/n.get_d())/getMean();
}

void SimpleStats::add(double value) {
	sum2+=value*value;
	sum+=value;
	n++;
}

SimpleStats::~SimpleStats() {
}


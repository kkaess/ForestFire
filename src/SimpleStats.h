/*
 * SimpleStats.h
 *
 *  Created on: Aug 9, 2017
 *      Author: kaess
 */

#ifndef SIMPLESTATS_H_
#define SIMPLESTATS_H_

#include <gmpxx.h>

class SimpleStats {
public:
	SimpleStats();
	double getMean();
	double getSVAR();
	double getFracMeanDev();
	void add(double value);
	virtual ~SimpleStats();

	long long getN() const {
		return n.get_ui();
	}

private:
	mpf_class sum;
	mpf_class sum2;
	mpz_class n;
};

#endif /* SIMPLESTATS_H_ */

/*
 * ForestFire.cpp
 *
 *  Created on: Aug 18, 2017
 *      Author: kaess
 */

#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>
#include <utility>
#include <set>
#include "SimpleStats.h"
#include "ForestFireOpts.h"

using namespace std;

int main(int argc, char ** argv) {
	auto start = chrono::system_clock::now();

	enum class TreeState
		: char {Empty = 0, Green, Black, Burning
	};
	//using ~enum class ... : char~ minimizes memory usage

	double err; //Holds the desired fractional mean error, if required
	int n; //Holds the desired number of iterations, if required
	int L; //Holds the side length of the grid
	//The next two booleans are altered by choice of commmand-line args
	bool terminate_on_err = false; //Terminate when err is reached
	bool terminate_on_n = true; //Terminate when n is reached

	//Populate command-line args
	// TODONE move this to a class, so it doesn't recompile every time
	// ---- because it adds ~6s to compile time
//	options_description desc("Options");
//	desc.add_options()("help,h", "print this help message")("length,L",
//			value<int>(&L)->default_value(1000),
//			"length of side in square grid")("err,e",
//			value<double>(&err)->default_value(0.05),
//			"goal fractional error on mean.  If not specified, ForestFire will iterate a constant (n) number of times. If n is also specified, ForestFire will satisfy both. Must be positive.")(
//			"n,n", value<int>(&n)->default_value(5),
//			"number of iterations to run. If err is also specified, ForestFire will satisfy both. ForestFire will always iterate at least once.");
//	variables_map vm;
//	store(po::parse_command_line(argc, argv, desc), vm);
//	notify(vm);
	ForestFireOpts ffo(argc, (const char**) argv);
	//If we called the program with --help, print options and quit.
	if (ffo.count("help")) {
		cout << static_cast<po::options_description>(ffo) << endl;
		return 0;
	}
//
	L = ffo.L;
	n = ffo.n;
	err = ffo.err;
	if (!ffo["err"].defaulted()) {
		if (err <= 0)
			throw logic_error("Must specify a positive value for err.");
		terminate_on_err = true;
		terminate_on_n = !ffo["n"].defaulted();
	}

	cout << "L = \t" << L << endl;

	//TreeState * forest = new TreeState[L * L];
	// Above fails for large monolithic memory allocation
	TreeState ** forest = new TreeState*[L];
	for (int i = 0; i < L; i++) {
		forest[i] = new TreeState[L];
	}

	set<pair<int, int>> fireLine;
	set<pair<int, int>> newFireLine;
	for (int i = 30; i <= 80; i += 2) {
		double p = 0.01 * i;
		SimpleStats stats;
		int total;
		do {
			total = 0;
#pragma omp parallel
			{
				unsigned seed;
#pragma omp critical (seed)
				seed = chrono::system_clock::now().time_since_epoch().count();

				default_random_engine rand { seed };
				uniform_real_distribution<double> uniform(0, 1);
//Initialize the grid - O(L^2)
#pragma omp for collapse(2)
				for (int i = 0; i < L; i++) {
					for (int j = 0; j < L; j++) {
						if (uniform(rand) < p) {
							forest[i][j] = TreeState::Green;
						} else {
							forest[i][j] = TreeState::Empty;
						}
					}
				}
			}

			for (int i = 0; i < L; i++) {
				pair<int, int> onFire { i, 0 };
				newFireLine.emplace(onFire);
			}  //Light the first row on fire O(L log(L))
			while (newFireLine.size() > 0) {
				//This runs ~L times, so everything in here, multiply complexity by O(L)
				total++; //Increment number of iterations
				fireLine.swap(newFireLine); //Move burning trees to current fireline
				newFireLine.clear(); //make next iteration's fireline empty
				//We need an indexed container for OpenMP
				//Hence temp
				vector<pair<int, int>> temp;
				//Flatten fireLine into temp - O(L)
				for (auto tree : fireLine) {
					temp.push_back(tree);
				}

#pragma omp parallel
				{
					set<pair<int, int>> localFireLine; //Create per-thread newFireLine
#pragma omp for
					for (vector<pair<int, int>>::size_type i = 0;
							i < temp.size(); i++) {
						forest[temp[i].first][temp[i].second] =
								TreeState::Black;
					} //Set all current burning trees to burnt O(L)
#pragma omp for
					for (vector<pair<int, int>>::size_type i = 0;
							i < temp.size(); i++) { //This loop is O(L)
						int x = temp[i].first;
						int y = temp[i].second;
						//These emplaces are O(log(L))
						if (x != 0 && forest[x - 1][y] == TreeState::Green)
							localFireLine.emplace(pair<int, int>(x - 1, y));
						if (y != 0 && forest[x][y - 1] == TreeState::Green)
							localFireLine.emplace(pair<int, int>(x, y - 1));
						if (x != L - 1 && forest[x + 1][y] == TreeState::Green)
							localFireLine.emplace(pair<int, int>(x + 1, y));
						if (y != L - 1 && forest[x][y + 1] == TreeState::Green)
							localFireLine.emplace(pair<int, int>(x, y + 1));
					} //Find bordering green trees and put them in the new fireLine O(L)
#pragma omp critical
					newFireLine.merge(localFireLine); //This should be <= O(L log(L))
				} //Also, since it's a critical section, idles the other cores

			}

			stats.add(total); //Track Stats

		} while ((terminate_on_n && stats.getN() < n)
				|| (terminate_on_err
						&& (stats.getN() == 1 || stats.getSVAR() == 0
								|| stats.getFracMeanDev() > err)));

		cout << p << "\t" << stats.getMean() << "\t"
				<< (stats.getN() == 1 ? 0 : stats.getSVAR()) << "\t"
				<< stats.getN() << endl;
	}

	for (int i = 0; i < L; i++) {
		delete[] forest[i];
	}
	delete[] forest;

	auto total_time = chrono::system_clock::now() - start;
	cout << "Total time in seconds:\t"
			<< chrono::duration_cast<chrono::seconds>(total_time).count()
			<< endl;

	return 0;

}


/*
 * Copyright (c) 2020 Samuel Prevost.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>
#include <limits>
#include <cstring>
#include "Simplex.h"
#include "Problem.h"

#define DEFAULT_OUTPUT_FILE "out.lp"
#define FLAG_RANDOM "-R"
#define FLAG_QUIET "-q"
#define FLAG_VERBOSE "-v"
#define FLAG_DOUBLE_VERBOSE "-vv"

using namespace std;
using namespace Simplex;

int main(int argc, char** argv) {

    srand(time(NULL));
    Problem* problem;

    int verbose_level = 0;
    if (argc == 1){
        cout << "Usage: " << argv[0] << " [file.lp or " << FLAG_RANDOM << "] ";
        cout << "[" << FLAG_QUIET << " or " << FLAG_VERBOSE << " or " << FLAG_DOUBLE_VERBOSE << "]";
        cout << endl;
        exit(EXIT_SUCCESS);
    }
    if (argc > 2) {
        if (strcmp(argv[2], FLAG_QUIET) == 0) {
            verbose_level = -1;
        } else if (strcmp(argv[2], FLAG_VERBOSE) == 0) {
            verbose_level = 1;
        } else if (strcmp(argv[2], FLAG_DOUBLE_VERBOSE) == 0) {
            verbose_level = 2;
        }
    }
    string filename = argv[1];
    if (filename == FLAG_RANDOM){
        int it = 0;
        while(true) {
            it++;
            problem = Problem::getRandomProblem();
            try {
                perform_simplex(problem, -1);
            } catch (OptimalReachedException&){
                break;
            } catch (UnboundedProblemException&){
                continue;
            }
        }
        if (verbose_level > 0){
            cout << "Took " << it << " iterations to find well bounded random problem !" << endl;
        }
    } else {
        problem = new Problem(filename);
    }
    if (verbose_level > -1) {
        cout << "Initial problem:" << endl;
        problem->print();
        cout << "base sol \t= [" << get_solution_vector(problem).transpose() << "]" << endl;
    }
    try {
        perform_simplex(problem, verbose_level);
    } catch (OptimalReachedException &e) {
        if (verbose_level > -1){
            cout << "Optimality reached = ";
        }
        cout << e.getValue() << endl;
        if (verbose_level > -1) {
            cout << "Optimal solution = " << endl << "\t";
            Problem::print_labeled_vect(get_solution_vector(problem));
        } else {
            cout << get_solution_vector(problem).transpose() << endl;
        }
    } catch (UnboundedProblemException &e) {
        cerr << e.what() << endl;
    }
    if (filename == FLAG_RANDOM && verbose_level > -1){
        char type;
        do
        {
            cout << "Would you like to save the problem to a file? [y/n]" << endl;
            cin >> type;
        }
        while( !cin.fail() && type != 'y' && type != 'n' );
        if (type == 'y'){
            problem->save_glpsol(DEFAULT_OUTPUT_FILE);
            cout << "Problem saved under " << std::__fs::filesystem::current_path().string() << "/" << DEFAULT_OUTPUT_FILE << endl;
        }
    }
    return EXIT_SUCCESS;
}

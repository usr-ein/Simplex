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

#ifndef SIMPLEXCPP_PROBLEM_H
#define SIMPLEXCPP_PROBLEM_H

#include <iostream>
#include <vector>
#include <Eigen/Dense>

using namespace std;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::VectorXi;
using Eigen::MatrixXi;

class Problem {

public:
    MatrixXd A;
    VectorXd b;
    VectorXd costs;
    VectorXi basic_vars;

    Problem(MatrixXd A, VectorXd b, VectorXd costs, VectorXi basic_vars);
    Problem(const string& filename);

    void print();
    void save_glpsol(const string& filename);

    static void print_labeled_vect(VectorXd x);

    static Problem* getRandomProblem();

};

#endif //SIMPLEXCPP_PROBLEM_H

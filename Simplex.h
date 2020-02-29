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

#ifndef SIMPLEXCPP_SIMPLEX_H
#define SIMPLEXCPP_SIMPLEX_H

#include <iostream>
#include <cmath>
#include <limits>
#include <cstring>
#include <Eigen/Dense>
#include <fstream>
#include "LinalgHelper.h"
#include "SimplexException.h"
#include "Problem.h"

using namespace std;
using namespace LinalgHelper;
using namespace SimplexException;

using Eigen::VectorXi;
using Eigen::ArrayXd;
using Eigen::VectorXd;
using Eigen::MatrixXd;
using Eigen::Matrix;

namespace Simplex {

    bool is_optimal(const VectorXd& costs, const VectorXi& basic_vars);

    int pivot_col(const VectorXd& costs, const VectorXi& basic_vars);

    int pivot_row(const MatrixXd& A, const VectorXd& b, int column);

    MatrixXd get_inverse_base_matrix(const MatrixXd& A, const VectorXi& basic_vars);

    VectorXd get_simplex_mults(const MatrixXd& inverse_base, const VectorXd& costs, const VectorXi& basic_vars);

    VectorXd get_solution_vector(Problem* problem);

    double simplex_iteration(Problem* problem, bool verbose);

    double perform_simplex(Problem* problem, int verbose_level);

}

#endif //SIMPLEXCPP_SIMPLEX_H

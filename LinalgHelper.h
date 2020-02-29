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

#ifndef SIMPLEXCPP_LINALGHELPER_H
#define SIMPLEXCPP_LINALGHELPER_H

#include <Eigen/Dense>
using Eigen::VectorXi;
using Eigen::VectorXd;
using Eigen::MatrixXd;

namespace LinalgHelper {
    MatrixXd slice_rows(const MatrixXd& to_slice, const VectorXi& indices);
    MatrixXd slice_cols(const MatrixXd& to_slice, const VectorXi& indices);
    int argmax(VectorXd v);
    int argmin(VectorXd v);
    VectorXi opposite_indices(const VectorXi& indices, int var_count);
};


#endif //SIMPLEXCPP_LINALGHELPER_H

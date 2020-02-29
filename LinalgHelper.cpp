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

#include "LinalgHelper.h"

namespace LinalgHelper {

    MatrixXd slice_rows(const MatrixXd& to_slice, const VectorXi& indices){
        MatrixXd sliced(indices.size(), to_slice.cols());
        for (int i = 0; i < indices.size(); ++i) {
            sliced.row(i) = to_slice.row(indices(i));
        }
        return sliced;
    }

    MatrixXd slice_cols(const MatrixXd& to_slice, const VectorXi& indices) {
        return slice_rows(to_slice.transpose(), indices).transpose();
    }

    int argmax(VectorXd v){
        double max_val = v(0);
        int max_idx = 0, i = 0;
        while (++i < v.size()){
            if(max_val < v(i)){
                max_val = v(i);
                max_idx = i;
            }
        }
        return max_idx;
    }

    int argmin(VectorXd v){
        return argmax(-v);
    }

    /***
     * If indices = [a1, a2, ..., an] where a1..an are natural numbers
     * in any order in the range [0, var_count], then this returns
     * [b1, b2, ..., bm] where for all i in [1,n] and j in [1,m], ai != bj
     * and b1...bm are also in the range [0, var_count].
     *
     * For instance:
     * indices = [5, 3, 1, 8] and var_count = 10
     * return = [0, 2, 4, 6, 7, 9, 10]
     * @param indices indices which not to include
     * @param var_count the range on which to operate.
     * @return the "opposit" indices in the range, see desc.
     */
    VectorXi opposite_indices(const VectorXi& indices, int var_count){
        VectorXi opposite(var_count-indices.size());
        int k = 0;
        for (int i = 0; i < var_count; ++i) {
            // Check if the value i is in indices
            bool i_in_indices = false;
            for (int j = 0; j < indices.size(); ++j) {
                if (indices(j) == i){
                    i_in_indices = true;
                    break;
                }
            }
            if (!i_in_indices)
                opposite(k++) = i;
        }
        return opposite;
/*        VectorXi opposite(var_count-indices.size());
        int k = 0; // used to go across opposite's indices
        bool in_base = false; // true when i in base, false otherwise
        for (int i = 0; i < var_count; ++i) {
            for (int j = 0; j < indices.size(); ++j) {
                if (indices(j) == i) {
                    in_base = true;
                    break;
                }
            }
            if (!in_base) {
                opposite(k++) = i;
                in_base = false;
            }
        }
        return opposite;*/
    }
}
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

#include "Simplex.h"

namespace Simplex {
    bool is_optimal(const VectorXd &costs, const VectorXi &basic_vars) {
        VectorXi non_basic_vars = opposite_indices(basic_vars, costs.size());
        VectorXd rows = slice_rows(costs, non_basic_vars);

        return rows.minCoeff() >= 0;
    }

    int pivot_col(const VectorXd &costs, const VectorXi &basic_vars) {
        VectorXi non_basic_vars = opposite_indices(basic_vars, costs.size());
        VectorXd non_basic_costs = slice_rows(costs, non_basic_vars);

        return non_basic_vars(argmin(non_basic_costs));
    }

    int pivot_row(const MatrixXd &A, const VectorXd &b, int column) {
        VectorXd ratios = b.cwiseQuotient(A.col(column));
        // Discard any value lower or eq to zero (set it to +infty so that argmin will never get it)
        for (int i = 0; i < ratios.size(); ++i) {
            if (ratios(i) <= 0)
                ratios(i) = numeric_limits<double>::infinity();
        }

        int row = argmin(ratios);
        // We shouldn't pick a row like such, if it's our only choice, then the problem is undefined
        if (std::isinf(ratios(row))) {
            throw UnboundedProblemException();
        }
        return row;
    }

    MatrixXd get_inverse_base_matrix(const MatrixXd &A, const VectorXi &basic_vars) {
        MatrixXd B = slice_cols(A, basic_vars);
        if(abs(B.determinant()) < 1e-4){
            throw UnboundedProblemException();
        }
        return B.inverse();
    }

    VectorXd get_simplex_mults(const MatrixXd &inverse_base, const VectorXd &costs, const VectorXi &basic_vars) {
        VectorXd trans(basic_vars.size());
        cout << costs.size() << endl;
        cout << basic_vars.transpose() << endl;
        for (int i = 0; i < basic_vars.size(); ++i) {
            trans(i) = costs(basic_vars(i));
        }
        return trans.transpose() * inverse_base;
    }

    VectorXd get_solution_vector(Problem *problem) {
        VectorXd X = VectorXd::Zero(problem->A.cols());
        MatrixXd inverse_base = get_inverse_base_matrix(problem->A, problem->basic_vars);
        VectorXd new_b = inverse_base * problem->b;
        for (int i = 0; i < problem->basic_vars.size(); ++i) {
            X(problem->basic_vars(i)) = new_b(i);
        }
        return X;
    }

    double simplex_iteration(Problem *problem, bool verbose) {
        const MatrixXd &A = problem->A;
        const VectorXd &b = problem->b;
        const VectorXd &costs = problem->costs;
        VectorXi &basic_vars = problem->basic_vars;

        MatrixXd inverse_base = get_inverse_base_matrix(A, basic_vars);
        VectorXd mults = get_simplex_mults(inverse_base, costs, basic_vars);

        VectorXd new_c = costs - A.transpose() * mults;
        VectorXd new_b = inverse_base * b;
        MatrixXd new_A = inverse_base * A;
        double objective_value = mults.transpose() * b;

        // AFAIK: does nothing on basic_vars but evaluates the lazy expression of all coeffs
        // IF ALL ELSE FAIL unleash this here
        //basic_vars.transpose().eval();

        if (is_optimal(new_c, basic_vars))
            throw OptimalReachedException(objective_value);


        if (verbose) {
            cout << "B-1  \t= " << endl << inverse_base << endl;
            cout << "B-1*A\t= " << endl << new_A << endl;
            cout << "mults\t= [" << mults.transpose() << "]" << endl;
            cout << "new_c\t= [" << (new_c).transpose() << "]" << endl;
            cout << "new_b\t= [" << (new_b).transpose() << "]" << endl;
        }

        int col = pivot_col(new_c, basic_vars);
        int row = pivot_row(new_A, new_b, col);
        // Update base
        basic_vars(row) = col;

        if (verbose) {
            cout << "pivot\t= (" << row << ", " << col << ")" << endl;
            cout << "base  \t= " << basic_vars.transpose() << endl;
        }

        return objective_value;

    }

    double perform_simplex(Problem *problem, int verbose_level) {
        double objective = numeric_limits<double>::infinity();
        for (int i = 0; i < 1000; ++i) {
            if (verbose_level > 0) cout << "-------------- it #" << i << " --------------" << endl;
            objective = simplex_iteration(problem, verbose_level >= 2);
            if (verbose_level > 0) cout << "obj  \t= " << objective << endl;
        }
        return objective;
    }

}
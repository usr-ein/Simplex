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

#ifndef SIMPLEXCPP_SIMPLEXEXCEPTION_H
#define SIMPLEXCPP_SIMPLEXEXCEPTION_H

#include <exception>
using namespace std;

namespace SimplexException {
    struct OptimalReachedException : public exception {
        double value;
        OptimalReachedException(double val) : value(val) {}

        const char * what () noexcept {
            return "Optimal reached";
        }
        double getValue(){
            return this->value;
        }
    };

    struct UnboundedProblemException : public exception {
        const char * what () noexcept {
            return "Problem is unbounded";
        }
    };
}

#endif //SIMPLEXCPP_SIMPLEXEXCEPTION_H

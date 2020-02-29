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

#include <fstream>
#include "Problem.h"

Problem::Problem(const MatrixXd A, const VectorXd b, const VectorXd costs, VectorXi basic_vars){
    this->A = A;
    this->b = b;
    this->costs = costs;
    this->basic_vars = basic_vars;
}
/* File parsing functions */
static VectorXd parse_vector(istringstream& is_file){
    vector<int> indices;
    vector<double> vals;
    string current, tmp;
    while(getline(is_file, current, 'x')){
        if(vals.empty()){
            vals.push_back(stod(current));
            continue;
        }
        stringstream str_stream(current);
        str_stream >> tmp;
        int index = stoi(tmp);
        indices.push_back(index);
        if(str_stream >> tmp) {
            double value = stod(tmp);
            vals.push_back(value);
        }
    }
    int max_index = indices[0];
    for(int i = 0; i<indices.size(); i++) {
        if (max_index < indices[i]) {
            max_index = indices[i];
        }
    }

    VectorXd vect = VectorXd::Zero(max_index);
    for(int i = 0; i<indices.size(); i++) {
        vect(indices[i]-1) = vals[i];
    }

    return vect;
}
static VectorXd parse_costs(ifstream& is_file){
    vector<int> indices;
    vector<double> vals;
    string objective;
    getline(is_file, objective);
    objective = objective.substr(6); // discards " obj: "
    istringstream is_objective(objective);

    return parse_vector(is_objective);
}
struct MatBias {
    MatrixXd A;
    VectorXd b;
};

static MatBias parse_constraints_and_bias(ifstream& is_file){
    string line;
    vector<VectorXd> vects;
    vector<double> b_vals;
    while (getline(is_file, line)){
        if (line.substr(0, line.find(':')).find('c') != string::npos){
            b_vals.push_back(stod(line.substr(line.find(" <= ")+4)));
            line = line.substr(line.find(':')+1, line.find(" <= ")-line.find(':'));
            istringstream is_line(line);
            vects.push_back(parse_vector(is_line));
        } else {
            break;
        }
    }
    int max_size = 0;
    for(VectorXd v: vects){
        if (v.size() > max_size){
            max_size = v.size();
        }
    }
    MatrixXd A = MatrixXd::Zero(vects.size(), max_size);
    for (int i = 0; i < A.rows(); ++i) {
        A.row(i).head(vects[i].size()) = vects[i].col(0);
    }
    VectorXd b(b_vals.size());
    for (int j = 0; j < b_vals.size(); ++j) {
        b(j) = b_vals[j];
    }
    return {A, b};
}

Problem::Problem(const string& filename){
    /* SHOULD BE ABLE TO PARSE save_glpsol() OUTPUT !!! */
    std::ifstream is_file(filename);
    string line;
    VectorXd partial_costs;
    while(getline(is_file, line)){
        if(line == "Minimize"){
            // Parses something like
            // " obj: -12.5162 x1 +40.2776 x2 -53.5679 x3 +84.8146 x4 +79.3232 x5"
            partial_costs = parse_costs(is_file);
        } else if(line == "Subject To"){
            MatBias mb = parse_constraints_and_bias(is_file);
            this->A = mb.A;
            this->b = mb.b;
        }
    }
    this->costs = VectorXd::Zero(A.cols());
    this->costs.head(partial_costs.size()) = partial_costs;

    this->basic_vars = VectorXi (this->A.rows());
    for (int i = 0; i < this->A.rows(); ++i) {
        this->basic_vars(i) = A.cols() - A.rows() + i;
    }
}

static string get_timestamp(){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string str(buffer);
    return str;
}

void Problem::save_glpsol(const string& filename){
    /* SHOULD REMAIN PARSEABLE BY THE CONSTRUCTOR !!! */
    ofstream outStream(filename);
    outStream << "\\Generated using Simplex @ " << get_timestamp() << endl << endl;
    outStream << "Minimize" << endl << " obj: ";
    for (int k = 0; k < costs.size(); ++k) {
        if (costs(k) != 0) {
            outStream << ((costs(k) > 0) ? "+" : "") << costs(k) << " x" << k + 1 << " ";
        }
    }
    outStream << endl;
    outStream << "Subject To" << endl;
    for (int i = 0; i < A.rows(); ++i) {
        outStream << " c" << i+1 << ": ";
        for (int j = 0; j < A.cols(); ++j) {
            if (A(i,j) != 0){
                outStream << ((A(i, j) > 0) ? "+" : "") << A(i, j) << " x" << j + 1 << " ";
            }
        }
        outStream << "<= " << b(i) << endl;
    }
    outStream << "Bounds" << endl;
    for (int k = 0; k < costs.size(); ++k) {
        outStream << " x" << k + 1 << " >= 0";
    }
    outStream << endl << endl << "End";
}

void Problem::print(){
    cout << "A\t\t= " << endl << this->A << endl;
    cout << "b\t\t= [" << this->b.transpose() << "]" << endl;
    cout << "costs\t\t= [" << this->costs.transpose() << "]" << endl;
    cout << "basis\t\t= [" << this->basic_vars.transpose() << "]" << endl;
}

void Problem::print_labeled_vect(VectorXd x){
    for(int i = 1; i < x.size() + 1; i++){
        cout << "x" << i << "\t";
    }
    cout << endl << "\t";
    for(int i = 0; i < x.size(); i++){
        cout << x(i) << "\t";
    }
    cout << endl;
}

Problem* Problem::getRandomProblem(){
    bool round = false;
    // For scaling too small (< 50), GLPSol will indicate that the problem is unsolvable
    // while it's still solvable !
    int scale = (rand()%500) - 25;
    if (abs(scale) < 8) scale = 8;
    int height = 1 + rand()%15;
    int width = 1 + rand()%17;

    MatrixXd A = MatrixXd::Random(height, width+height) * scale;
    A.block(0, width, height, height) = MatrixXd::Identity(height, height);
    VectorXd b = VectorXd::Random(height) * scale;
    VectorXd costs = VectorXd::Random(width+height) * scale;
    costs.tail(height) = VectorXd::Zero(height);
    VectorXi basic_vars(height);
    for (int i = 0; i < height; ++i) {
        basic_vars(i) = i + width;
    }

    if (round){
        A = A.array().round();
        b = b.array().round();
        costs = costs.array().round();
    }

    auto* problem = new Problem(A, b, costs, basic_vars);
    return problem;
}
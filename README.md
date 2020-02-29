# Simplex Algorithm

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/e/ef/Simplex-description-en.svg/986px-Simplex-description-en.svg.png" alt="simplex illustration" width="500">


This basic implementation of the Simplex algorithm is designed to be reused transparently in future projects and hence be as robust as possible. 

## Basics
In a wide variety of cases, it's possible to convert a problem into a linear programming one, i.e. of the following form:
```
    minimize z = c^T.x
    such that
    Ax = b
    x >= 0
```
With a real valued matrix `A`, two real values vectors `b` and `c`, and a vector of real variables `x` which we're trying to optimise for. `z` is called the objective value and we want to minimize it as much as we can given the constraints.

We also enforce that the components of `x` are all positive or null.

## Interchange format
Crucially, this implementation supports the `.lp` files format required by GLPK's GLPSolve linear programming solver. The parser isn't as robust as GLPK's one (doesn't accept spaces at some places where GLPK does...) mainly due to the hassle of coding a robust configuration file parser in C++. 
Hopefully I'll get around doing it one day, but this trick is mostly for easy debugging and testing.

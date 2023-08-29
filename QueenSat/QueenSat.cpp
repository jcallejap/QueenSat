#include <minisat/core/Solver.h>
#include <iostream>
#include <chrono>

using LitVector = Minisat::vec<Minisat::Lit>;

void atMostOneLiteralIsTrue(Minisat::Solver& solver, LitVector const& literals)
{
  for (int i = 0; i < literals.size(); ++i) {
    for (int j = i + 1; j < literals.size(); ++j) {
      solver.addClause(~literals[i], ~literals[j]);
    }
  }
}

void atLeastOneLiteralIsTrue(Minisat::Solver& solver, LitVector const& literals)
{
  solver.addClause(literals);
}

void exactlyOneLiteralIsTrue(Minisat::Solver& solver, LitVector const& literals)
{
  atLeastOneLiteralIsTrue(solver, literals);
  atMostOneLiteralIsTrue(solver, literals);
}

int toVar(int i, int j, int table_size)
{
  return i + j * table_size;
}

void addHorizontalEquations(Minisat::Solver& solver, int table_size)
{
  for (int i = 0; i < table_size; ++i) {
    LitVector literals;
    for (int j = 0; j < table_size; ++j) {
      literals.push(Minisat::mkLit(toVar(i, j, table_size)));
    }
    exactlyOneLiteralIsTrue(solver, literals);
  }
}

void addVerticalEquations(Minisat::Solver& solver, int table_size)
{
  for (int i = 0; i < table_size; ++i) {
    LitVector literals;
    for (int j = 0; j < table_size; ++j) {
      literals.push(Minisat::mkLit(toVar(j, i, table_size)));
    }
    exactlyOneLiteralIsTrue(solver, literals);
  }
}

void addDiagonalEquations(Minisat::Solver& solver, int table_size)
{
  for (int i = -table_size; i < 2 * table_size; ++i) {
    for (int signus = -1; signus <= 1; ++signus) {
      LitVector literals;
      if (signus == 0) { continue; }
      for (int j = 0; j < table_size; ++j) {
        int x = i + signus * j;
        if (x >= 0 && x < table_size) {
          literals.push(Minisat::mkLit(toVar(x, j, table_size)));
        }
      }
      if (literals.size() > 1) {
        atMostOneLiteralIsTrue(solver, literals);
      }
    }
  }
}

void printSolution(const Minisat::Solver& solver, int table_size)
{
  using Minisat::lbool;

  for (int i = 0; i < table_size; ++i) {
    for (int j = 0; j < table_size; ++j) {
      if (solver.modelValue(toVar(i, j, table_size)) == l_True) {
        std::cout << "x";
      }
      else { std::cout << "-"; }
    }
    std::cout << '\n';
  }
}

void solveFor(int table_size, bool print_results)
{
  using Minisat::mkLit;
  using Minisat::lbool;

  // Init solver variables
  Minisat::Solver solver;
  for (int i = 0; i < table_size * table_size; ++i) {
    solver.newVar();
  }

  // Add basic equations
  addHorizontalEquations(solver, table_size);
  addVerticalEquations(solver, table_size);
  addDiagonalEquations(solver, table_size);

  // Print solution
  const auto has_solution = solver.solve();
  if (has_solution && print_results) {
    printSolution(solver, table_size);
  }
}

int main(int argc, char* argv[])
{
  // Program arguments
  const int begin_table_size = (argc > 1) ? atoi(argv[1]) : 5;
  const int end_table_size = (argc > 2) ? atoi(argv[2]) : (begin_table_size + 1);
  const bool print_results = (argc > 3) ? atoi(argv[3]) > 0: true;
  std::cout << std::format("{0} starting at {1} and ending at {2}", (print_results ? "Calculate times" : "Print tables"), begin_table_size, end_table_size) << std::endl;

  // Operations
  for (int i = begin_table_size; i < end_table_size; ++i) {
    const auto start = std::chrono::steady_clock::now();
    solveFor(i, print_results);
    const auto end = std::chrono::steady_clock::now();
    std::cout << std::format("Solved a {0}x{0} board in {1} milliseconds", i, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << std::endl;
  }

  return 0;
}

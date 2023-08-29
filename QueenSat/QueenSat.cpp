#include <minisat/core/Solver.h>
#include <iostream>
#include <chrono>

using LitVector = Minisat::vec<Minisat::Lit>;

/*
 * Converts a (row, col) position of the board to a variable index.
 */
int toVar(int row, int col, int board_size)
{
  return row + col * board_size;
}

/*
 * At most one literal is true.
 * For two literals, the equation is: !a | !b = true
 * This equation assures that both literal are not true at the same time.
 * We can add this equation for each pair of literals of the list.
 */
void atMostOneLiteralIsTrue(Minisat::Solver& solver, LitVector const& literals)
{
  for (int i = 0; i < literals.size(); ++i) {
    for (int j = i + 1; j < literals.size(); ++j) {
      solver.addClause(~literals[i], ~literals[j]);
    }
  }
}

/*
 * At least one literal is true.
 * The equation is: v1 | v2 | v3 | ... | vn = true
 * So we only have to add every variable to one equation.
 */
void atLeastOneLiteralIsTrue(Minisat::Solver& solver, LitVector const& literals)
{
  solver.addClause(literals);
}

/*
 * Exactly one literal is true.
 */
void exactlyOneLiteralIsTrue(Minisat::Solver& solver, LitVector const& literals)
{
  atLeastOneLiteralIsTrue(solver, literals);
  atMostOneLiteralIsTrue(solver, literals);
}

/*
 * Only one queen on each row.
 */
void addHorizontalEquations(Minisat::Solver& solver, int board_size)
{
  for (int row = 0; row < board_size; ++row) {
    LitVector literals;
    for (int col = 0; col < board_size; ++col) {
      literals.push(Minisat::mkLit(toVar(row, col, board_size)));
    }
    exactlyOneLiteralIsTrue(solver, literals);
  }
}

/*
 * Only one queen on each column.
 */
void addVerticalEquations(Minisat::Solver& solver, int board_size)
{
  for (int col = 0; col < board_size; ++col) {
    LitVector literals;
    for (int row = 0; row < board_size; ++row) {
      literals.push(Minisat::mkLit(toVar(row, col, board_size)));
    }
    exactlyOneLiteralIsTrue(solver, literals);
  }
}

/*
 * Only one queen on each diagonal.
 */
void addDiagonalEquations(Minisat::Solver& solver, int board_size)
{
  for (int i = -board_size; i < 2 * board_size; ++i) {
    for (int signus = -1; signus <= 1; ++signus) {
      LitVector literals;
      if (signus == 0) { continue; }
      for (int j = 0; j < board_size; ++j) {
        int x = i + signus * j;
        if (x >= 0 && x < board_size) {
          literals.push(Minisat::mkLit(toVar(x, j, board_size)));
        }
      }
      if (literals.size() > 1) {
        atMostOneLiteralIsTrue(solver, literals);
      }
    }
  }
}

/*
 * Print the board using 'x' for queens and '-' for empty spaces.
 */
void printSolution(const Minisat::Solver& solver, int board_size)
{
  using Minisat::lbool;

  for (int i = 0; i < board_size; ++i) {
    for (int j = 0; j < board_size; ++j) {
      if (solver.modelValue(toVar(i, j, board_size)) == l_True) {
        std::cout << "x";
      }
      else { std::cout << "-"; }
    }
    std::cout << '\n';
  }
}

/*
 * Calculat the solution.
 */
void solveFor(int board_size, bool print_results)
{
  using Minisat::mkLit;
  using Minisat::lbool;

  // Init solver variables
  Minisat::Solver solver;
  for (int i = 0; i < board_size * board_size; ++i) {
    solver.newVar();
  }

  // Add basic equations
  addHorizontalEquations(solver, board_size);
  addVerticalEquations(solver, board_size);
  addDiagonalEquations(solver, board_size);

  // Print solution
  const auto has_solution = solver.solve();
  if (has_solution && print_results) {
    printSolution(solver, board_size);
  }
}

int main(int argc, char* argv[])
{
  // Program arguments
  const int begin_board_size = (argc > 1) ? atoi(argv[1]) : 5;
  const int end_board_size = (argc > 2) ? atoi(argv[2]) : (begin_board_size + 1);
  const bool print_results = (argc > 3) ? atoi(argv[3]) > 0: true;
  std::cout << std::format("{0} starting at {1} and ending at {2}", (print_results ? "Calculate times" : "Print boards"), begin_board_size, end_board_size) << std::endl;

  // Operations
  for (int i = begin_board_size; i < end_board_size; ++i) {
    const auto start = std::chrono::steady_clock::now();
    solveFor(i, print_results);
    const auto end = std::chrono::steady_clock::now();
    std::cout << std::format("Solved a {0}x{0} board in {1} milliseconds", i, std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) << std::endl;
  }

  return 0;
}

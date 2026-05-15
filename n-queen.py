def solve_n_queens(n):
    # This will store all valid board configurations
    solutions = []
    
    # The actual board (0 = empty, 1 = Queen)
    board = [[0] * n for _ in range(n)]

    # --- Branch & Bound Constraint Arrays ---
    # These arrays track which lines are currently under attack.
    # If a column or diagonal is True, we know instantly we cannot place a queen there.
    
    cols = [False] * n
    # There are 2*N - 1 major diagonals (top-left to bottom-right)
    # Formula to get a unique index for a major diagonal: row - col + (n - 1)
    major_diagonals = [False] * (2 * n - 1)
    
    # There are 2*N - 1 minor diagonals (top-right to bottom-left)
    # Formula to get a unique index for a minor diagonal: row + col
    minor_diagonals = [False] * (2 * n - 1)

    # --- The Backtracking Algorithm ---
    def backtrack(row):
        # Base Case: If we successfully placed a queen in every row, we found a solution!
        if row == n:
            # Format the board for pretty printing
            formatted_solution = []
            for r in range(n):
                row_string = " ".join("Q" if board[r][c] else "." for c in range(n))
                formatted_solution.append(row_string)
            solutions.append(formatted_solution)
            return

        # Try placing a queen in each column of the current row
        for col in range(n):
            major_idx = row - col + (n - 1)
            minor_idx = row + col

            # Check Constraints: Is this spot under attack?
            if cols[col] or major_diagonals[major_idx] or minor_diagonals[minor_idx]:
                continue # Prune this branch, move to the next column
            
            # --- Branching (Make a Move) ---
            board[row][col] = 1 # Place Queen
            cols[col] = True
            major_diagonals[major_idx] = True
            minor_diagonals[minor_idx] = True

            # Recurse to the next row
            backtrack(row + 1)

            # --- Backtracking (Undo the Move) ---
            # If the recursion didn't find a solution, remove the queen and try the next spot
            board[row][col] = 0
            cols[col] = False
            major_diagonals[major_idx] = False
            minor_diagonals[minor_idx] = False

    # Start the backtracking process from row 0
    backtrack(0)
    return solutions

# --- User Input and Execution ---
def main():
    print("--- N-Queens Solver (CSP) ---")
    while True:
        try:
            user_input = input("Enter the number of Queens (N), or 'q' to quit: ").strip()
            if user_input.lower() == 'q':
                break
                
            n = int(user_input)
            if n <= 0:
                print("Please enter a positive integer greater than 0.\n")
                continue
            if n in [2, 3]:
                print(f"Fun fact: No solution exists for N={n}!\n")
                continue

            print(f"\nCalculating solutions for {n}-Queens...")
            solutions = solve_n_queens(n)

            print(f"Total valid configurations found: {len(solutions)}")
            
            if solutions:
                print("\nShowing the first valid solution:")
                print("-" * (n * 2))
                for row in solutions[0]:
                    print(row)
                print("-" * (n * 2) + "\n")
                
        except ValueError:
            print("Invalid input. Please enter a whole number.\n")

if __name__ == '__main__':
    main()
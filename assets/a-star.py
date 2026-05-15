import heapq

def a_star_simple(maze, start, end):
    rows = len(maze)
    cols = len(maze[0]) if rows > 0 else 0
    directions = [(0, -1), (0, 1), (-1, 0), (1, 0)]

    def h(pos):
        return abs(pos[0] - end[0]) + abs(pos[1] - end[1])

    open_list = [(h(start), start)] 
    g_scores = {start: 0}
    came_from = {}
    closed_set = set()

    while open_list:
        current_f, current = heapq.heappop(open_list)

        if current == end:
            path = []
            while current in came_from:
                path.append(current)
                current = came_from[current]
            path.append(start)
            return path[::-1]

        if current in closed_set: continue
        closed_set.add(current)

        for move in directions:
            neighbor = (current[0] + move[0], current[1] + move[1])
            nr, nc = neighbor

            if not (0 <= nr < rows and 0 <= nc < cols):
                continue
            if maze[nr][nc] != 0:
                continue
            if neighbor in closed_set:
                continue

            tentative_g = g_scores[current] + 1

            if neighbor not in g_scores or tentative_g < g_scores[neighbor]:
                g_scores[neighbor] = tentative_g
                f = tentative_g + h(neighbor)
                came_from[neighbor] = current
                heapq.heappush(open_list, (f, neighbor))

    return None

def print_maze_result(maze, path, start, end):
    if not path:
        print("\n❌ No path found.")
        return
        
    maze_copy = [row[:] for row in maze]
    
    for r, c in path:
        if maze_copy[r][c] == 0:
            maze_copy[r][c] = '*'
            
    maze_copy[start[0]][start[1]] = 'S'
    maze_copy[end[0]][end[1]] = 'E'
    
    for row in maze_copy:
        print(" ".join(str(cell) for cell in row))

# --- NEW: User Input Validation Function ---
def get_valid_coordinate(prompt, maze):
    rows = len(maze)
    cols = len(maze[0])
    
    while True:
        user_input = input(prompt).strip()
        try:
            # Try to split the input into exactly two integers
            r_str, c_str = user_input.split()
            r, c = int(r_str), int(c_str)
            
            # 1. Check if it's within the grid boundaries
            if not (0 <= r < rows and 0 <= c < cols):
                print(f"  Error: Out of bounds! Row must be 0-{rows-1}, Col must be 0-{cols-1}.")
                continue
                
            # 2. Check if the chosen spot is a wall
            if maze[r][c] != 0:
                print("  Error: That spot is a wall (1). You must pick an open path (0).")
                continue
                
            # If it passes all checks, return the tuple
            return (r, c)
            
        except ValueError:
            print("  Error: Invalid format. Please enter two numbers separated by a space (e.g., '0 0').")

if __name__ == '__main__':
    maze = [
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ]

    print("--- A* Grid Search ---")
    print("Initial Maze Map:")
    for i, row in enumerate(maze):
        print(f"Row {i}:", " ".join(str(cell) for cell in row))
    print()

    # Get validated input from the user
    start_point = get_valid_coordinate("Enter START coordinate (row col) [e.g. '0 0']: ", maze)
    end_point = get_valid_coordinate("Enter END coordinate (row col) [e.g. '9 9']: ", maze)

    print(f"\nFinding path from {start_point} to {end_point}...")
    path = a_star_simple(maze, start_point, end_point)
    
    print("\nSolved Maze (S: Start, E: End, *: Path):")
    print_maze_result(maze, path, start_point, end_point)
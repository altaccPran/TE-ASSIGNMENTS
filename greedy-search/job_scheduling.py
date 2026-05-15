def print_job_scheduling(jobs, max_deadline):
    # Sort jobs in descending order of profit (The Greedy Choice)
    # x[2] represents the profit in the tuple (id, deadline, profit)
    jobs.sort(key=lambda x: x[2], reverse=True)
    
    # Initialize an array to keep track of free time slots
    slots = [False] * max_deadline
    # Array to store the result sequence of jobs
    result = ['-1'] * max_deadline
    
    total_profit = 0

    # Iterate through all given jobs
    for i in range(len(jobs)):
        # Find a free slot for this job (starting from the last possible slot)
        # We do job[1] - 1 because array indices start at 0
        for j in range(min(max_deadline - 1, jobs[i][1] - 1), -1, -1):
            if not slots[j]:
                # Free slot found! Assign the job to this slot
                result[j] = jobs[i][0]
                slots[j] = True
                total_profit += jobs[i][2]
                break

    return result, total_profit

if __name__ == '__main__':
    print("--- Job Scheduling Problem ---")
    # Format: (Job_ID, Deadline, Profit)
    jobs = [
        ('Job A', 2, 100),
        ('Job B', 1, 19),
        ('Job C', 2, 27),
        ('Job D', 1, 25),
        ('Job E', 3, 15)
    ]
    max_deadline = 3
    
    print("Available Jobs (ID, Deadline, Profit):", jobs)
    scheduled_jobs, profit = print_job_scheduling(jobs, max_deadline)
    print(f"Scheduled Jobs: {scheduled_jobs}")
    print(f"Total Profit: {profit}")
def selection_sort(arr):
    n = len(arr)
    
    # Traverse through all array elements
    for i in range(n):
        # Find the minimum element in remaining unsorted array
        min_idx = i
        for j in range(i + 1, n):
            if arr[j] < arr[min_idx]:
                min_idx = j
                
        # Swap the found minimum element with the first element of the unsorted part
        arr[i], arr[min_idx] = arr[min_idx], arr[i]
        
    return arr

if __name__ == '__main__':
    print("--- Selection Sort ---")
    data = [64, 25, 12, 22, 11]
    print(f"Original Array: {data}")
    sorted_data = selection_sort(data)
    print(f"Sorted Array:   {sorted_data}")
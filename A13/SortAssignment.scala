// 1. SCALA QUICKSORT IMPLEMENTATION
def quickSort(arr: Array[Int]): Array[Int] = {
  if (arr.length <= 1) {
    arr
  } else {
    // Choose the middle element as the pivot
    val pivot = arr(arr.length / 2)
    // Recursively sort elements smaller, equal, and larger than the pivot
    Array.concat(
      quickSort(arr.filter(_ < pivot)),
      arr.filter(_ == pivot),
      quickSort(arr.filter(_ > pivot))
    )
  }
}

val data = Array(64, 34, 25, 12, 22, 11, 90)

println("\n========================================")
println("--- 1. SCALA QUICKSORT ALGORITHM ---")
println("Original Array: " + data.mkString(", "))
val sortedData = quickSort(data)
println("Sorted Array:   " + sortedData.mkString(", "))
println("========================================\n")


// 2. APACHE SPARK FRAMEWORK IMPLEMENTATION
println("========================================")
println("--- 2. APACHE SPARK BIG DATA SORTING ---")

// Convert the local array into a Spark Resilient Distributed Dataset (RDD)
// Note: 'sc' (SparkContext) is automatically provided by the spark-shell
val rdd = sc.parallelize(data)

// Use Spark's built-in distributed sorting mechanism
val sparkSorted = rdd.sortBy(x => x, ascending = true)

println("Spark RDD Original: " + rdd.collect().mkString(", "))
println("Spark RDD Sorted:   " + sparkSorted.collect().mkString(", "))
println("========================================\n")

// Exit the shell once execution is complete
sys.exit(0)

# Apache Spark: Distributed Sorting in Scala

This repository contains a Scala-based implementation of the Quick Sort algorithm. It demonstrates both standard local execution and distributed Big Data sorting using Resilient Distributed Datasets (RDDs) within the Apache Spark framework.

## Prerequisites
To run this application, ensure your Ubuntu environment has the following installed and configured:
* **Java 8** (`JAVA_HOME` configured).
* **Scala** (Installed via Ubuntu package manager).
* **Apache Spark** (v3.5.x with Hadoop support) extracted and `$SPARK_HOME` added to your system path.

## Execution Steps

Unlike standard Java MapReduce jobs, this Scala script does not need to be compiled into a `.jar` file. It leverages Spark's interactive shell for immediate execution and evaluation.

### 1. Ensure the File Exists
Verify that the `SortAssignment.scala` file is present in your current working directory.

### 2. Run with Spark Shell
Execute the script by passing it into the Apache Spark shell. Spark will automatically initialize a `SparkContext` (`sc`), compile the Scala code on the fly, and execute the sorting algorithms.

Run the following command in your terminal:
```bash
spark-shell -i SortAssignment.scala
```
Note: As the Spark engine initializes, it will output several internal connection and warning logs. This is standard behavior for Spark's local standalone mode.

# Hadoop MapReduce: Word Count Application

This repository contains a Java-based MapReduce program to count the number of occurrences of each word in a given dataset. It is designed to run on a local-standalone or pseudo-distributed Hadoop cluster on Ubuntu.

## Prerequisites
Before running this application, ensure your Ubuntu environment has the following configured:
* **Java 8** installed and `JAVA_HOME` set.
* **Hadoop (v3.x)** installed, formatted, and running (NameNode, DataNode, ResourceManager, NodeManager).
* `$HADOOP_CLASSPATH` environment variable exported.

## Execution Steps

Follow these instructions in your Ubuntu terminal to compile, package, and execute the MapReduce job.

### 1. Compile the MapReduce Code
Create a directory for the compiled classes and compile the Java source file using the Hadoop classpath:

```bash
# Create directory for compiled classes
mkdir -p count_classes

# Compile the Java source file
javac -classpath ${HADOOP_CLASSPATH} -d count_classes Count.java
```
### 2. Package into a JAR

Archive the compiled classes into a JAR file so it can be executed by the Hadoop framework:
```bash
jar -cvf count.jar -C count_classes/ .
```
### 3. Prepare the HDFS Input

Hadoop processes data stored in the Hadoop Distributed File System (HDFS). Create an input directory and load your test data:
```bash
# Create an input directory in HDFS
hdfs dfs -mkdir -p /input

# Create a local test text file
echo "hello hadoop hello world this is an assignment for hadoop" > test.txt

# Upload the test file to HDFS
hdfs dfs -put test.txt /input/
```
### 4. Run the MapReduce Job

Execute the JAR file using Hadoop, specifying the main class (demo.Count), the input directory, and the output directory:
```bash
hadoop jar count.jar demo.Count /input /output
```
### 5. View the Output

Once the MapReduce job completes successfully, read the output directly from HDFS:
```bash
hdfs dfs -cat /output/part-r-00000
```

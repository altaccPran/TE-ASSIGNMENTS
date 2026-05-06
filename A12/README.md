# Hadoop MapReduce: Weather Data Analysis

This repository contains a MapReduce application that processes a weather dataset to calculate advanced metrics. For each city in the dataset, it calculates the **Average, Maximum, and Minimum Temperature**, as well as the **Average Wind Speed and Average Humidity**. 

It is configured to run on a local-standalone or pseudo-distributed Hadoop cluster on Ubuntu.

## Prerequisites
Before executing this job, ensure your environment is set up:
* **Java 8** is installed and `JAVA_HOME` is configured.
* **Hadoop (v3.x)** is installed, the HDFS is formatted, and all daemons are running (`start-all.sh` or `start-dfs.sh` & `start-yarn.sh`).
* The `$HADOOP_CLASSPATH` environment variable is exported.

## Execution Steps

Follow these terminal instructions on your Ubuntu machine to compile, package, and run the job.

### 1. Compile the Source Code
Create a directory for the compiled classes and compile the Java file using the Hadoop classpath. *(Note: Ensure `AdvancedWeather.java` is in your current directory).*

```bash
# Create a directory for compiled classes
mkdir -p adv_weather_classes

# Compile the Java source file
javac -classpath ${HADOOP_CLASSPATH} -d adv_weather_classes AdvancedWeather.java
```
### 2. Package into a JAR

Archive the compiled classes into a .jar file so Hadoop can execute it:
```bash
jar -cvf adv_weather.jar -C adv_weather_classes/ .
```
### 3. Prepare the Input Data

Generate a sample dataset and load it into the Hadoop Distributed File System (HDFS).
```bash
# Generate the local mock dataset
cat <<EOF > sample_weather_advanced.txt
City Date Temp DewPoint WindSpeed Humidity Pressure
Pune 2026-05-01 38.5 15.2 12.5 45 1010
Mumbai 2026-05-01 34.0 22.1 18.0 80 1008
Nagpur 2026-05-01 42.1 10.0 8.5 20 1005
Pune 2026-05-02 39.0 14.5 10.0 40 1012
Mumbai 2026-05-02 34.5 22.5 15.5 82 1007
Nagpur 2026-05-02 43.0 9.5 7.0 18 1004
Pune 2026-05-03 37.5 16.0 14.0 50 1011
EOF

# Create an input directory in HDFS
hdfs dfs -mkdir -p /adv_input

# Upload the dataset to HDFS
hdfs dfs -put sample_weather_advanced.txt /adv_input/
```
### 4. Run the MapReduce Job

Execute the MapReduce job. If you are re-running the job, make sure to delete the output directory first, as Hadoop will throw an error if the output folder already exists.
```bash
# (Optional) Remove the output directory if it already exists
hdfs dfs -rm -r -f /adv_output

# Run the MapReduce job
hadoop jar adv_weather.jar demo.AdvancedWeather /adv_input /adv_output
```
### 5. View the Output

Once the job completes, read the results directly from HDFS:
```bash
hdfs dfs -cat /adv_output/part-r-00000
```

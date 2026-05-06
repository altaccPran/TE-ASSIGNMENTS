package demo;

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class AdvancedWeather {

    // MAPPER CLASS
    public static class WeatherMapper extends Mapper<Object, Text, Text, Text> {
        private Text cityKey = new Text();
        private Text metricsValue = new Text();

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            String line = value.toString();
            // Expected format: City Date Temp DewPoint WindSpeed Humidity Pressure
            String[] parts = line.split("\\s+"); 
            
            // Skip the header row and malformed rows
            if (parts.length == 7 && !parts[0].equalsIgnoreCase("City")) {
                try {
                    String city = parts[0];
                    double temp = Double.parseDouble(parts[2]);
                    double wind = Double.parseDouble(parts[4]);
                    double humidity = Double.parseDouble(parts[5]);

                    // Set city as the key
                    cityKey.set(city);
                    // Pass Temp, Wind, and Humidity to the Reducer separated by commas
                    metricsValue.set(temp + "," + wind + "," + humidity);
                    
                    context.write(cityKey, metricsValue);
                } catch (NumberFormatException e) {
                    // Ignore parsing errors
                }
            }
        }
    }

    // REDUCER CLASS
    public static class AdvancedReducer extends Reducer<Text, Text, Text, Text> {
        private Text resultText = new Text();

        public void reduce(Text key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
            double sumTemp = 0, sumWind = 0, sumHumidity = 0;
            double maxTemp = Double.MIN_VALUE;
            double minTemp = Double.MAX_VALUE;
            int count = 0;
            
            for (Text val : values) {
                String[] metrics = val.toString().split(",");
                double temp = Double.parseDouble(metrics[0]);
                double wind = Double.parseDouble(metrics[1]);
                double humidity = Double.parseDouble(metrics[2]);

                sumTemp += temp;
                sumWind += wind;
                sumHumidity += humidity;
                
                if (temp > maxTemp) maxTemp = temp;
                if (temp < minTemp) minTemp = temp;
                
                count++;
            }
            
            if (count > 0) {
                double avgTemp = sumTemp / count;
                double avgWind = sumWind / count;
                double avgHumidity = sumHumidity / count;

                // Format the advanced output string
                String outputStr = String.format(
                    "| Avg Temp: %.1f°C | Max: %.1f°C | Min: %.1f°C | Avg Wind: %.1f | Avg Humidity: %.1f%% |", 
                    avgTemp, maxTemp, minTemp, avgWind, avgHumidity
                );
                
                resultText.set(outputStr);
                context.write(key, resultText);
            }
        }
    }

    // DRIVER (MAIN) CLASS
    public static void main(String[] args) throws Exception {
        Configuration c = new Configuration();
        String[] files = new GenericOptionsParser(c, args).getRemainingArgs();
        
        if (files.length != 2) {
            System.err.println("Usage: AdvancedWeather <in> <out>");
            System.exit(2);
        }

        Job j = Job.getInstance(c, "advanced_weather");
        j.setJarByClass(AdvancedWeather.class);
        j.setMapperClass(WeatherMapper.class);
        j.setReducerClass(AdvancedReducer.class);
        
        j.setOutputKeyClass(Text.class);
        j.setOutputValueClass(Text.class);
        
        FileInputFormat.addInputPath(j, new Path(files[0]));
        FileOutputFormat.setOutputPath(j, new Path(files[1]));
        
        System.exit(j.waitForCompletion(true) ? 0 : 1);
    }
}

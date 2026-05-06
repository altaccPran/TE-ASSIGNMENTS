package demo;

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class Count {

    // MAPPER CLASS
    public static class TokenizerMapper extends Mapper<Object, Text, Text, LongWritable> {
        private final static LongWritable one = new LongWritable(1);
        private Text word = new Text();

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString());
            while (itr.hasMoreTokens()) {
                word.set(itr.nextToken());
                context.write(word, one);
            }
        }
    }

    // REDUCER CLASS
    public static class LongSumReducer extends Reducer<Text, LongWritable, Text, LongWritable> {
        private LongWritable result = new LongWritable();

        public void reduce(Text key, Iterable<LongWritable> values, Context context) throws IOException, InterruptedException {
            long sum = 0;
            for (LongWritable val : values) {
                sum += val.get();
            }
            result.set(sum);
            context.write(key, result);
        }
    }

    // DRIVER (MAIN) CLASS
    public static void main(String[] args) throws Exception {
        Configuration c = new Configuration();
        String[] files = new GenericOptionsParser(c, args).getRemainingArgs();
        
        if (files.length != 2) {
            System.err.println("Usage: Count <in> <out>");
            System.exit(2);
        }

        Job j = Job.getInstance(c, "wordcount");
        j.setJarByClass(Count.class);
        j.setMapperClass(TokenizerMapper.class);
        j.setCombinerClass(LongSumReducer.class);
        j.setReducerClass(LongSumReducer.class);
        
        j.setOutputKeyClass(Text.class);
        j.setOutputValueClass(LongWritable.class);
        
        FileInputFormat.addInputPath(j, new Path(files[0]));
        FileOutputFormat.setOutputPath(j, new Path(files[1]));
        
        System.exit(j.waitForCompletion(true) ? 0 : 1);
    }
}

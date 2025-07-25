import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class crc{
    public static void divide(List<Integer>n,List<Integer>k,List<Integer>ans,List<Integer>rem){
        int i=0;

        for(int y=0;y<k.size();y++){
            rem.add(n.get(y));
            i++;
        }
        while(i<n.size()){
            if(rem.get(0)==1){
                ans.add(1);
                for(int j=0;j<k.size();j++){
                    int xor = k.get(j) ^ rem.get(j);
                    rem.set(j, xor);
                }
                if(rem.get(0)==0){
                    for(int x=0;x<rem.size()-1;x++){
                        rem.set(x, rem.get(x+1));
                    }
                }
            }
            else if(rem.get(0)==0){
                ans.add(0);
                for(int j=0;j<k.size();j++){
                    int xor = rem.get(j) ^ 0;
                    rem.set(j, xor);
                }
                if(rem.get(0)==0){
                    for(int x=0;x<rem.size()-1;x++){
                        rem.set(x, rem.get(x+1));
                    }
                }
            }
            rem.set(rem.size()-1, n.get(i));
            i++;
           
        }
        if(rem.get(0)==1){
            ans.add(1);
            for(int j=0;j<k.size();j++){
                int xor = k.get(j) ^ rem.get(j);
                rem.set(j, xor);
            }
            
        }
        else if(rem.get(0)==0){
            ans.add(0);
            for(int j=0;j<k.size();j++){
                int xor = rem.get(j) ^ 0;
                rem.set(j, xor);
            }
          
        }
        System.out.println("Syndrome: "+ rem.toString());
    }
    public static void main(String[] args) {
        List<Integer>n = new ArrayList<>();
        List<Integer>k= new ArrayList<>(Arrays.asList(1,0,1,0,1));
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter data :");
        String data = sc.nextLine();
        //convert to ascii to binary
        int v=0;
        for(int i=0;i<data.length();i++){
             v = v+ (int)(data.charAt(i));
        }
        String binary = Integer.toBinaryString(v);


        for(int i=0;i<binary.length();i++){
            n.add(binary.charAt(i)-'0');
        }

        for(int i=0;i<k.size()-1;i++){
            n.add(0);
        }

        System.out.println("data: "+n.toString());
        System.out.println("Generator: "+k.toString());
        
        List<Integer>ans = new ArrayList<>();
        List<Integer>rem = new ArrayList<>();
        divide(n, k, ans, rem);
        
        int lengthn = n.size();
        int lengthen = k.size();
        int z=0;
        for(int f=lengthn-1;f>lengthn-lengthen;f--){
            n.set(f, rem.get(z));
            z++;

        }
        System.out.println("Do you want to add error");
        System.out.println("1-->Yes");
        System.out.println("2-->No");
        int ch = sc.nextInt();
        if(ch==1){
            int r = (int) (Math.random()*lengthen);
            // System.out.println("Bit changed position "+(lengthn-r));
            if(n.get(r)==0) n.set(r, 1);
            else if (n.get(r)==1) n.set(r, 0);
            List<Integer>ans1 = new ArrayList<>();
            List<Integer>rem1 = new ArrayList<>();

            divide(n, k, ans1, rem1);
            System.out.println("Error detected");
        }
        if(ch==2){
            List<Integer>ans2 = new ArrayList<>();
            List<Integer>rem2 = new ArrayList<>();
            divide(n, k, ans2, rem2);

        }


        sc.close();



    }
}

import java.io.*;

public class RWNums {

    static int readSplit(InputStream is, char[] buff, char s, int maxlen) throws IOException{
        for(int i=0; i<maxlen-1; i++)
        {
          int c = is.read();
          if(c==-1)
          {
            buff[i+1] = '\0';
            return -1;
          }
          if(((char)c)==s)
          {
            buff[i+1] = '\0';
            return 0;
          }
          buff[i] = (char)c;
        }
        buff[maxlen-1] = '\0';
        return -1;
    }
    
    public static void main(String[] args) {
      int res = 0;
      char[] buff = new char [100];
      Float num;
      
      if(args.length < 3) {
         System.out.println("Usage: java RWFile infile outfile num");
         return;
      }
      num = Float.parseFloat(args[2]);
        
      try {
        InputStream is = new FileInputStream(args[0]);
        OutputStream os = new FileOutputStream(args[1]);
         
         while(res != -1) {
             res = readSplit(is,buff,' ', 100);
             if(res != -1) {
                Float f = num *Float.parseFloat(new String(buff));
                String out = f.toString();
                os.write(out.getBytes());
                os.write(' ');
             }
         }
         is.close();
         os.close();
      } catch (Exception ex) {  ex.printStackTrace(); }        
    }
}

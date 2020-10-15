package task2;
import java.io.*;
import java.util.*;
public class task2main {
  public static void main(String[] args){
	  System.out.println("欢迎来到个人操作界面，接下来将是您此次的操作选项");
	  do{
		  System.out.println("输入0，您将退出操作");
		  System.out.println("输入1，您将对文件进行替换操作");
		  System.out.println("输入2，您将对文件进行删除操作");
		  System.out.println("输入3，您将对文件进行保留操作");
		  try{
			  Scanner stdin = new Scanner(System.in);  
			  int i= stdin.nextInt();
		  }catch(Exception e){ 
			   e.printStackTrace();
			}
		  switch(i){
		  case 0:
			   System.out.println("退出结束");break;
		  case 1:
			  System.out.println("执行操作1");break;
		  case 2:
			  System.out.println("执行操作2");break;
		  case 3:
		      System.out.println("执行操作3");break;
		  default :
			  System.out.println("输入错误，请重新输入");
			  
		  }


		 
	  }while(i==0);
  }
}








public static void reSave(String s, String saveString) {
        String result = "";
        for (int i = 0; i < s.length(); i++) {
            if (saveString.contains(s.charAt(i) + "")) {
                result += s.charAt(i);
            }
        }
     }
}


































                                       
 JAVA YUAN WENJIAN
package task2;
import java.io.*;
import java.util.*;
public class task2main {
  public static void main(String[] args){
	  System.out.println("欢迎来到个人操作界面，接下来将是您此次的操作选项");
	  int input ;
	  String filename="E:/test_string.txt";
	 
	  do{
		  System.out.println("输入0，您将退出操作");
		  System.out.println("输入1，您将对文件进行替换操作");
		  System.out.println("输入2，您将对文件进行删除操作");
		  System.out.println("输入3，您将对文件进行保留操作");
		  System.out.println("输入4，您将对文件进行删除多个字符操作");
		  System.out.println("输入5，您将对文件进行替换一对多字符操作");
		  Scanner stdin = new Scanner(System.in);  
		  input= stdin.nextInt();
		  switch(input){
		  case 0:
			   System.out.println("退出结束");break;
		  case 1:
			  System.out.println("执行操作1");
			  replacechar.readFileByBytes(filename);
			  replacechar.replacecha(filename);
			  
			  break;
		  case 2:
			  System.out.println("执行操作2");
			  deletchar.deletcha(filename);
			  break;
		  case 3:
		      System.out.println("执行操作3");
		      SAVEch.SAVEcha(filename); break;
		  case 4:
		      System.out.println("执行操作4");
		      Deltechar.Deltecha(filename); break;
		  case 5:
			  System.out.println("执行操作5");
			  REplcechar.Replcecha(filename);break;
		  default :
			  System.out.println("输入错误，请重新输入");  
		  }
		 
	  }while(input!=0);
  }
}
//********************************************************
//  定义字符替换
class replacechar{
	 public  static void readFileByBytes(String fileName) {       //读文件的函数，以字节为单位
	        File file = new File(fileName);  
	        InputStream in = null;  
	        try {  
	            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
	            // 一次读一个字节  
	            in = new FileInputStream(file);  
	            int tempbyte;  
	            while ((tempbyte = in.read()) != -1) {  
	                System.out.write(tempbyte);  
	            }  
	            in.close();  
	        } catch (IOException e) {  
	            e.printStackTrace();  
	            return;
	        }
	        System.out.println();
	 }
	 public static void replacecha(String fileName){ //替换并且输出功能的实现
		    File file = new File(fileName); 
		    File obfile = new File("E:/test_string_result.txt");
		    String objetfile="E:/test_string_result.txt";
	        InputStream in = null;  
	        System.out.println("请输入要替换的字母");
	        Scanner inside = new Scanner(System.in);
	        String s=inside.next();
	        char orchar=s.charAt(0);
	        System.out.println("请输入要替换的目的字母");
	        s=inside.next();
	        char rechar=s.charAt(0);
	        System.out.println("将对文件进行"+orchar+"到"+rechar+"的替换");
	        try {  
	            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
	            // 一次读一个字节  
	            in = new FileInputStream(file);  
	            FileOutputStream input = new FileOutputStream(obfile);
	            int tempbyte;  
	            while ((tempbyte = in.read()) != -1) {  
	            	if(tempbyte==orchar){
	            		tempbyte=rechar;
	            	}
	                System.out.write(tempbyte);
	                input.write(tempbyte);  
	                   
	                    // boolean success=true;  
	                    // System.out.println("写入文件成功");  
	            } 
	            in.close();  
	            input.close();
	            }catch (IOException e) {  
	            e.printStackTrace();  
	            return;
	        }
	        System.out.println();
	        readFileByBytes(objetfile);

	 }
	 	 
	 
	 public static void readFileByChars(String fileName) {  //以字符为单位读取文件内容
	        File file = new File(fileName);  
	        Reader reader = null;  
	        try {  
	            System.out.println("以字符为单位读取文件内容，一次读一个字节：");  
	            // 一次读一个字符  
	            reader = new InputStreamReader(new FileInputStream(file));  
	            int tempchar;  
	            while ((tempchar = reader.read()) != -1) {  
	                // 对于windows下，rn这两个字符在一起时，表示一个换行。  
	                // 但如果这两个字符分开显示时，会换两次行。  
	                // 因此，屏蔽掉r，或者屏蔽n。否则，将会多出很多空行。  
	                if (((char) tempchar) != 'r') {  
	                    System.out.print((char) tempchar);  
	                }  
	            }  
	            reader.close();  
	        } catch (Exception e) {  
	            e.printStackTrace();  
	        }  
	 }
}
//*****************************************************************
//一对多
class REplcechar{
	public static void Replcecha(String fileName){
		File file = new File(fileName);  
		File obfile = new File("E:/test_string_result.txt");
        InputStream in = null;  
        int len;   //输入的字符长度
        int i;
        int n;
        // 复杂的输入输出流
        System.out.println("请输入要替换的字母");
        Scanner inside = new Scanner(System.in);
        String s=inside.next();
        char orchar=s.charAt(0);
        System.out.println("请输入要替换的字符串");
        s=inside.next();
        System.out.println("将对文件进行"+orchar+"到"+s+"的替换"); 
        char[] chrry;
        chrry = s.toCharArray();
        len=s.length();
        try {  
            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
            // 一次读一个字节  
            in = new FileInputStream(file);  
            FileOutputStream insi = new FileOutputStream(obfile);
            int tempbyte;  
            while ((tempbyte = in.read()) != -1) {  
            	n=0;
            	if(tempbyte==orchar){
            	    for(i=0;i<len;i++){
            		 System.out.print(chrry[i]);
            		 insi.write(chrry[i]);
            	    }
            	    n=1;
            	}  
            	if(n==0){
            	   System.out.write(tempbyte);
       		       insi.write(tempbyte);
       		       }else{
       		    	insi.write(tempbyte);  
       		       }
            }  
            in.close(); 
            insi.close();
        } catch (IOException e) {  
            e.printStackTrace();  
            return;
        }
        System.out.println();
	}
		
}
//*******************************************************************
//删除单个字符操作
class deletchar{
	public static void deletcha(String fileName){ //  实现删除单个字符操作
		File file = new File(fileName);  
        InputStream in = null;  
        File obfile = new File("E:/test_string_result.txt");
        String objetfile="E:/test_string_result.txt";
        System.out.println("请输入要删除的单个字母");
        Scanner inside = new Scanner(System.in);
        String s=inside.next();
        char dechar=s.charAt(0);
        System.out.println("您要删除的单个字母是"+dechar);
        try {  
            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
            // 一次读一个字节  
            in = new FileInputStream(file);  
            int tempbyte;  
            FileOutputStream input = new FileOutputStream(obfile);
            while ((tempbyte = in.read()) != -1) { 
            	if(tempbyte==dechar){
            		continue;
            	}
                System.out.write(tempbyte);  
                input.write(tempbyte);
            }  
            in.close();  
            input.close();
        } catch (IOException e) {  
            e.printStackTrace();  
            return;
        }
        System.out.println();
        replacechar.readFileByBytes(objetfile);
	}
	
}
//***********************************************************
//定义多个字符删除
class Deltechar{
	public static void Deltecha(String fileName){
		File file = new File(fileName);  
		File obfile = new File("E:/test_string_result.txt");
        InputStream in = null;  
        int len;   //输入的字符长度
        int i;
        int n;
        // 复杂的输入输出流
        System.out.println("请输入要删除的字符");
        Scanner inside = new Scanner(System.in);
        String s=inside.next();
        char[] chrry;
        chrry = s.toCharArray();
        len=s.length();
        System.out.println("您要删除的字符"+s+"长度为"+len);
        try {  
            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
            // 一次读一个字节  
            in = new FileInputStream(file);  
            FileOutputStream insi = new FileOutputStream(obfile);
            int tempbyte;  
            while ((tempbyte = in.read()) != -1) {  
            	for(i=0,n=0;i<len;i++){
            		if(tempbyte==chrry[i]){
            			tempbyte=' ';
                		System.out.write(tempbyte);
                		insi.write(tempbyte);
            			n=1;
            		    continue;	
            		}
            	}
            	if(n==0){
            		System.out.write(tempbyte);
        			insi.write(tempbyte);
            	}  
            }  
            in.close(); 
            insi.close();
        } catch (IOException e) {  
            e.printStackTrace();  
            return;
        }
        System.out.println();
	}
	
}
//****************************************************************8
//失败的字符串保留
class Savechar{
	public static void savecha(String fileName){
		File file = new File(fileName);  
        InputStream in = null; //源文件的读取参数
        InputStream in1 = null; //源文件的副指针，用于比较字符
        File obfile = new File("E:/test_string_result.txt");
        String objetfile="E:/test_string_result.txt";
        System.out.println("请输入要保留的字符串");
        Scanner inside = new Scanner(System.in);
        String savec=inside.nextLine();
        char sac = savec.charAt(0);
        int len=savec.length();
        int i=1;
        int n=0;
        int[] chararry=new int[20]; //新建数组存储文件内容
        char[] chararry1=new char[20];
        System.out.println("您要保留的字符串是"+savec);
        try {  
            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
            // 一次读一个字节  
            in = new FileInputStream(file);
            in1 = new FileInputStream(file);
            int tempbyte;  
            FileOutputStream input = new FileOutputStream(obfile);
            while (((tempbyte = in.read()) != -1)&&(in1.read()!=-1)) { 
            	if(tempbyte==sac){
            		chararry[0]=tempbyte;
            		chararry1[0]=(char)chararry[0];
            		while(i<len){
            			chararry[i]=in1.read();
            			chararry1[i]=(char)chararry[i];
            			i++;
            		}
            		String s=new String(chararry1);
            		if(i==len&&(savec.equals(s))){//若字符相同，计数加一，输出一个
            			n++;
            			System.out.print(savec);
            			while(i<len){
            				in.read();
            			}
            		}
                	
            	} 
               // input.write(tempbyte);
            }  
            in.close();
            in1.close();
            input.close();
        } catch (IOException e) {  
            e.printStackTrace();  
            return;
        }
        if(n==0){
        	System.out.print("无相同字符");
        }else{
        	System.out.print("有"+n+"个相同字符");
        }
       
        System.out.println();
       // replacechar.readFileByBytes(objetfile);
	}		
}
//**************************************************************8
//    保留字符操作的
class SAVEch{
	public static void SAVEcha(String fileName){
		File file = new File(fileName);  
		File obfile = new File("E:/test_string_result.txt");
        InputStream in = null;  
        int len;   //输入的字符长度
        int i;
        int n;
        // 复杂的输入输出流
        System.out.println("请输入要保留的字符");
        Scanner inside = new Scanner(System.in);
        String s=inside.next();
        char[] chrry;
        chrry = s.toCharArray();
        len=s.length();
        System.out.println("您要要保留的字符串"+s+"长度为"+len);
        try {  
            System.out.println("以字节为单位读取文件内容，一次读一个字节：");  
            // 一次读一个字节  
            in = new FileInputStream(file);  
            FileOutputStream insi = new FileOutputStream(obfile);
            int tempbyte;  
            while ((tempbyte = in.read()) != -1) {  
            	for(i=0,n=0;i<len;i++){
            		if(tempbyte==chrry[i]){
            			System.out.write(tempbyte);
            			insi.write(tempbyte);
            			n=1;
            		    continue;	
            		}
            	}
            	if(n==0){
            		tempbyte=' ';
            		System.out.write(tempbyte);
            		insi.write(tempbyte);
            	}  
            }  
            in.close(); 
            insi.close();
        } catch (IOException e) {  
            e.printStackTrace();  
            return;
        }
        System.out.println();
	}
}

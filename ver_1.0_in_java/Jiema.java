import java.util.*;
public class Jiema
{
   int b[] =new int[6];
    public void jiema(int i)
    {
        int cnt=0;
        while(i!=0)
        {
            b[++cnt]=i%3;
            i=i/3;
        }
    }
    public String toString()
    {
        return ""+b[1]+""+b[2]+""+b[3]+""+b[4]+""+b[5];
    }
}//规定上下左右中为01234次幂，/*1表示空格，0为罐子，2为墙,3为小黄*/
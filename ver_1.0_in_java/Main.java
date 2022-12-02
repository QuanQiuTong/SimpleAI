import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.*;
import java.util.Map;

public class Main
{

    public static void main(String[] args) {
        int rank[] = new int[20101];
        //rank[1]=1;
        int cnt=1,noww=0;
        for (int i = 1; i <= 20100; i++) {
            rank[i]=cnt;
            noww++;
            if(noww >= cnt)
            {
                noww=0;
                cnt++;
            }
        }
        /*
              前缀和求排名，保证得分高的有更大概率被选择
              for(int i=1;i<=20100;i++)
            {
                System.out.printf("%d\n ",rank[i]);
            }*/
        Genes DNA[]= new Genes[200];
        for(int i=0;i<=199;i++)
        {
            DNA[i]= new Genes();
        }

    for(int lun=1;lun<=3000;lun++)//进行1000轮训练
        {
         /*   try {
                PrintStream  sc = new PrintStream("src/Celue.txt");
               // System.out.printf("123");
                System.setOut(sc);
                sc.close();
            } catch (FileNotFoundException e) {
                throw new RuntimeException(e);
            }//随机生成并写入文件
            File txt1= new File("src/Celue.txt");

            //System.out.println("Hello world!");
            try {
                Scanner readd=new Scanner(txt1);

            } catch (FileNotFoundException e) {
                throw new RuntimeException(e);
            }*/

            People lily[] = new People[200];

            for (int j = 0; j <= 199; j++)//创建200个小黄
            {
                lily[j]=new People();
                if(lun!=1) lily[j].changeCelue(DNA[j].Celue,0,242);
                DNA[j]= new Genes();
                for (int k = 1; k <= 100; k++)//生成100张图，平均分数
                {
                    //lily[j].map =new Map();
                    lily[j].map.InitMap();
                    lily[j].score = 0;
                    // lily.map.MapVisable(2,2) ;

                    for (int i = 1; i <= 300; i++)//五十步
                    {
                        lily[j].Move();
                    }
                    lily[j].scoreba += lily[j].score;
                }
                lily[j].scoreba = lily[j].scoreba / 100.0;


            }
            Arrays.sort(lily);
          /*  for(int i=0;i<= lily.length-1;i++)
            {
                //System.out.printf("%f\n",lily[i].scoreba );
            }
            for(int j=0;j<=199;j++)
            {

            }*/
            if(lun==2000)
            {
                for(int i=0;i<=242;i++)
                {
                    System.out.printf("%d ",lily[199].str[i]);
                }
            }
            System.out.printf("%d轮学习后小黄的得分是%f\n",lun,lily[199].scoreba );
            for(int i=0;i<=189;i+=2)
            {
                int chouyang=(int) (Math.random()*20100)+1;
                int id1=rank[chouyang]-1;
                int chouyang2=(int) (Math.random()*20100)+1;
                int id2=rank[chouyang2]-1;
               DNA[i].setCelue(lily[id2].str ,0,60);  DNA[i+1].setCelue(lily[id1].str ,0,60);
               DNA[i].setCelue(lily[id1].str ,61,120);  DNA[i+1].setCelue(lily[id2].str ,61,120);
               DNA[i].setCelue(lily[id2].str ,121,180);  DNA[i+1].setCelue(lily[id1].str ,121,180);
               DNA[i].setCelue(lily[id1].str ,181,242);  DNA[i+1].setCelue(lily[id2].str ,181,242);
               //染色体交叉配对，受精过程
            }
            for(int i=190;i<=199;i++)
            {

                int id1=rank[i];
                DNA[i].setCelue(lily[id1].str,0,242);
            }

        }


    }
}
public class People implements Comparable
{
    int x;
    int y;
    int score;
    double scoreba;
    Map map=new Map();
    int str[]= new int[243];//策略表数组
    public People()
    {
        this.score=0;
        this.x=2;
        this.y=2;
        this.scoreba=0;
        getCelue();
    }
    void getCelue()
    {
        for(int i=0;i<=242;i++)
        {
            int num=(int)(Math.random() *7);

            //生成0，1，2，3，4，5，6，代表随机，上，下，左，右，不动，捡罐子7种行为

            str[i]=num;
          //  System.out.printf("%d ",num);
        }
    }
    void changeCelue(int a[],int startt,int endd)
    {
        Jiema jiema=new Jiema();

        for(int i=startt;i<=endd;i++)
        {
            this.str[i]=a[i];
            jiema.jiema(i);
            String s=jiema.toString();
            if(str[i]==0)
            {
                int ra=(int)(Math.random()*4)+1;
                str[i]=ra;
            }
            if(s.charAt(0)=='2')
            {
                if(str[i]==1||str[i]==5)
                {
                    if(s.charAt(4)=='0') str[i]=6;
                    else if(s.charAt(1)!='2') str[i]=2;
                        else if(s.charAt(2)!='2') str[i]=3;
                            else if(s.charAt(3)!='2') str[i]=4;
                }

            }
            if(s.charAt(1)=='2')
            {
                if(str[i]==1||str[i]==5)
                {
                    if(s.charAt(4)=='0') str[i]=6;
                    else if(s.charAt(0)!='2') str[i]=1;
                    else if(s.charAt(2)!='2') str[i]=3;
                    else if(s.charAt(3)!='2') str[i]=4;
                }

            }
            if(s.charAt(2)=='2')
            {
                if(str[i]==3||str[i]==5)
                {
                    if(s.charAt(4)=='0') str[i]=6;
                    else if(s.charAt(1)!='2') str[i]=2;
                    else if(s.charAt(0)!='2') str[i]=1;
                    else if(s.charAt(3)!='2') str[i]=4;
                }
            }
            if(s.charAt(3)=='2')
            {
                if(str[i]==4||str[i]==5)
                {
                    if(s.charAt(4)=='0') str[i]=6;
                    else if(s.charAt(1)!='2') str[i]=2;
                    else if(s.charAt(2)!='2') str[i]=3;
                    else if(s.charAt(0)!='2') str[i]=1;
                }
            }
            if(s.charAt(4)=='0') str[i]=6;
            if(s.charAt(4)=='1')
            {
                if(str[i]==6)
                {
                    int ra=(int)(Math.random()*4)+1;
                    str[i]=ra;
                }

            }
            int Bianyi=(int )(Math.random() *800);
            if(Bianyi==0) this.str[i]=(int )(Math.random()*7);
        }
    }

    boolean HitWall(int posx,int posy)
    {
        if(map.g[posx][posy]==2) return false;
        else return true;
    }
    void MoveUp()
    {
        if(HitWall(this.x-1,this.y)) this.x--;
        else
        {
           this. score -=5;

        }
    }
    void MoveLeft()//左移
    {
        if(HitWall(this.x,(this.y-1))) this.y --;
        else
        {
            this.score -=5;

        }
    }
    void MoveRight()//右移
    {
        if(HitWall(this.x,(this.y+1))) this.y ++;
        else
        {
        this.score -=5;

        }
    }
    void MoveDown()//下移
    {
        if(HitWall((this.x+1),this.y)) this.x ++;
        else
        {
            this.score -=5;

        }
    }
    void PickUp()
    {
        if(map.g[x][y ]==0)
        {
            map.g[x ][y ]=1;
            this.score +=10;
        }
        else this.score -=2;
    }
    void Move()
    {
        int up,down,left,right,mid,clw;//小黄上下左右中，以及策略表中的策略位
        mid=this.map.g [this.x ][this.y];
        up=this.map.g [this.x-1 ][this.y];
        down=this.map.g [this.x+1 ][this.y];
        left=this.map.g [this.x][this.y-1];
        right=this.map.g [this.x][this.y+1];
        clw=up+down*3+left*9+right*27+mid*81;
        if(str[clw]==0)
        {
            int sj=(int)(Math.random() *6)+1;
            if(sj==1)
            {
                this.MoveUp();
            }
            if(sj==2)
            {
                this.MoveDown();
            }
            if(sj==3)
            {
                this.MoveLeft() ;
            }
            if(sj==4)
            {
                this.MoveRight() ;
            }
            if(sj==5)
            {

            }
            if(sj==6)
            {
                this.PickUp() ;
            }

        }
        if(str[clw]==1)
        {
              this.MoveUp() ;


        }
        if(str[clw]==2)
        {
           this.MoveDown() ;

        }
        if(str[clw]==3)
        {
            this.MoveLeft() ;

        }
        if(str[clw]==4)
        {
            this.MoveRight() ;

        }
        if(str[clw]==5)
        {

        }
        if(str[clw]==6)
        {
            this.PickUp() ;

        }
    }//按照策略表行动

    public static void main(String args[])//类测试程序
    {
        Jiema jiema= new Jiema();
        jiema.jiema(20);
        System.out.printf(jiema.toString());
        for(int  j=1;j<=100;j++)//创建100个小黄
        {
            People lily=new People();
            for(int k=1;k<=100;k++)//生成100张图，平均分数
            {

                lily.map.InitMap();
                lily.score =0;
                // lily.map.MapVisable(2,2) ;
                // lily.map.MapVisable(2,2);
                for(int i=1;i<=50;i++)//五十步
                {
                    lily.Move();
                }
                lily.scoreba += lily.score ;
            }
           lily.scoreba = lily.scoreba /100.0;
            System.out.printf("小黄%d的得分是%f\n",j,lily.scoreba );

        }


    }

    @Override
    public int compareTo(Object o)
    {
        People s=(People) o;
        if(s.scoreba<this.scoreba) return 1;
        if(s.scoreba==this.scoreba) return 0;
        else  return -1;
    }
}

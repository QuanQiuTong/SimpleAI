public class Map
{
    int g[][]=new int[13][13];
    int gg[][]=new int[13][13];
    public Map()
    {
        int g[][]=new int[13][13];
        int gg[][]=new int[13][13];
    }
    int GetRandom()
    {
        double num=Math.random();
        if(num>=0.5) return 0;
        else return 1;
    }
    void InitMap()
    {
        for(int i=1;i<=12;i++)
        {
            g[1][i]=2;g[12][i]=2;
            g[i][1]=2;g[i][12]=2;
        } //造围墙

        for(int i=2;i<=12-1;i++)
        {
            for(int j=2;j<=12-1;j++)
            {
                g[i][j]=GetRandom();
                //cout<<g[i][j]<<endl;

            }//cout<<endl;//初始化随机图
            /*1表示空格，0为罐子，2为墙,3为小黄*/
        }

    }
    void MapVisable(int x,int y)
    {
        for(int i=1;i<=12;i++)
        {
            for(int j=1;j<=12;j++)
            {
                gg[i][j]=g[i][j];
            }
        }
        gg[x][y]=3;
        for(int i=1;i<=12;i++)
        {
            for(int j=1;j<=12;j++)
            {
                if(gg[i][j]==2) System.out.printf("# ");
                if(gg[i][j]==1) System.out.printf("  ");
                if(gg[i][j]==0) System.out.printf("@ ");
                if(gg[i][j]==3) System.out.printf("! ");
            }
                System.out.printf("\n");
        }
    }
    public static  void main(String args[])
    {
        Map map=new Map();
        map.InitMap();
        map.MapVisable(2,2) ;
    }

}

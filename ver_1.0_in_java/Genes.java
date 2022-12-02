public class Genes
{
    int id;//存放亲本名字
    int Celue[]= new int[243];//存放DNA；
    public Genes()
    {

    }
    void setCelue (int a[],int startt,int endd)
    {
        for(int i=startt;i<=endd;i++)
        {
            this.Celue[i]=a[i];

        }
    }

}

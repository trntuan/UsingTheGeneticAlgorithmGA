#include <stdio.h>
#include <conio.h>
#include <math.h>

int CT[10]; // 10 ca the
int n=4;
int GTTN[10]; // gia tri thich nghi cua 10 ca the
int kq;
int Max1,Max2;
int Max1moi=0;
int Max2moi=0;
int A[5],B[5];


void Taoquanthe(int n)
{
    for(int i=0; i<n; i++)
    {
        printf("nhap ca the thu %d:",i);
        scanf("%d",&CT[i]);
    }
}

void Giatrithichnghi(int n)
{
    int gttn;
    for(int i=0; i<n; i++)
    {
        gttn=(1000-(CT[i]*CT[i]-64));
        GTTN[i]=gttn;
    }
}

int Kiemtra(int n)
{
    for(int i=0; i<n; i++)
    {
        if(GTTN[i]==1000) return CT[i];
    }
    return 0;
}

void Timcathelai()
{
    for(int i=0; i<n-1; i++)
    {
        for(int j=i+1; j<n; j++)
        {
            if(GTTN[i]<GTTN[j])
            {
                int a = GTTN[i];
                GTTN[i]=GTTN[j];
                GTTN[j]=a;
            }
        }
        Max1=sqrt(1064-GTTN[0]);
        Max2=sqrt(1064-GTTN[1]);
    }
}

void Laicathe()
{
    int tam;
// ma hoa 2 ca the ra nhi phan
    for(int i=0; i<5; i++)
    {
        A[i]=0;
        B[i]=0;
    }
    // ma hoa ca the A thanh thap phan tu Max1
    while(Max1!=0)
    {
        for(int i=0; i<5; i++)
        {
            A[i]=Max1%2;
            Max1=Max1/2;
        }
    }
    // ma hoa ca the A thanh thap phan tu Max2
    while(Max2!=0)
    {
        for(int i=0; i<5; i++)
        {
            B[i]=Max2%2;
            Max2=Max2/2;
        }
    }
    //hoa doi 3 doan gen cuoi giua A va B
    for(int i=4; i>1; i--)
    {
        tam=A[i];
        A[i] = B[i];
        B[i]=tam;
    }
    // giai ma 2 ca the moi vua lai ghep ve thap phan
    for(int m=4; m>=0; m--)
    {
        Max1moi=Max1moi+A[m]*pow(2,m);
        Max2moi=Max2moi+B[m]*pow(2,m);
    }
}

void Taoquanthemoi()
{
    CT[0] = Max1;
    CT[1] = Max2;
    CT[2] = Max1moi;
    CT[3] = Max2moi;
}

int main()
{
    printf("Giai PT X*X=64, bang thuat toan duy truyen\n");
    printf("Tao quan the co 4 ca the\n");
    Taoquanthe(n);
    Giatrithichnghi(n);
    kq = Kiemtra(n);
    while(kq==0)
    {
        Timcathelai();
        Laicathe();
        Taoquanthemoi();
        Giatrithichnghi(n);
        kq=Kiemtra(n);
    }
    printf("Ket qua phuong trinh la: %d",kq);
}

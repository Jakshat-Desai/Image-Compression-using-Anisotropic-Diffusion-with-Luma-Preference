#include<bits/stdc++.h>
#define LAMBDA 0.7//Set between 0 to 1
#define THRESHOLD 0//the value of du/dt below which you dont want to allow diffusion
using namespace std;

int r,c,b;
void YCbCr()//takes input ppm file, seperates them into Y,Cr,Cb
{
    FILE *f,*fw_y,*fw_cb,*fw_cr; int i,j; char fname[100];
  printf("Enter the name of input ppm file:\n");
  scanf("%s",fname);
  f=fopen(fname,"r");
  fw_y=fopen("Y.pgm","w+");
  fw_cb=fopen("Cb.pgm","w+");
  fw_cr=fopen("Cr.pgm","w+");
  char format[10];
  fscanf(f," %s",format);
  fprintf(fw_y,"%s\n","P2");
  fprintf(fw_cb,"%s\n","P2");
  fprintf(fw_cr,"%s\n","P2");
  int r,c,b;
  fscanf(f," %d %d %d",&c,&r,&b);
  fprintf(fw_y,"%d %d\n%d\n",c,r,b);
  fprintf(fw_cb,"%d %d\n%d\n",c,r,b);
  fprintf(fw_cr,"%d %d\n%d\n",c,r,b);
  int rcb[3*c];
  for(i=0;i<r;i++){
    for(j=0;j<3*c;j+=3){
      fscanf(f,"%d%d%d",&rcb[j],&rcb[j+1],&rcb[j+2]);
      int y=floor(rcb[j]*0.299+rcb[j+1]*0.587+rcb[j+2]*0.114);
      int b=floor(-0.168736*rcb[j]-0.331264*rcb[j+1]+0.5*rcb[j+2])+128;
      int r=floor(rcb[j]*0.5-rcb[j+1]*0.418688-rcb[j+2]*0.081312)+128;
      fprintf(fw_y,"%d ",y);
      fprintf(fw_cb,"%d ",b);
      fprintf(fw_cr,"%d ",r);
    }
    fprintf(fw_y, "\n");
    fprintf(fw_cb, "\n");
    fprintf(fw_cr, "\n");
  }
  fclose(f);
  fclose(fw_y);
  fclose(fw_cb);
  fclose(fw_cr);
}
void gaussian()//creates a pgm file containing filtered output of Y channel initially
{
    FILE *fw_output,*channel; int i,j;char fname[100];
    channel=fopen("Y.pgm","r");
    fw_output=fopen("Y_filtered.pgm","w+");
    char format[10];
    fscanf(channel," %s", format);
    fprintf(fw_output,"%s\n",format);
    int r,c,b;
    fscanf(channel,"%d%d%d",&c,&r,&b);
    fprintf(fw_output,"%d %d\n%d\n",c,r,b);
    int points[r][c];
    int newpoints[r][c];
    double gaussian[5][5];
    gaussian[0][0]=0.00296902;gaussian[0][1]=0.0133062;gaussian[0][2]=0.0219382;gaussian[0][3]=0.0133062;gaussian[0][4]=0.00296902;
    gaussian[1][0]=0.0133062;gaussian[1][1]=0.0596343;gaussian[1][2]=0.0983203;gaussian[1][3]=0.0596343;gaussian[1][4]=0.0133062;
    gaussian[2][0]=0.0219382;gaussian[2][1]=0.0983203;gaussian[2][2]=0.162103;gaussian[2][3]=0.0983203;gaussian[2][4]=0.0219382;
    gaussian[3][0]=0.0133062;gaussian[3][1]=0.0596343;gaussian[3][2]=0.0983203;gaussian[3][3]=0.0596343;gaussian[3][4]=0.0133062;
    gaussian[4][0]=0.00296902;gaussian[4][1]=0.0133062;gaussian[4][2]=0.0219382;gaussian[4][3]=0.0133062;gaussian[4][4]=0.00296902;
    for(i=0;i<r;i++)
        for(j=0;j<c;j++){
            fscanf(channel,"%d",&points[i][j]);
        }
    for(i=1,j=1;!(i<r-1 && j<c-1);i++,j++)
    {
        if(i<r-1)
        {newpoints[i][0]=points[i][0];
        newpoints[i][1]=points[i][1];}
        if(j<c-1)
        {newpoints[0][j]=points[0][j];
        newpoints[1][j]=points[1][j];}
    }
    for(i=2;i<r-2;i++)
        for(j=2;j<c-2;j++){
            double sum=0;
            int x,y;
            for(x=-2;x<=2;x++)for(y=-2;y<=2;y++)sum=sum+gaussian[x+2][y+2]*(float)points[i+x][j+y];
            newpoints[i][j]=ceil(sum);
        }
    for(i=0;i<r;i++){
        for(j=0;j<c;j++)
            fprintf(fw_output,"%d ",newpoints[i][j]);
        fprintf(fw_output,"\n");
    }
    fclose(fw_output);
    fclose(channel);
}

void gradient(int ch, float** &newpoints, float** &directions)//finds gradient of one of the channels or the filtered Y channel
{
    FILE* channel;
    int i,j;
    if(ch==-1)
        channel=fopen("Y_filtered.pgm","r");
    else if(ch==0)
        channel=fopen("Y.pgm","r");
    else if(ch==1)
        channel=fopen("Cb.pgm","r");
    else if(ch==2)
        channel=fopen("Cr.pgm","r");
    char format[10];
    fscanf(channel," %s", format);
    fscanf(channel,"%d%d%d",&c,&r,&b);
    float points[r][c];
    newpoints=new float*[r];
    directions=new float*[r];
    for(i=0;i<r;i++)
        {
            newpoints[i]=new float[c];
            directions[i]=new float[c];
        }
    for(i=0;i<r;i++)
        for(j=0;j<c;j++){
            fscanf(channel,"%f",&points[i][j]);
        }
    for(i=1;i<r-1;i++)
    {
            int diff=abs((points[i-1][0]-points[i+1][0]));
            newpoints[i][0]=abs(diff);
            directions[i][0]=0;
    }
    for(j=1;j<c-1;j++)
    {
            int diff=abs((points[0][j-1]-points[0][j+1]));
            newpoints[0][j]=abs(diff);
            directions[0][j]=3.141519/2.0;
    }
    newpoints[0][c-1]=0;
    newpoints[r-1][c-1]=0;
    for(i=1;i<r-1;i++)
        for(j=1;j<c-1;j++){
            float diff1=(points[i-1][j]-points[i+1][j]);
            float diff2=(points[i][j-1]-points[i][j+1]);
            int diff=floor(sqrt(diff1*diff1+diff2*diff2));
            if(diff1!=0)
            directions[i][j]=atan(diff2/diff1);
            else
            directions[i][j]=3.141519/2.0;
            newpoints[i][j]=diff;
            }
    fclose(channel);
     cout<<"flag1\n";
}

void divergence(float** points, float** &newpoints)//finds divergence of input file(entered in the form of a 2D array of intensity values)
{
    int i,j;
    newpoints=new float*[r];
    for(i=0;i<r;i++)
        {
            newpoints[i]=new float[c];
        }
    for(i=1;i<r-1;i++)
    {
            newpoints[i][0]=abs(points[i-1][0]-points[i+1][0]);
    }
    for(j=1;j<c-1;j++)
    {
            newpoints[0][j]=abs(points[0][j-1]-points[0][j+1]);
    }
    for(i=1;i<r-1;i++)
        for(j=1;j<c-1;j++){
            float diff1=(points[i+1][j]+points[i-1][j]-2*points[i][j]);
            float diff2=(points[i][j+1]+points[i][j-1]-2*points[i][j]);
            float diff=(diff1+diff2);
            newpoints[i][j]=(diff);
            }
         cout<<"flag2\n";
}

void addDerivative(int ch, float** ans)//updates the Y,Cr and Cb channel values after diffusion
{
    char* fname;int i,j;
    if(ch==-1)
        fname="Y_filtered.pgm";
    else if(ch==0)
        fname="Y.pgm";
    else if(ch==1)
        fname="Cb.pgm";
    else if(ch==2)
        fname="Cr.pgm";
    FILE* channel;
    channel=fopen(fname,"r");
    char format[10];
    fscanf(channel," %s", format);
    fscanf(channel,"%d%d%d",&c,&r,&b);
    float points[r][c];
    for(i=0;i<r;i++)
        for(j=0;j<c;j++){
            fscanf(channel,"%f",&points[i][j]);
        }
    fclose(channel);
    channel=fopen(fname,"w+");
    fprintf(channel,"%s\n", format);
    fprintf(channel,"%d %d\n%d\n",c,r,b);
    for(i=0;i<r;i++){
        for(j=0;j<c;j++){
            fprintf(channel,"%f ",(abs(ans[i][j])>THRESHOLD && points[i][j]+ans[i][j]>0 && points[i][j]+ans[i][j]<255)?(points[i][j]+ans[i][j]):points[i][j]);
        }
        fprintf(channel,"\n");
        }
        fclose(channel);
         cout<<"flag3\n";
}
void recombine()//recombines the updated Y,Cr and Cb values(after diffusion) and stores the output in output.ppm
{
    FILE *fw_output,*fr_y,*fr_cb,*fr_cr;int i,j;
    fw_output=fopen("output.ppm","w+");
    fr_y=fopen("Y.pgm","r");
    fr_cb=fopen("Cb.pgm","r");
    fr_cr=fopen("Cr.pgm","r");
    char format[10];
    fscanf(fr_y," %s", format);
    fscanf(fr_cb," %s", format);
    fscanf(fr_cr," %s", format);
    fprintf(fw_output,"%s\n","P3");
    int r,c,b;
    fscanf(fr_y,"%d%d%d",&r,&c,&b);
    fscanf(fr_cb,"%d%d%d",&r,&c,&b);
    fscanf(fr_cr,"%d%d%d",&r,&c,&b);
    fprintf(fw_output,"%d %d\n%d\n",r,c,b);
    float rcb[3*c];
    for(i=0;i<r;i++){
        for(j=0;j<3*c;j+=3){
            fscanf(fr_y,"%f",&rcb[j]);
            fscanf(fr_cb,"%f",&rcb[j+1]);
            fscanf(fr_cr,"%f",&rcb[j+2]);
            int red=ceil((rcb[j]-16)+1.402*(rcb[j+2]-128));
            int blue=ceil((rcb[j]-16)-0.344136*(rcb[j+1]-128)-0.714136*(rcb[j+2]-128));
            int green=ceil((rcb[j]-16)+1.772*(rcb[j+1]-128));
            fprintf(fw_output,"%d %d %d ",red,blue,green);
        }
        fprintf(fw_output, "\n");
    }
    fclose(fw_output);
    fclose(fr_y);
    fclose(fr_cb);
    fclose(fr_cr);
}
void compress()//produces compressed ppm image using data from Y,Cb and Cr channels and saves output in comp.ppm
{
        FILE *fw_output,*fr_y,*fr_cb,*fr_cr;int i,j;
    fr_y=fopen("Y.pgm","r");
    fr_cb=fopen("Cr.pgm","r");
    fr_cr=fopen("Cb.pgm","r");
    fw_output=fopen("comp.ppm","w+");
    char format[10];
    fscanf(fr_y," %s", format);
    fscanf(fr_cb," %s", format);
    fscanf(fr_cr," %s", format);
    fprintf(fw_output,"%s\n","P3");
    int r,c,b;
    fscanf(fr_y,"%d%d%d",&r,&c,&b);
    fscanf(fr_cb,"%d%d%d",&r,&c,&b);
    fscanf(fr_cr,"%d%d%d",&r,&c,&b);
    int comp,lpr;
    printf("Enter compression ratio and LP compression ratio\n");
    scanf("%d%d",&comp,&lpr);
    float sy=(b/((1.0+2*lpr)*comp));
    int sze=ceil(sy);
    float frac=b/sy;
    fprintf(fw_output,"%d %d\n%d\n",r,c,sze);
    float rcb[3*r];
    for(i=0;i<c;i++){
        for(j=0;j<3*r;j+=3){
            fscanf(fr_y,"%f",&rcb[j]);
            fscanf(fr_cb,"%f",&rcb[j+1]);
            fscanf(fr_cr,"%f",&rcb[j+2]);
            float Y,Cr,Cb;
            Y=((rcb[j]-16)/(frac));
            Cr=((rcb[j+1]-128)/(frac*lpr));
            Cb=((rcb[j+2]-128)/(frac*lpr));
            int red=floor((Y)+1.402*(Cb));
            int blue=floor((Y)-0.344136*(Cr)-0.714136*(Cb));
            int green=floor((Y)+1.772*(Cr));
            fprintf(fw_output,"%d %d %d ",red,green,blue);
        }
        fprintf(fw_output, "\n");
    }
    fclose(fw_output);
    fclose(fr_y);
    fclose(fr_cb);
    fclose(fr_cr);
}
float lambda1(float val)//calculates lambda1
{
    return 1.0/(sqrt(1+val/(LAMBDA*LAMBDA)));
}
int main()
{
    int t;
    YCbCr();
    gaussian();
    float** dir;
    float** UYsigma_mag;
    gradient(-1,UYsigma_mag,dir);
    float** newpoints;
    float** ans;
    for(int i=0;i<3;i++)
    {
        gradient(i,newpoints,dir);
        divergence(newpoints,ans);
        for(int k=0;k<r;k++)
            for(int j=0;j<c;j++)
        {
            float l1=lambda1(pow(UYsigma_mag[k][j],2));
            float l2=1;
            float prod=(l1+l2)/2;//replace prod with md here
            //md=(l1+l2)/2;
            //FA: float prod=sqrt((3.0/2)*(pow(md-l1,2)+pow(md-l2,2))/(l1*l1+l2*l2));
            //RA: prod=sqrt((pow(md-l1,2)+pow(md-l2,2))/md);
            ans[k][j]*=prod;
        }
        addDerivative(i,ans);
    }
    recombine();
    compress();
}

#include<stdio.h>
#include<math.h>
#include<string.h>
int main()
{
    FILE *fw_output,*channel; int i,j;char fname[100];
    printf("Enter input pgm file name:\n");
    scanf("%s",fname);
    channel=fopen(fname,"r");
    printf("Enter the output pgm file name for channel:\n");
    scanf("%s",fname);
    fw_output=fopen(fname,"w+");
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
    return 0;
}

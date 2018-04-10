#include<stdio.h>
#include<math.h>
#include<string.h>
int main()
{
    FILE *fw_output,*channel,*dir_ppdclr; int i,j;char fname[100];
    printf("Enter input pgm file name:\n");
    scanf("%s",fname);
    channel=fopen(fname,"r");
    printf("Enter the output (magnitude)pgm file name\n");
    scanf("%s",fname);
    fw_output=fopen(fname,"w+");
    printf("Enter the output (direction perpendicular to edge)pgm file name\n");
    scanf("%s",fname);
    dir_ppdclr=fopen(fname,"w+");
    char format[10];
    fscanf(channel," %s", format);
    fprintf(fw_output,"%s\n",format);
    fprintf(dir_ppdclr,"%s\n",format);
    int r,c,b;
    fscanf(channel,"%d%d%d",&c,&r,&b);
    fprintf(fw_output,"%d %d\n%d\n",c,r,b);
    fprintf(dir_ppdclr,"%d %d\n%d\n",c,r,b);
    int points[r][c];
    int newpoints[r][c];
    float directions[r][c];
    for(i=0;i<r;i++)
        for(j=0;j<c;j++){
            fscanf(channel,"%d",&points[i][j]);
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
    for(i=0;i<r;i++){
        for(j=0;j<c;j++){
            fprintf(fw_output,"%d ",newpoints[i][j]);
            fprintf(dir_ppdclr,"%f ",directions[i][j]);
        }fprintf(fw_output,"\n");
    }
    fclose(fw_output);
    fclose(channel);
    fclose(dir_ppdclr);
    return 0;
}

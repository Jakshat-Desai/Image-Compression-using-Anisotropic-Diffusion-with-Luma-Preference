#include<stdio.h>
#include<math.h>
#include<string.h>
int main()
{
    FILE *fw_output,*channel_mag,*channel_dir; int i,j;char fname[100];
    printf("Enter input pgm file name storing magnitude:\n");
    scanf("%s",fname);
    channel_mag=fopen(fname,"r");
    printf("Enter input pgm file name storing direction:\n");
    scanf("%s",fname);
    channel_dir=fopen(fname,"r");
    printf("Enter the output pgm file name for channel:\n");
    scanf("%s",fname);
    fw_output=fopen(fname,"w+");
    char format[10];
    fscanf(channel_mag," %s", format);
    fscanf(channel_dir," %s", format);
    fprintf(fw_output,"%s\n",format);
    int r,c,b;
    fscanf(channel_mag,"%d%d%d",&c,&r,&b);
    fscanf(channel_dir,"%d%d%d",&c,&r,&b);
    fprintf(fw_output,"%d %d\n%d\n",c,r,b);
    int points[r][c];
    int newpoints[r][c];
    float dir[r][c];
    for(i=0;i<r;i++)
        for(j=0;j<c;j++){
            fscanf(channel_mag,"%d",&points[i][j]);
            fscanf(channel_dir,"%f",&dir[i][j]);
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
            double val1=points[i+1][j]*(cos(dir[i+1][j])+sin(dir[i+1][j]));
            double val2=points[i-1][j]*(cos(dir[i-1][j])+sin(dir[i-1][j]));
            newpoints[i][j]=floor((val1-val2));
        }
    for(i=0;i<r;i++){
        for(j=0;j<c;j++)
            fprintf(fw_output,"%d ",newpoints[i][j]);
        fprintf(fw_output,"\n");
    }
    fclose(fw_output);
    fclose(channel_mag);
    fclose(channel_dir);
    return 0;
}

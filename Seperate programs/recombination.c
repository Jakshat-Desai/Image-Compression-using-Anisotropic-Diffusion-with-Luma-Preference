#include<stdio.h>
int main()
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
    int rcb[3*c];
    for(i=0;i<r;i++){
        for(j=0;j<3*c;j+=3){
            fscanf(fr_y,"%d",&rcb[j]);
            fscanf(fr_cb,"%d",&rcb[j+1]);
            fscanf(fr_cr,"%d",&rcb[j+2]);
            int red=floor((rcb[j]-16)+1.402*(rcb[j+2]-128));
            int blue=floor((rcb[j]-16)-0.344136*(rcb[j+1]-128)-0.714136*(rcb[j+2]-128));
            int green=floor((rcb[j]-16)+1.772*(rcb[j+1]-128));
            fprintf(fw_output,"%d %d %d ",red+16,blue+16,green+16);
        }
        fprintf(fw_output, "\n");
    }
    fclose(fw_output);
    fclose(fr_y);
    fclose(fr_cb);
    fclose(fr_cr);
    return 0;
}

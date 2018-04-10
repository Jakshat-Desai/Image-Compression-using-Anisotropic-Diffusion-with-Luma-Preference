#include<stdio.h>
#include<math.h>
int main()
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
    return 0;
}

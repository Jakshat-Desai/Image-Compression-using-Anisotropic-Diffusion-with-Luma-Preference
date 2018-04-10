#include<stdio.h>
#include<math.h>
int main()
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
  fscanf(f," %d %d %d",&r,&c,&b);
  fprintf(fw_y,"%d %d\n%d\n",r,c,b);
  fprintf(fw_cb,"%d %d\n%d\n",r,c,b);
  fprintf(fw_cr,"%d %d\n%d\n",r,c,b);
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
  return 0;
}

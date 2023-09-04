#include <stdio.h>                  /*  Marr-Hildreth.c  (or marrh.c) */
#include <math.h>
#include <stdlib.h>
#define  PICSIZE 256
#define  MAXMASK 100

         int    pic[PICSIZE][PICSIZE];
         double outpicx[PICSIZE][PICSIZE];
         double outpicy[PICSIZE][PICSIZE];
         double magVal[PICSIZE][PICSIZE];
         int    edgeflag[PICSIZE][PICSIZE];
         double maskX[MAXMASK][MAXMASK];
         double maskY[MAXMASK][MAXMASK];
         double convX[PICSIZE][PICSIZE];
         double convY[PICSIZE][PICSIZE];

main(argc,argv)
int argc;
char **argv;
{
  int     i,j,p,q,s,t,mr,centx,centy;
  double  slope,maskval,sum,sig,minival,maxval,ZEROTOL;
  FILE    *fo1, *fo2,*fo3, *fo4, *fp1, *fopen();
  char    *foobar;

  argc--; argv++;
  foobar = *argv;
  fp1=fopen(foobar,"rb");

  argc--; argv++;
  foobar = *argv;
  fo1=fopen(foobar,"wb");

  argc--; argv++;
  foobar = *argv;
  fo2=fopen(foobar,"wb");

  argc--; argv++;
  foobar = *argv;
  fo3=fopen(foobar,"wb");

  argc--; argv++;
  foobar = *argv;
  fo4=fopen(foobar,"wb");

  fprintf(fo1, "P5\n");
  fprintf(fo1, "%i %i\n", 256, 256);
  fprintf(fo1, "255\n");

  fprintf(fo2, "P5\n");
  fprintf(fo2, "%i %i\n", 256, 256);
  fprintf(fo2, "255\n");

  fprintf(fo3, "P5\n");
  fprintf(fo3, "%i %i\n", 256, 256);
  fprintf(fo3, "255\n");

  fprintf(fo4, "P5\n");
  fprintf(fo4, "%i %i\n", 256, 256);
  fprintf(fo4, "255\n");

  argc--; argv++;
  foobar = *argv;
  sig = atof(foobar);

  argc--; argv++;
  foobar = *argv;
  ZEROTOL = atof(foobar);

  mr = (int)(sig * 3);
  centx = (MAXMASK / 2);
  centy = (MAXMASK / 2);

  for (i=0;i<256;i++)
  { for (j=0;j<256;j++)
          {
            pic[i][j]  =  getc (fp1);
          }
  }

  // Calculate X and Y gaussian masks
  for (p=-mr;p<=mr;p++)
  {  for (q=-mr;q<=mr;q++)
      {
        maskval = q*exp(-((q*q)+(p*p))/(2*sig*sig));
        (maskX[p+centy][q+centx]) = maskval;
        maskval = p*exp(-((q*q)+(p*p))/(2*sig*sig));
        (maskY[p+centy][q+centx]) = maskval;
      }
  }

  // Convolve X
  for (i=mr;i<=255-mr;i++)
  { for (j=mr;j<=255-mr;j++)
    {
        sum = 0;
        for (p=-mr;p<=mr;p++)
        {
          for (q=-mr;q<=mr;q++)
          {
              sum += pic[i+p][j+q] * maskX[p+centy][q+centx];
          }
        }
        outpicx[i][j] = sum;
        convX[i][j] = sum;
    }
  }
  // Y convolution
  for (i=mr;i<=255-mr;i++)
  { for (j=mr;j<=255-mr;j++)
    {
        sum = 0;
        for (p=-mr;p<=mr;p++)
        {
          for (q=-mr;q<=mr;q++)
          {
              sum += pic[i+p][j+q] * maskY[p+centy][q+centx];
          }
        }
        outpicy[i][j] = sum;
        convY[i][j] = sum;
    }
  }

  //Find maxval mag and mag image
  maxval = 0;
  for (i=mr;i<256-mr;i++)
  { for (j=mr;j<256-mr;j++)
    {
        magVal[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                (outpicy[i][j]*outpicy[i][j])));
        if (magVal[i][j] > maxval)
          maxval = magVal[i][j];

      }
  }
  

  // generate image for peaks
  for(i=mr; i <256-mr;i++){
    for(j=mr;j<256-mr;j++){

      if(convX[i][j] == 0.0){
        convX[i][j] = .00001;
      }

      slope = convY[i][j]/convX[i][j];

      if(slope <= .4142 && slope > -.4142){
        if((magVal[i][j] > magVal[i][j-1]) && (magVal[i][j] > magVal[i][j+1])){
          edgeflag[i][j] = 255;
          
        }
      }else if(slope <= 2.4142 && slope > .4142){
        if((magVal[i][j] > magVal[i-1][j-1]) && (magVal[i][j] > magVal[i+1][j+1])){
          edgeflag[i][j] = 255;
        
        }
      }else if(slope <= -.4142 && slope > -2.4142){
        if((magVal[i][j] > magVal[i+1][j-1]) && (magVal[i][j] > magVal[i-1][j+1])){
          edgeflag[i][j] = 255;
       
        }
      }
      else{
        if((magVal[i][j] > magVal[i-1][j]) && (magVal[i][j] > magVal[i+1][j])){
          edgeflag[i][j] = 255;
       
        }
      }
    }
  }
  // _________________________________________________________________________
  // Write Images
  // _________________________________________________________________________
        
  // write peaks image

  for (i=0;i<256;i++)
    { for (j=0;j<256;j++)
      {
        fprintf(fo4,"%c",(char)((int)(edgeflag[i][j])));
        
      }
    }
  // write magnitude image
  for (i=0;i<256;i++)
    { for (j=0;j<256;j++)
      {
        magVal[i][j] = (magVal[i][j] / maxval) * 255;            
        fprintf(fo3,"%c",(char)((int)(magVal[i][j])));
        
      }
    }
  
  //Find maxval X
  maxval = 0;
  for (i=0;i<256;i++)
    { for (j=0;j<256;j++)
      {
        if (convX[i][j] > maxval)
          maxval = convX[i][j];
      }
    }
  // write X conv image
  for (i=0;i<256;i++)
    { for (j=0;j<256;j++)
      {
        convX[i][j] = (convX[i][j] / maxval) * 255;            
        fprintf(fo1,"%c",(char)((int)(convX[i][j])));
        
      }
    }

  //Find maxval Y
  maxval = 0;
  for (i=0;i<256;i++)
  { for (j=0;j<256;j++)
    {
      if (convY[i][j] > maxval)
        maxval = convY[i][j];
    }
  }
  // write Y conv image
  for (i=0;i<256;i++)
  { for (j=0;j<256;j++)
    {
      convY[i][j] = (convY[i][j] / maxval) * 255;            
      fprintf(fo2,"%c",(char)((int)(convY[i][j])));
      
    }
  }
}


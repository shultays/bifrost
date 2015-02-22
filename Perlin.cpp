#include "Perlin.h"


void addRandomMapC(int w, int h, float k, int size, Grid<float> &map, int chance){
	Grid<int> temp(w+2, h+2);
  int i, j, a, b;
  float x, y;
  int r = (int)((w+2)*(h+2)*(chance/100.0f));
 // fprintf(fout, "%d\n", r);
    
  for(i=0; i<w+2; i++){
    for(j=0; j<h+2; j++){
      if(r){
        r--;
        temp[i][j] = 1;
      }else temp[i][j] = 0;
      a = rand()%(i+1); 
      
      if(a != i){
        b = rand()%(h+2); 
      }else{
        b = rand()%(j+1); 
      }
      swapt(temp[i][j], temp[a][b]);
    } 
  }
  
  float rx = (rand()%1000)/1000.0f;
  float ry = (rand()%1000)/1000.0f;
  for(i=0; i<size; i++){
    for(j=0; j<size; j++){
      
      a=(int)((i*w/((float)size))+rx);
      b=(int)((j*h/((float)size))+ry);
      x = (rx+(i*w)/((float)size))-a;
      y = (ry+(j*h)/((float)size))-b;
      
      float t1 = temp[a][b]*(1-x)+ temp[a+1][b]*x;
      float t2 = temp[a][b+1]*(1-x)+ temp[a+1][b+1]*x;
      
      float t = t1*(1-y) + t2*y;
      
      map[i][j] += t*k;
    }
  }
}


void addRandomMap(int w, int h, float k, int size, Grid<float> &map){
  
  addRandomMapC(w,h,k, size, map, 40+rand()%20);
}


void lcg_addRandomMap(int x, int y, int s, float k, Grid<float> &map){
  /*int i, j, a, b;
  unsigned int r=0;
  
  
  for(j=0; j<s*3; j++){
    for(i=0; i<s*3; i++){
        int rx1, rx2, ry1, ry2;
        
        rx1=x+i/s-1;
        ry1=y+j/s-1;
        rx2=i%s;
        ry2=j%s;
        
        temp[i][j] = lcg_rand(randSeed[rx1][ry1]+((rx2)*(s)+ry2))&1;
      } 
    }
  for(j=0; j<detailedMeshDetail*3-1; j++){
    for(i=0; i<detailedMeshDetail*3-1; i++){
      
      a=(int)((i*(s)/((float)detailedMeshDetail)));
      b=(int)((j*(s)/((float)detailedMeshDetail)));
      float xn = ((i*(s))/((float)detailedMeshDetail))-a;
      float yn = ((j*(s))/((float)detailedMeshDetail))-b;
      
    
      float t1 = temp[a][b]*(1-xn)+ temp[a+1][b]*xn;
      float t2 = temp[a][b+1]*(1-xn)+ temp[a+1][b+1]*xn;
      
      float t = t1*(1-yn) + t2*yn;
      
      
      map[i][j] += t*k;
    }
  }
  */
}



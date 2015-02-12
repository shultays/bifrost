#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <stdlib.h>
#include <string.h>

template <class T>
class Array2D{
  public:
  T* data;
  int w, h;
  
   Array2D(int w, int h){
    this->w = w;
    this->h = h;
  
    data = (T*)malloc(sizeof(T)*w*h);
  }

  
  void setAll(T a){
    T* p = data + w*h;
    while(p --> data) *p = a; 
  }
  
  inline T* operator[](int i){  
    return data+i*h;
  } 
  
  ~Array2D(){
    free(data);    
  }
};

#endif

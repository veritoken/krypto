//emcc --bind -o test5.js test5.cpp
//test5.cpp
#include <emscripten/bind.h>
#include<emscripten.h>

using namespace emscripten;

float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

float lerp2(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

#include<stdio.h>

void forkOff(int s,int y) {
  printf("WOO HOO! [%d], %d\n", s, y);
  return;
}

extern "C"
const char* hello_world(){
  return "hello, world!";
}

extern "C"
const char* hello_to(char*who){
  static char buf[1024];
  sprintf(buf, "hello, %s!", who);
  return buf;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);
    function("lerp2", &lerp2);
    function("forkOff", &forkOff);
}

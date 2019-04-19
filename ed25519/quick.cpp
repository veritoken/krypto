//emcc --bind -o quick_example.js quick_example.cpp
//quick_example.cpp
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

void forkOff() {
  printf("WOO HOO!\n");
  return;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);
    function("lerp2", &lerp2);
    function("forkOff", &forkOff);
}

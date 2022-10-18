#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define T 5

float generateTemperature(int mn, int mx)
{
  float ratio = rand() / (float)RAND_MAX;
  printf("%f", ratio);
  return mn + ratio * (mx - mn);
}

int main()
{
  time_t t;
  srand((unsigned)time(&t));
  while (1)
  {
    time(&t);
    printf("Temperature = %1.2f @ %s", generateTemperature(-10, 35), asctime(localtime(&t)));
    sleep(T);
  }
}
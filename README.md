# libclockcapcha
C-Library to create fast Capchas based on SVGs

# Features
This library creates a random time (5 minute step) and a randomly obfuscated capcha for that. These could be further blured e.g. by imagick or similar services, by the main goal if this library is to create capchas - as fast as possible, e.g. for anti-ddos protections.

# Usage
```
#include "libcclock.cpp"

int main(){
  LibCClock cclock = LibCClock();

  cclock.width = 600;
  cclock.height = 600;

  LibCClock::Format c = cclock.Generate();

  printf("capcha solution %s\n", c.solution);
  printf("svg content %s\n", c.content);

  return 0;
}
```

Sample Capchas:

![Capcha1](capcha1.png?raw=true "Capcha1")
![Capcha2](capcha2.png?raw=true "Capcha2")

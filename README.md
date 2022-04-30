Some code exapmle of *Operating Systems: Three Easy Pieces*

```bash
gcc -o cpu cpu.c -Wall
gcc -o mem mem.c -Wall
gcc -o thread thread.c -Wall -pthread
gcc -o io io.c -Wall
gcc -o ./build/thread_api thread_api.c -Wall -pthread
```
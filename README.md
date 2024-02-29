# ZPP Raspberry Pi Pico
## Testing Linux version
- In terminal run the socat command to create virtual ports:
```bash
  socat -d -d pty,rawer,echo=0 pty,rawer,echo=0
``` 
- One port is for the Gateway Computer app, another one needs to be written into ` src/picoController/LinuxController.hpp `.
- If LINUX_VERSION in ` CMakeLists.txt ` is ON, set it to OFF and run `afterChangingCmakeLinuxVersion.sh`.
- In `build` directory run:
```bash
    cmake ..
    make
    ./src/LinuxMain
```
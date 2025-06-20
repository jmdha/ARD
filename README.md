# Automatic Room Dectection
An Automatic Room Detection (ARD) algorithm splits a map into rooms, or sections, based on the layout of said map.
Base | Manual | Automatic
:-:|:-:|:-:
![base](https://github.com/user-attachments/assets/3b579f04-01bc-4e8b-9993-9682ea18a5f7) | ![manual](https://github.com/user-attachments/assets/dead282a-d246-42a3-91ac-b5026a4837b5) | ![automatic](https://github.com/user-attachments/assets/7fa22291-f661-4c68-9e02-ac67ffed9527)

The "automatic" is created by the program. It correctly splits the rooms, but fails to handle the corridors. This is a failure of the algorithm; it is not specific to the test case, or implementation (I think).

Do note: the code was written to be concise. There has been little consideration to the performance, nor readability of it.

## Building
Uses [stb](https://github.com/nothings/stb) image library, which is included, however that requires math. As such to build simply link math.
```
git clone https://github.com/jmdha/ARD.git && \
cd ARD && \
gcc -o3 -o ARD main.c -lm
```
The program will be called ARD, which only requires the path to a png to create images.

## Algorithm
TBD

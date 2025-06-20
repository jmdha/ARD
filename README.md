# Automatic Room Dectection
An Automatic Room Detection (ARD) algorithm splits a map into rooms, or sections, based on the layout of said map.
Base | Manual | Automatic
:-:|:-:|:-:
![base](https://github.com/user-attachments/assets/3a961e89-68e4-4cab-8807-c90387cecedf) | ![sections](https://github.com/user-attachments/assets/e2177e82-6adb-4514-88d7-58cf322becf2)

Do note: the code was written to be concise. There has been little consideration to the performance, nor readability of it.

## Building
Uses [stb](https://github.com/nothings/stb) image library, which is included, however that requires math. As such to build simply link math.
```
git clone https://github.com/jmdha/ARD.git && \
cd ARD && \
gcc -o3 -o ARD main.c -lm
``

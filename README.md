# Automatic Room Dectection
An Automatic Room Detection (ARD) algorithm splits a map into rooms, or sections, based on the layout of said map.
Base | Manual | Automatic
:-:|:-:|:-:
![base](https://github.com/user-attachments/assets/3b579f04-01bc-4e8b-9993-9682ea18a5f7) | ![manual](https://github.com/user-attachments/assets/6b577495-abbe-4b0c-baa3-bd0be98dd3b6) | ![automatic](https://github.com/user-attachments/assets/7fa22291-f661-4c68-9e02-ac67ffed9527)


Do note: the code was written to be concise. There has been little consideration to the performance, nor readability of it.

## Building
Uses [stb](https://github.com/nothings/stb) image library, which is included, however that requires math. As such to build simply link math.
```
git clone https://github.com/jmdha/ARD.git && \
cd ARD && \
gcc -o3 -o ARD main.c -lm
``

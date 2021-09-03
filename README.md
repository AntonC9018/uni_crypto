# Criptografie

This repository contains assignments, implemented as part of the course **Criptografie** (**Cryptography**).

It features a GUI app build in GTK. See explanatory markdown documents for each of the assignments in the `doc` folder. Assignments are all in *Romanian*.

> The markdown files use [tex math formulas, which won't show on github](https://stackoverflow.com/questions/11256433/how-to-show-math-equations-in-general-githubs-markdownnot-githubs-blog). To see formulas, you will either have to convert markdown to html or pdf, with [`MathJax`](https://www.mathjax.org/) enabled, or find the compiled pdf's [on my google drive](https://drive.google.com/drive/folders/1Rs0-qy6ivSDuHh5JadrP4Ta4YDhuVRiC).

There are no compiled binaries, because building GTK is a real pain in the butt.

PR's with grammar corrections, bug fixes, improvement suggestions or translations are very welcome.

Leave a star as a way to say "Thank you". Enjoy!


## Approximate build instructions

I personally don't recommend dipping your feet into GTK at all, so don't even try building it honestly.
There is a pretty readable `CMakeLists.txt`, which should aid you to get there. You will also have to adjust `run.bat` to match the paths in your file system.

Also see [uni-cnmo](https://github.com/AntonC9018/uni-cnmo), which uses a similar build method, but it's on Qt instead.
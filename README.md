# Cyberpunk 2077 Breach Protocol Solver

The program attempts to solve Breach Protocol in Cyberpunk 2077.


## Compilation

Any C++ compiler supporting at least C++17 works. However, the GNU C++ compiler (g++) is prefered due to `-Ofast` optimization. For any other compilers, consult the documentation for equivalent optimization flags.


```bash
g++ -std=c++17 -Ofast -o main src/main.cpp
```

## Usage

Upon running, the program can receive input from a file or generate the puzzle randomly. 


### From a file
```bash
azzmi@azmicomp:~/projects/Tucil1_13522109$ g++ -std=c++17 -Ofast -o main src/main.cpp
azzmi@azmicomp:~/projects/Tucil1_13522109$ ./main 
Penyelesaian Cyberpunk 2077 Breach Protocol dengan Algoritma Bruce Force.

Pilih metode input:
1. Input dengan text file.
2. Input dihasilkan secara automatis.

:> 1
Format text file:

buffer_size
matrix_width matrix_height
matrix (jumlah row sebanyak matrix_height dan jumlah kolom sebanyak matrix_width)
number_of_sequences
sequences_1
sequences_1_reward
sequences_2
sequences_2_reward
...
sequences_n
sequences_n_reward

Masukkan alamat lengkap file: test/sample.in
50
7A BD 7A BD 1C BD 55 
1, 1
1, 4
3, 4
3, 5
6, 5
6, 3
1, 3

2.64345 ms

Apakah ingin menyimpan solusi? (y/n)
```

### Puzzle generated randomly
```bash
azzmi@azmicomp:~/projects/Tucil1_13522109$ ./main 
Penyelesaian Cyberpunk 2077 Breach Protocol dengan Algoritma Bruce Force.

Pilih metode input:
1. Input dengan text file.
2. Input dihasilkan secara automatis.

:> 2

Format input:

Jumlah token unik
Token-token
Ukuran buffer
Ukuran matriks
Jumlah sekuens
Ukuran maksimal sekuens

5
BD 1C 7A 55 E9
7
6 6
3
4
Berikut adalah konfigurasi permainan yang dihasilkan secara acak.

E9 7A BD 55 55 E9 
E9 55 7A 55 1C 55 
E9 BD BD 55 BD E9 
7A 1C 7A 7A 1C E9 
1C 55 E9 E9 E9 55 
E9 7A BD 55 7A 55 

BD 55 
4
BD 55 55 55 
8
BD 55 E9 
6

12
E9 E9 55 BD 55 55 55 
1, 1
1, 2
2, 2
2, 3
4, 3
4, 1
5, 1

2.84963 ms

Apakah ingin menyimpan solusi? (y/n)
```



## Author

Name: Azmi M. Bazeid

Class: K01

NIM: 13522109
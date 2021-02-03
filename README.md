# uqac-metaheuristics

## Environments
Sources files given by the teacher relies on Windows packages.  
You need to setup Visual Studio 2019 Community on Windows.

## Debug
Once you've build the project for debug dist will output in **~\x64\Debug**  
Launch a new instance of **Developer Command Prompt for Visual Studio 2019**  
To test the application you have to give this arguments to the .exe :  
```sh
Number of iteration (int) // 20
Dataset (File) // datasets\PROB401.TXT
Algorithm (String) // vns, tabu ... 
```

## Usage
```sh
8INF852TP.exe 1 datasets\PROB401.TXT vns
// S0 : 0 7 2 9 6 5 1 4 3 12 8 11 10 14 13
// TT : 0 0 0 0 0 0 0 0 0 0 0 0 3 37 156
// CT : 100 216 318 421 533 632 729 832 937 1041 1148 1259 1371 1470 1588
// ST : 12 20 4 6 12 2 2 3 3 2 5 4 4 3 18
// Fitness : 196
```
Each iteration takes 60 seconds to run

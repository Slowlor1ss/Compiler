# Compiler
 
This Github reposetory is part of my graduation work at Digital Arts and Entertainment as a Game-Development student.
[Here](ResearchPaper.pdf) you can find a like to the reserach paper that was written as part of the gradwork reasearching the impact of different optimization techniques in a compiler.

## Features
Some of the implemented optimizations include:  
 -Dead Code Elimination  
 -Constant Propagation  
 -Instruction Selection  
 -Register Allocation  
 -Peephole Optimization  
How each one of these have been implemented and what algortihm was used is described in detail inside of the paper.
```
Compiler flags:  
 -O1           [turn on all optimization flags]  
 -O0           [turn of all optimization flags]  
 -i            [specify input file]  
 -o            [specify output file]  
 -fcprop       [const propagation]  
 -fdce         [dead code elimination]  
 -fverbose-asm [added comments to the generated assembly code]  
 -fregalloc    [naive register allocation]  
 -fpeephole    [peephole optimizations]  
 -e            [specify a error log file (if not specified will be printed to console)]  
```
## Demo Showcase

To test and demonstarte the project I have also set up a small [Godbolt](https://godbolt.org/) like website, for this is used some simple html, CSS, JS, PHP, and some powershell scripts to run the project and added features.

## About

The project includes a self made compiler for a self made laguage, combined with various compiler optimizations, to make this for the front-end of the compiler [ANTLR](https://www.antlr.org/) was used. ANTLR is a tool for language recognation and is used in this project to define the self made language and generate the AST(abstract syntax tree). This is the only external project used to make this compiler and everything else has been self made and written in C++. For a detailed explenation and of how everything has been implemented and made you can refer to the source files and the research paper.

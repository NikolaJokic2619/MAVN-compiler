# MAVN Compiler

A compiler that translates MAVN (MIPS Assembler of High Level) programs 
into MIPS 32-bit assembly language.

## About

MAVN is a higher-level MIPS assembly language that allowing programmers to use named variables instead 
of just registers.

## Project Structure

src/        - source code
examples/   - example .mavn input files
doc/        - project documentation

## Build

g++ *.cpp -o projekat

## Usage

./projekat

## Output

Generates .s file containing valid MIPS 32-bit assembly 
that can be run on the QtSpim simulator.

## Course

Osnovi paralelnog programiranja i softverski alati - 2026
Fakultet Tehničkih nauka, Novi Sad

Fundamentals of Parallel Programming and Software Tools - 2026
Faculty of Technical Sciences, Novi Sad, Serbia

C project for OS. Due 18:00 7th November 2014

folder for submission in one of our accounts: "~/ca321-1415/assignment1"

Adam O'Flynn 12378651
Conor Smyth 12452382

Overview

This assignment is about POSIX threads and signal handling.

Assignment

You are to write a C program that prints the sum of each pair of integers in a text file. 
(The file whose contents are to be printed is supplied as a command line parameter.) 

Program requirements are as follows:

One thread (the reader) reads a pair of integers from the input file and prints them to the screen.
Another thread (the calculator) then sums the numbers just loaded by the reader and prints the result to the screen.
The reader blocks while waiting for the calculator to print the latest result.
The calculator blocks while waiting for the reader to submit a pair of integers for addition.
Before reading the next pair of integers from the file, the reader sleeps for a random number of microseconds (up to 10000).
After summing the latest two integers loaded by the reader, the calculator sleeps for a random number of microseconds (up to 10000).
A main thread coordinates the actions of the reader and calculator threads.
Unix signals are used for inter-thread communication and to avoid busy-waiting.
The main thread cancels the other threads once the last pair of integers has been summed.
The main thread cancels the other threads if a SIGINT is received.
Each cancelled thread prints out a farewell message as it exits.
A working solution would produce output like that shown here.

Getting started

The sample code on the course home page will be useful in helping you get started with this assignment. The following are available to you:

POSIX threads solution to the bounded buffer problem
Code demonstrating signal handling
Code demonstrating thread cancellation
Code demonstrating signal handling in multithreaded programs

Deliverables

You are required to deliver the following in text files:

One C source code file containing your solution
description.txt
problems.txt

Marks are awarded for solving the assignment, solving it efficiently and demonstrating your understanding of problems, issues and solutions through comments. Any shortcomings in your solution must be clearly identified. 

Marks will be deducted for:

Overly-complicated solutions
Failing to cleanly shutdown threads
Excessive resource usage
Busy-waiting
Solutions that unnecessarily rely on calls to sleep to function correctly
Commented out code
Undocumented shortcomings
Lines longer than 80 characters
Unrequested additional "features"
Failing to meet the requirements listed above
Provide a brief description of how your solution functions in design.txt. In problems.txt provide an explanation of any shortcomings or weaknesses in your solution. Failure to list any shortcomings indicates you are not aware of them and marks will be deducted accordingly.

What's it worth?

15% of your overall mark.

C project for OS. Due 18:00 7th November 2014

folder for submission in one of our accounts: 
```
~/ca321-1415/assignment1
```
|Name|Student Number|
|:-------------:|:--------:|
|Adam O'Flynn | 12378651|
|Conor Smyth | 12452382|

<h4>Overview</h4>

This assignment is about POSIX threads and signal handling.

<h3>Assignment</h3>

You are to write a C program that prints the sum of each pair of integers in a text file. 
(The file whose contents are to be printed is supplied as a command line parameter.) 

Program requirements are as follows:

One thread (the reader) reads a pair of integers from the input file and prints them to the screen.<br />
Another thread (the calculator) then sums the numbers just loaded by the reader and prints the result to the screen.<br />
The reader blocks while waiting for the calculator to print the latest result.<br />
The calculator blocks while waiting for the reader to submit a pair of integers for addition.<br />
Before reading the next pair of integers from the file, the reader sleeps for a random number of microseconds (up to 10000).
After summing the latest two integers loaded by the reader, the calculator sleeps for a random number of microseconds (up to 10000).<br />
A main thread coordinates the actions of the reader and calculator threads.<br />
Unix signals are used for inter-thread communication and to avoid busy-waiting.<br />
The main thread cancels the other threads once the last pair of integers has been summed.<br /> 
The main thread cancels the other threads if a SIGINT is received.<br />
Each cancelled thread prints out a farewell message as it exits.<br />
A working solution would produce output like that shown here.<br />

<h4>Getting started</h4>

The sample code on the course home page will be useful in helping you get started with this assignment. The following are available to you:<br />

POSIX threads solution to the bounded buffer problem<br />
Code demonstrating signal handling<br />
Code demonstrating thread cancellation<br />
Code demonstrating signal handling in multithreaded programs<br />

<h4>Deliverables</h4>

You are required to deliver the following in text files:<br />

One C source code file containing your solution
```
description.txt
problems.txt
```

Marks are awarded for solving the assignment, solving it efficiently and demonstrating your understanding of problems, issues and solutions through comments. Any shortcomings in your solution must be clearly identified. 

Marks will be deducted for:

Overly-complicated solutions<br />
Failing to cleanly shutdown threads<br />
Excessive resource usage<br />
Busy-waiting<br />
Solutions that unnecessarily rely on calls to sleep to function correctly<br />
Commented out code<br />
Undocumented shortcomings<br />
Lines longer than 80 characters<br />
Unrequested additional "features"<br />
Failing to meet the requirements listed above<br />
Provide a brief description of how your solution functions in design.txt. <br />In problems.txt provide an explanation of any shortcomings or weaknesses in your solution. <br />Failure to list any shortcomings indicates you are not aware of them and marks will be deducted accordingly.

<h4>What's it worth?</h4>

15% of your overall mark.

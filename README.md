<p align="center">
  <img src="https://raw.githubusercontent.com/shkyler/gmit-pfda-project/master/img/GMITLOGO.jpg" width="500"/></p>

# GMIT Multi-Paradigm Programming Shop Assignment
The purpose of this assignment is to write a shop simulation using a procedural programming style in C, and then to write the same project using an object oriented style in Java. A short report is then to be written to compare the 2 paradigms.

In this repository you will find the C program in the file `c-shop`. The code for the program is written in the file `shop.c`. 

The report is available in `G00364753 - P Moore - MPP - Shop Report.pdf`.

As part of the project we were to provide different orders which may or may not be processible by the shop.

I have included 4 order.csv files as follows:
* order1.csv - the shop should be able to process this order (1 time)
* order2.csv - the total bill should exceed the customer budget - this will throw an appropriate error and return to the program
* order3.csv - has an item on the list that is not stocked - this will throw an appropriate error and return to the program
* order4.csv - orders more Coke Cans than are available - this will throw an appropriate error and return to the program

For the Java implementation:

I have included the project files in the `java-shop` folder. The project source code files are available in `/src` folder. I have also included copies of the 4 order files for the java implementation.

## Running the software

In order to run this software is is expected that you have an operational <a href="https://gcc.gnu.org/">C compiler</a> and <a href="https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html">Java compliler</a> installed. 

The best way get the software installed is to clone this repository from Github as follows:

`git clone https://github.com/shkyler/gmit-mpp-shop.git`

To run the C project from this repository run the following commands:
``` 
cd c-shop
gcc shop.c
./a.out
```

To run the Java project from this repository run the following commands:
```
cd java-shop/src/shop
javac *.java -d../../bin
cd ../../
java -cp bin/ shop.Shop

```
This compiles the code to the `bin` folder and and then runs the compiled code from the project root.


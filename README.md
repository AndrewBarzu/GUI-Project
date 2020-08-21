## GUI-Project

# Broad description

This is a GUI project made in c++ with Qt in Windows Visual Studio

This is a C++ GUI project, made with Qt, which allows basic CRUD operations.
It features multiple repository types, from non-persistent in memory allocation to csv and db repositories. It also contains a
"favorites" list, a chart for displaying some statistics and two separate windows that are made using the MVC pattern.

# Design Patterns used

The project makes use of various design patterns, such as: 

**_Command pattern:_**

Used in the creation of the cascading undo/redo functionality, using wrapper objects for different actions taken, and keeping a history of each one in two stacks, one for undos and one for redos. This functionality can also make use of shortcuts, such as ctrl+z and ctrl+y for undo and redo respectively


**_Iterator pattern:_**

Used in the creation of the repositories, they now are easier to traverse making use of the iterator. This way it can hide it's underlying structure while still allowing easy access to it's data.


**_Hook pattern:_**

Used for creating a debug hook. This was needed because Qt didn't report memory leaks correctly and was triggering false positives, and i really needed to see where my memory leaks happened, so i had to override the "new" keyword with another one defined by me, that would write in logs whenever an object created with that "new" keyword was destroyed.
Works like a charm!

# Repository types

The program also makes use of 4 types of repositories, with the same interface, so they can be used interchangeably: 

**_In memory:_**

Probably the most basic one, stores data in memory and can never be reused

**_In file:_**

Uses a CSV formatted file to store data. Can also be viewed as an excel spreadsheet

**_In HTML:_**

Data is formatted as rows in an html table, pretty printed. All the printing is done by the class, so there's no need for the user to think about that.

**_In SQL database:_**

Using sqlite3, stores the data in a local sqlite3 database.

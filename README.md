# Overview
MingDatabase is an experimental/toy database that I am developing to understand how to implement a primitive database. In particular, this project does not attempt to be a performant or ready for production use. It is, and will, stay simple to make understanding/implementing it easy.

# Architecture
This project employs a strict layered architecture. Each layer of the system depends only on the immediate layer below it. If necessary, the underlying layer can expose callback so that the upper layer can provide information on how perform a certain step. Here are the layers:

* File Layer
* Allocation layer
* Caching layer
* Database layer
    * BTree
    * Table

# File Layer
The file layer is responsible for handling details about file I/O access. Beyond this layer, there should not be any direct access to the file itself. It can handle read/write of arbitrary pages with a page number, as well as getting new pages at the end of the file.

**Status**: Implemented, with the file end truncation work pending.

# Allocation layer
The allocation layer is responsible for handling allocation/deallocation requests. All allocation requests are of fixed size (i.e. 1 page), and therefore, a simple free list based solution suffice. When the free-list is full, it will compact itself with some help from the upper layer to make sure references are updated correctly.

**Status**: Implemented, with the update parent reference work pending.

# Caching layer
The caching layer is responsible for caching the pages so that repeated access do not hit the disk. It **owns** the memory buffers, and it is also responsible for making sure changed pages will eventually be written to disk.

**Status**: Designed - with some scaffolding/incomplete implementation.

# Btree
Btree should work on top of the caching layer - it assign meaning to the pages (except the 0th), it grows from both ends to store keys and (value/children). The code is data_structure is a model for this, most complications related to implementing this as a file should already be solved by it.

* insert a key/value based record by key
* delete a record by key
* select a record by key
* update a value by key
 
Note that a BTree is an object in the database layer, not a layer of its own.

**Status**: We have a prototype implementation of the BTree in the MiscLab project. That will be used as a reference to implement this one.

# Table
A table is basically a btree with some additional metadata information telling what the keys are so that the the key/value are given meaning. The metadata itself is stored as a special table with fixed metadata so the the table understand it.

* insert a record
* delete a record
* select a record
* update a value 

Note that a table is an object in the database layer, not a layer of its own.

# Database layer
Basically a collection of table. Haven't thought too much this far yet.

**Status**: I have a dream ... okay ... There is nothing here yet.

# More?
Secondary index / query processing / transaction is basically out of scope, too much for now.
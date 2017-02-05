# Thoughts about how to design this database project
I started this project with experimenting with the unfamiliar btree data structure, and I think I have basically understood it good enough. Now we can move on to build the real database - the code there, however, is a burden now. So I moved it to the data_structure project and plan to write a new one here.

# The roadmap
The lowest layer of this project is to interact with the file system. On top of it, we should have an abstraction of it that allow us to allocate pages from it (which include a free list of pages) and be able to compact the pages. On top of it, we should be able to build btrees. On top of it, we can build table. And on top of it, we build database.

# FileSystem
The file system need to serve these requests:
* Read a page with number
* Write a page with number
* Allocate a page (tell me what number it is)
* Deallocate the last page

The goal of this system is to make it easy to port to different platforms and to abstract away the fact that this is persisted. The implementation of this is almost trivial, no attempt is made to make this fast except from the page size.

# Page Manager
The page manager need to serve these requests:
* Read a page with a page number
* Get me a blank page (and tell me what number it is)
* Deallocate a numbered page (not necessarily the last)
* Write a page with a page number
* Compacting the pages automatically (with some callbacks to the upper layer is necessary for this)

To make this simple, we leave caching outside of this. The 0th page will be used as a special page for free-list. Reading a page with number is a fall-through to file system, getting a blank page is free-list or fall through, deallocating a page is a free-list or fall-through, writing a page is a fall through. Nothing is crazy hard at this point. 

Compacting is the interesting part. It should be triggered by a full free-list. The idea is to move the bubble to the end, like a bubble sort, with that we can do the moving in-place. The only issue is that the page being moved might be referenced by other pages. We need to use the help from the btree layer to solve this - essentially - we need the pages to have parent pointer whenever it is referenced, and the compacting logic is uninterested in knowing those, so it use callback to ask the upper layer, that interprets the page, to patch it. That is an expensive process and we will want to make sure we do not accrue too much work.

# Caching
The cache manager serve these requests:
* Read a page with a page number
* Get me a blank page (and tell me what number it is)
* Deallocate a numbered page (not necessarily the last)

With compacting solved by the lower layer, this layer focus on caching. This is basically a LRU cache. We need to make sure evicted dirty pages are written back to the page.

# Btree
Btree should work on top of the caching layer - it assign meaning to the pages (except the 0th), it grows from both ends to store keys and (value/children). The code is data_structure is a model for this, most complications related to implementing this as a file should already be solved by it.
* insert a key/value based record by key
* delete a record by key
* select a record by key
* update a value by key

# Table
A table is basically a btree with some additional metadata information telling what the keys are so that the the key/value are given meaning. The metadata itself is stored as a special table with fixed metadata so the the table understand it.
* insert a record
* delete a record
* select a record
* update a value 

# Database
Basically a collection of table. Haven't thought too much this far yet.

# More?
Query processing / transaction is basically out of scope, too much for now.












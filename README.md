# WrapLite 
A C++20 wrapper for the C SQLite3 API.

### Note
This is still a work in progress, and for anyone wanting to use a production-ready library, [sqlite_modern_cpp](https://github.com/SqliteModernCpp/sqlite_modern_cpp) is probably the best possibility.

# About
This project is a wrapper for the legacy [SQLite3 API](https://www.sqlite.org/amalgamation.html). It is being created for my own entertainment, and to get some more knowledge about C++20, concepts and templates in general. Therefore, I can't call it my own work completely, since a lot of the code and ideas originate from the [sqlite_modern_cpp](https://github.com/SqliteModernCpp/sqlite_modern_cpp) library.

# Architecture
1. **Legacy**: This project contains the legacy C code that is described above. No changes has been made in this project from the original source code.
2. **Conversion Layer**: The conversion layer was made with the idea to separate the old C code and the new C++ code as much as possible. This therefore holds all wrapper functions for communication with the SQLite3 library, and it also does certain safety checks and throws exceptions upon errors.
3. **WrapLite**: The project itself. This solely calls the conversion layer when communicating with the database, and is the high level API.

## To do:
1. Create a class that can specify options to use for the sqlite3 API. These will be all the flags that can be passed to sqlite3_open_v2.
2. Add static methods to create and delete existing databases as well as get general non-connection dependent information from them.
3. Change the `callback_binder` class to my own implementation.
4. Write unit tests.
5. Refactor existing code.
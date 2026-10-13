# Student Management System

A menu-driven Student Management System developed in C with SQLite for persistent data storage.

## Features

- Add student records
- Display all students
- Search for a student by roll number
- Update student details
- Delete student records
- Store records permanently using SQLite

## Technologies Used

- C
- SQLite
- GCC
- Git and GitHub

## Requirements

- GCC compiler
- SQLite source files (`sqlite3.c` and `sqlite3.h`)

## How to Compile

Run the following command in the project directory:

```bash
gcc -Wall -Wextra -std=c11 main.c sqlite3.c -o student.exe
```

## How to Run

On Windows PowerShell:

```powershell
.\student.exe
```

On MSYS2 / MinGW Bash:

```bash
./student.exe
```

The application creates `students.db` automatically when it initializes the database.

## Database

Student records are stored in SQLite with roll number, name, and marks.

## Author

Ajit Singh
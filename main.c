
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

sqlite3 *db;

int execute_sql(const char *sql) {
    char *error = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &error);

    if (rc != SQLITE_OK) {
        printf("Database error: %s\n", error ? error : sqlite3_errmsg(db));
        sqlite3_free(error);
        return 0;
    }
    return 1;
}

int initialize_database(void) {
    if (sqlite3_open("students.db", &db) != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS students ("
        "roll INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "marks REAL NOT NULL CHECK(marks >= 0 AND marks <= 100)"
        ");";

    return execute_sql(sql);
}

void add_student(void) {
    int roll;
    char name[100];
    double marks;
    sqlite3_stmt *stmt = NULL;

    printf("Enter roll number: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number.\n");
        return;
    }

    printf("Enter name (max 99 characters): ");
    if (scanf(" %99[^\n]", name) != 1) {
        printf("Invalid name.\n");
        return;
    }

    printf("Enter marks (0-100): ");
    if (scanf("%lf", &marks) != 1 || marks < 0 || marks > 100) {
        printf("Invalid marks.\n");
        return;
    }

    const char *sql =
        "INSERT INTO students (roll, name, marks) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Prepare error: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, roll);
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, marks);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("Student added successfully.\n");
    } else {
        printf("Could not add student: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void display_students(void) {
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT roll, name, marks FROM students ORDER BY roll;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\n%-10s %-30s %-10s\n", "Roll", "Name", "Marks");
    printf("------------------------------------------------------\n");

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("%-10d %-30s %-10.2f\n",
               sqlite3_column_int(stmt, 0),
               (const char *)sqlite3_column_text(stmt, 1),
               sqlite3_column_double(stmt, 2));
        found = 1;
    }

    if (!found) {
        printf("No students found.\n");
    }

    sqlite3_finalize(stmt);
}

void search_student(void) {
    int roll;
    sqlite3_stmt *stmt = NULL;

    printf("Enter roll number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number.\n");
        return;
    }

    const char *sql = "SELECT roll, name, marks FROM students WHERE roll = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, roll);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Roll: %d\n", sqlite3_column_int(stmt, 0));
        printf("Name: %s\n", (const char *)sqlite3_column_text(stmt, 1));
        printf("Marks: %.2f\n", sqlite3_column_double(stmt, 2));
    } else {
        printf("Student not found.\n");
    }

    sqlite3_finalize(stmt);
}

void update_student(void) {
    int roll;
    char name[100];
    double marks;
    sqlite3_stmt *stmt = NULL;

    printf("Enter roll number to update: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number.\n");
        return;
    }

    printf("Enter new name: ");
    if (scanf(" %99[^\n]", name) != 1) {
        printf("Invalid name.\n");
        return;
    }

    printf("Enter new marks (0-100): ");
    if (scanf("%lf", &marks) != 1 || marks < 0 || marks > 100) {
        printf("Invalid marks.\n");
        return;
    }

    const char *sql = "UPDATE students SET name = ?, marks = ? WHERE roll = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, marks);
    sqlite3_bind_int(stmt, 3, roll);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            printf("Student updated successfully.\n");
        } else {
            printf("Student not found, or details were unchanged.\n");
        }
    } else {
        printf("Update failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

void delete_student(void) {
    int roll;
    sqlite3_stmt *stmt = NULL;

    printf("Enter roll number to delete: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid roll number.\n");
        return;
    }

    const char *sql = "DELETE FROM students WHERE roll = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Database error: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, roll);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        if (sqlite3_changes(db) > 0) {
            printf("Student deleted successfully.\n");
        } else {
            printf("Student not found.\n");
        }
    } else {
        printf("Delete failed: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

int main(void) {
    if (!initialize_database()) {
        if (db != NULL) {
            sqlite3_close(db);
        }
        return EXIT_FAILURE;
    }

    int choice;

    do {
        printf("\n================================\n");
        printf("    STUDENT MANAGEMENT SYSTEM\n");
        printf("================================\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1: add_student(); break;
            case 2: display_students(); break;
            case 3: search_student(); break;
            case 4: update_student(); break;
            case 5: delete_student(); break;
            case 6: printf("Exiting program. Goodbye!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    sqlite3_close(db);
    return EXIT_SUCCESS;
}

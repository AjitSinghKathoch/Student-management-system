
#include <stdio.h>

#define MAX_STUDENTS 100

struct Student {
    int roll;
    char name[50];
    float marks;
};

int main(void) {
    struct Student students[MAX_STUDENTS];
    int count = 0;
    int choice;
    int roll;
    int found;
    int i, j;

    do {
        printf("\n================================\n");
        printf("     STUDENT MANAGEMENT SYSTEM\n");
        printf("================================\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
           
case 1: {
    if (count >= MAX_STUDENTS) {
        printf("Student limit reached!\n");
        break;
    }

    int newRoll;
    int duplicate = 0;

    printf("Enter roll number: ");
    if (scanf("%d", &newRoll) != 1) {
        printf("Invalid roll number.\n");
        while (getchar() != '\n');
        break;
    }

    for (i = 0; i < count; i++) {
        if (students[i].roll == newRoll) {
            duplicate = 1;
            break;
        }
    }

    if (duplicate) {
        printf("Roll number already exists!\n");
        break;
    }

    printf("Enter name (one word): ");
    if (scanf("%49s", students[count].name) != 1) {
        printf("Invalid name.\n");
        break;
    }

    printf("Enter marks (0-100): ");
    if (scanf("%f", &students[count].marks) != 1) {
        printf("Invalid marks.\n");
        while (getchar() != '\n');
        break;
    }

    if (students[count].marks < 0 ||
        students[count].marks > 100) {
        printf("Marks must be between 0 and 100.\n");
        break;
    }

    students[count].roll = newRoll;
    count++;

    printf("Student added successfully!\n");
    break;
}

             
               
           case 2:
    printf("DEBUG: Option 2 is working!\n");

    if (count == 0) {
        printf("No student records found.\n");
        break;
    }

                printf("\n%-10s %-20s %-10s\n",
                       "Roll No.", "Name", "Marks");
                printf("------------------------------------------\n");

                for (i = 0; i < count; i++) {
                    printf("%-10d %-20s %-10.2f\n",
                           students[i].roll,
                           students[i].name,
                           students[i].marks);
                }
                break;

            case 3:
                printf("Enter roll number to search: ");
                if (scanf("%d", &roll) != 1) {
                    printf("Invalid roll number.\n");
                    while (getchar() != '\n');
                    break;
                }

                found = 0;
                for (i = 0; i < count; i++) {
                    if (students[i].roll == roll) {
                        printf("\nStudent found!\n");
                        printf("Roll Number: %d\n", students[i].roll);
                        printf("Name: %s\n", students[i].name);
                        printf("Marks: %.2f\n", students[i].marks);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("Student not found.\n");
                }
                break;

            case 4:
                printf("Enter roll number to update: ");
                if (scanf("%d", &roll) != 1) {
                    printf("Invalid roll number.\n");
                    while (getchar() != '\n');
                    break;
                }

                found = 0;
                for (i = 0; i < count; i++) {
                    if (students[i].roll == roll) {
                        printf("Enter new name (one word): ");
                        if (scanf("%49s", students[i].name) != 1) {
                            printf("Invalid name.\n");
                            break;
                        }

                        printf("Enter new marks (0-100): ");
                        if (scanf("%f", &students[i].marks) != 1 ||
                            students[i].marks < 0 ||
                            students[i].marks > 100) {
                            printf("Invalid marks.\n");
                            while (getchar() != '\n');
                            break;
                        }

                        printf("Student updated successfully!\n");
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                    printf("Student not found or update failed.\n");
                }
                break;

            case 5:
                printf("Enter roll number to delete: ");
                if (scanf("%d", &roll) != 1) {
                    printf("Invalid roll number.\n");
                    while (getchar() != '\n');
                    break;
                }

                found = 0;
                for (i = 0; i < count; i++) {
                    if (students[i].roll == roll) {
                        for (j = i; j < count - 1; j++) {
                            students[j] = students[j + 1];
                        }

                        count--;
                        found = 1;
                        printf("Student deleted successfully!\n");
                        break;
                    }
                }

                if (!found) {
                    printf("Student not found.\n");
                }
                break;

            case 6:
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Enter a number from 1 to 6.\n");
        }

    } while (choice != 6);

    return 0;
}

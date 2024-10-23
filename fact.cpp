#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>  // For trimming white spaces

using namespace std;

// Constants for array sizes
const int MAX_STUDENTS = 200;
const int MAX_SUBJECTS = 12; // Name, ID, Section + 6 Subjects

// Custom Stack for Undo Functionality
struct Stack {
    string records[MAX_STUDENTS][MAX_SUBJECTS];  // To store student records
    int size[MAX_STUDENTS];  // To track the number of students in each state
    int top;  // Index to track the top of the stack

    Stack() {
        top = -1;
    }

    bool isFull() {
        return top == MAX_STUDENTS - 1;
    }

    bool isEmpty() {
        return top == -1;
    }

    void push(string studs[][MAX_SUBJECTS], int curStuds) {
        if (isFull()) {
            cout << "Undo stack is full, cannot push more states!" << endl;
            return;
        }
        top++;
        for (int i = 0; i < curStuds; i++) {
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                records[i][j] = studs[i][j];  // Copy the current state
            }
        }
        size[top] = curStuds;
    }

    void pop(string studs[][MAX_SUBJECTS], int &curStuds) {
        if (isEmpty()) {
            cout << "No actions to undo!" << endl;
            return;
        }
        for (int i = 0; i < size[top]; i++) {
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                studs[i][j] = records[i][j];  // Restore the previous state
            }
        }
        curStuds = size[top];  // Restore the number of students
        top--;
    }
};

// Create an instance of the stack
Stack undoStack;

// Function declarations
void getGradeReport(string studs[][MAX_SUBJECTS], int &curStuds);
void CalculateGrade(float results[], string stud[], int &curStuds, string studs[][MAX_SUBJECTS]);
void saveRecord(string studs[][MAX_SUBJECTS], int curStuds);
void readRecord(int &curStuds, string studs[][MAX_SUBJECTS]);
void splitString(string &line, string studs[][MAX_SUBJECTS], int i);
void trimString(string &str);  // Helper function to trim white spaces

int main() {
    float results[6] = {0, 0, 0, 0, 0, 0};
    int opt;
    int currentStuds = 0;
    bool loop = true;
    string student[MAX_SUBJECTS];
    string students[MAX_STUDENTS][MAX_SUBJECTS];

    while (loop) {
        readRecord(currentStuds, students);  // Load the data from file at the start
        system("cls");  // Clear the screen (use cautiously on non-Windows platforms)
        cout << "*********************** STUDENT GRADING SYSTEM ***************************" << endl;
        cout << "1. Get Grade Report\n2. Calculate Grade\n3. Undo Last Action\n4. Exit";
        cout << "\nPlease Enter your option: ";
        cin >> opt;

        switch (opt) {
            case 1:
                getGradeReport(students, currentStuds);
                break;
            case 2:
                undoStack.push(students, currentStuds);  // Push current state before calculating grade
                CalculateGrade(results, student, currentStuds, students);
                break;
            case 3:
                undoStack.pop(students, currentStuds);  // Undo last action
                saveRecord(students, currentStuds);  // Save the restored state
                cout << "Last action undone successfully!\n";
                system("pause");
                break;
            case 4:
                loop = false;
                break;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }
}

void CalculateGrade(float results[], string stud[], int &curStuds, string studs[][MAX_SUBJECTS]) {
    string firstName, lastName;
    system("cls");
    cout << "# Notice: This Menu helps you calculate a Student's Grade\n" << endl;
    cout << "\nFirst Name: ";
    cin >> firstName;
    cout << "Last Name: ";
    cin >> lastName;
    stud[0] = firstName + " " + lastName;
    cout << "ID: ";
    cin >> stud[1];
    cout << "Section: ";
    cin >> stud[2];

    // Input subject results for DSA, DBMS, DECO, DM, JAVA, WEB DEVELOPMENT
    string subjects[] = {"DSA", "DBMS", "DECO", "DM", "JAVA", "WEB DEVELOPMENT"};
    float totalMarks = 0.0;
    bool passedAll = true;

    for (int i = 0; i < 6; i++) {
        cout << "Enter Result for " << subjects[i] << ": ";
        cin >> results[i];

        // Check if the entered score is valid
        while (results[i] < 0 || results[i] > 100) {
            cout << "Invalid input! Please enter a valid score between 0 and 100 for " << subjects[i] << ": ";
            cin >> results[i];
        }

        stud[i + 3] = to_string(results[i]);  // Store results in the student array, first three indexes are for Non grade info
        totalMarks += results[i];

        // check if student passed in each subject
        if (results[i] < 40) {   // check pass condition
            passedAll = false;
        }
    }

    float average = totalMarks / 6; // Calculate average

    // Copy student's data into the main records array
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        studs[curStuds][i] = stud[i];
    }

    // Add total marks, average, and status to the record
    studs[curStuds][9] = to_string(totalMarks);     // Total marks
    studs[curStuds][10] = to_string(average);        // Average
    studs[curStuds][11] = passedAll ? "Pass" : "Fail"; // Status
    curStuds++;  // Increment the student count
    saveRecord(studs, curStuds);  // Save the updated record
}

void getGradeReport(string studs[][MAX_SUBJECTS], int &curStuds) {
    string subjects[] = {"DSA", "DBMS", "DECO", "DM", "JAVA", "WEB DEVELOPMENT"};
    int reportType;
    cout << "Choose report type:\n1. Individual Student Report (by ID)\n2. Class Report (by Section)" << endl;
    cin >> reportType;

    if (reportType == 1) {
        // Student report by ID
        string id;
        cout << "Enter Student ID: ";
        cin >> id;
        trimString(id);  // Trim the input ID to remove any extra white spaces

        bool found = false;
        for (int i = 0; i < curStuds; i++) {
            string storedId = studs[i][1];
            trimString(storedId);  // Trim the stored ID

            if (id == storedId) {
                cout << "\nName: " << studs[i][0] << endl;
                cout << "ID: " << studs[i][1] << endl;
                cout << "Section: " << studs[i][2] << "\n" << endl;
                cout << "******* RESULT *******\n" << endl;

                for (int j = 3; j < 9; j++) {
                    cout << subjects[j - 3] << ": " << studs[i][j] << endl;
                }

                // **Display Total Marks, Average, and Status with Titles**
                cout << "Total Marks: " << studs[i][9] << endl;     // Total Marks
                cout << "Average Marks: " << studs[i][10] << endl;  // Average
                cout << "Status: " << studs[i][11] << endl;
                
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "Student ID not found!\n" << endl;
        }

    } else if (reportType == 2) {
        // Class report by section
        string section;
        cout << "Enter Section: ";
        cin >> section;
        trimString(section);  // Trim the input section

        bool found = false;
        for (int i = 0; i < curStuds; i++) {
            string storedSection = studs[i][2];
            trimString(storedSection);  // Trim the stored section

            if (section == storedSection) {
                cout << "\nName: " << studs[i][0] << endl;
                cout << "ID: " << studs[i][1] << endl;
                cout << "Section: " << studs[i][2] << "\n" << endl;
                cout << "******* RESULT *******\n" << endl;

                for (int j = 3; j < 9; j++) {
                    cout << subjects[j - 3] << ": " << studs[i][j] << endl;
                }

                cout << "Total Marks: " << studs[i][9] << endl;     // Total Marks
                cout << "Average Marks: " << studs[i][10] << endl;  // Average
                cout << "Status: " << studs[i][11] << endl;
                
                found = true;
            }
        }

        if (!found) {
            cout << "No records found for the entered section!\n" << endl;
        }
    } else {
        cout << "Invalid report type!" << endl;
    }
    system("pause");
}

void saveRecord(string studs[][MAX_SUBJECTS], int curStuds) {
    fstream file("records.txt", ios::out);
    for (int i = 0; i < curStuds; i++) {
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            file << studs[i][j] << "\t";
        }
        file << "\n";
    }
    file.close();
}

void readRecord(int &curStuds, string studs[][MAX_SUBJECTS]) {
    fstream file("records.txt", ios::in);
    string line;
    curStuds = 0;

    while (getline(file, line)) {
        splitString(line, studs, curStuds);
        curStuds++;
    }
    file.close();
}

void splitString(string &line, string studs[][MAX_SUBJECTS], int i) {
    size_t pos = 0;
    string token;
    int j = 0;

    while ((pos = line.find("\t")) != string::npos) {
        token = line.substr(0, pos);
        studs[i][j++] = token;
        line.erase(0, pos + 1);
    }
}

void trimString(string &str) {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
}

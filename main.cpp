#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>  // For trimming white spaces

using namespace std;

// Constants for array sizes
const int MAX_STUDENTS = 200;
const int MAX_SUBJECTS = 12; // Name, ID, Section + 6 Subjects

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
        cout << "1. Get Grade Report\n2. Calculate Grade\n3. Exit";
        cout << "\nPlease Enter your option: ";
        cin >> opt;

        switch (opt) {
            case 1:
                getGradeReport(students, currentStuds);
                break;
            case 2:
                CalculateGrade(results, student, currentStuds, students);
                break;
            case 3:
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

         // **Input Validation**: Check if the entered score is valid
        while (results[i] < 0 || results[i] > 100) {
            cout << "Invalid input! Please enter a valid score between 0 and 100 for " << subjects[i] << ": ";
            cin >> results[i];
        }


        stud[i + 3] = to_string(results[i]);  // Store results in the student array
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

                cout << "Total Marks: " << studs[i][9] << endl; // ** Display Total Marks
                cout << "Average Marks: " << studs[i][10] << endl; // ** Display Average Marks
                cout << "Status: " << studs[i][11] << endl; // ** Display Pass/Fail Status
                
                found = true;
            }
        }

        if (!found) {
            cout << "No students found in section " << section << "!\n" << endl;
        }

    } else {
        cout << "Invalid option!\n";
    }

    system("pause");
}

void saveRecord(string studs[][MAX_SUBJECTS], int curStuds) {
    ofstream save;
    save.open("C:\\Users\\Niraj\\Desktop\\DBMS\\records.txt");

    if (save.is_open()) {
        for (int i = 0; i < curStuds; i++) {
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                save << studs[i][j];
                if (j < MAX_SUBJECTS - 1) save << ",";  // Add comma between entries
            }
            save << endl;  // New line after each student
        }
        save.close();
    } else {
        cout << "Error: Unable to open file for saving!" << endl;
    }
}


void readRecord(int &curStuds, string studs[][MAX_SUBJECTS]) {
    ifstream read("C:\\Users\\Niraj\\Desktop\\DBMS\\records.txt");

    string line;
    curStuds = 0;

    if (read.is_open()) {
        curStuds = 0;
        while (getline(read, line)) {
            splitString(line, studs, curStuds);
            curStuds++;
        }
        read.close();
    } else {
        cout << "Error: Unable to open file for reading!" << endl;
    }
    read.close();
}


void splitString(string &line, string studs[][MAX_SUBJECTS], int i) {
    string word = "";
    int x = 0;

    for (char c : line) {
        if (c == ',') {  // Split by comma instead of space
            studs[i][x++] = word;
            word = "";
        } else {
            word += c;
        }
    }
    studs[i][x] = word;  // Last word
}


// Helper function to trim leading and trailing spaces from a string
void trimString(string &str) {
    // Remove leading spaces
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch);
    }));

    // Remove trailing spaces
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch);
    }).base(), str.end());
}
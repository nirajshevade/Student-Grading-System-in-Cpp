import tkinter as tk
from tkinter import messagebox, ttk

# Constants
SUBJECTS = ["DSA", "DBMS", "DECO", "DM", "JAVA", "WEB DEVELOPMENT"]
MAX_SUBJECTS = 12
MAX_STUDENTS = 200

class StudentGradingSystem:
    def __init__(self, root):
        self.root = root
        self.root.title("Student Grading System")
        self.root.geometry("800x600")
        self.root.config(bg="#2c3e50")
        
        # Title label with new style
        title_label = tk.Label(
            self.root, 
            text="STUDENT GRADING SYSTEM", 
            font=("Arial", 24, "bold", ), 
            fg="white", 
            bg="#34495e", 
            pady=20
        )
        title_label.pack(fill="x")
        
        self.students = [[""] * MAX_SUBJECTS for _ in range(MAX_STUDENTS)]
        self.current_students = 0
        self.results = [0] * 6
        
        self.main_menu()

    def main_menu(self):
        menu_frame = tk.Frame(self.root, bg="#2c3e50")
        menu_frame.pack(pady=50)

        btn_style = {
            "font": ("Arial", 14), 
            "bg": "#1abc9c", 
            "fg": "white", 
            "width": 20, 
            "height": 2,
            "relief": "raised"
        }

        # Buttons for menu options
        grade_report_btn = tk.Button(menu_frame, text="Get Grade Report", command=self.get_grade_report, **btn_style)
        grade_report_btn.pack(pady=10)
        
        calculate_grade_btn = tk.Button(menu_frame, text="Calculate Grade", command=self.calculate_grade, **btn_style)
        calculate_grade_btn.pack(pady=10)
        
        exit_btn = tk.Button(menu_frame, text="Exit", command=self.root.quit, **btn_style)
        exit_btn.pack(pady=10)
    
    def calculate_grade(self):
        # New window for grade calculation
        calc_window = tk.Toplevel(self.root)
        calc_window.title("Calculate Grade")
        calc_window.geometry("600x500")
        calc_window.config(bg="#34495e")

        # Labels and entry fields
        input_style = {"font": ("Arial", 12), "bg": "#ecf0f1"}
        label_style = {"font": ("Arial", 14), "fg": "white", "bg": "#34495e"}
        
        tk.Label(calc_window, text="First Name:", **label_style).grid(row=0, column=0, padx=10, pady=10)
        self.first_name_entry = tk.Entry(calc_window, **input_style)
        self.first_name_entry.grid(row=0, column=1, padx=10, pady=10)
        
        tk.Label(calc_window, text="Last Name:", **label_style).grid(row=1, column=0, padx=10, pady=10)
        self.last_name_entry = tk.Entry(calc_window, **input_style)
        self.last_name_entry.grid(row=1, column=1, padx=10, pady=10)

        tk.Label(calc_window, text="ID:", **label_style).grid(row=2, column=0, padx=10, pady=10)
        self.id_entry = tk.Entry(calc_window, **input_style)
        self.id_entry.grid(row=2, column=1, padx=10, pady=10)

        tk.Label(calc_window, text="Section:", **label_style).grid(row=3, column=0, padx=10, pady=10)
        self.section_entry = tk.Entry(calc_window, **input_style)
        self.section_entry.grid(row=3, column=1, padx=10, pady=10)

        self.subject_entries = []
        for i, subject in enumerate(SUBJECTS):
            tk.Label(calc_window, text=f"{subject}:", **label_style).grid(row=4+i, column=0, padx=10, pady=5)
            entry = tk.Entry(calc_window, **input_style)
            entry.grid(row=4+i, column=1, padx=10, pady=5)
            self.subject_entries.append(entry)

        # Submit button
        submit_btn = tk.Button(calc_window, text="Submit", command=self.save_grade, font=("Arial", 14), bg="#1abc9c", fg="white")
        submit_btn.grid(row=10, column=0, columnspan=2, pady=20)

    def save_grade(self):
        first_name = self.first_name_entry.get()
        last_name = self.last_name_entry.get()
        student_id = self.id_entry.get()
        section = self.section_entry.get()

        if not (first_name and last_name and student_id and section):
            messagebox.showerror("Input Error", "All fields must be filled!")
            return

        total_marks = 0
        passed_all = True
        for i, entry in enumerate(self.subject_entries):
            try:
                score = float(entry.get())
                if score < 0 or score > 100:
                    raise ValueError
            except ValueError:
                messagebox.showerror("Input Error", f"Invalid score for {SUBJECTS[i]}")
                return
            
            self.students[self.current_students][i+3] = str(score)
            total_marks += score
            if score < 40:
                passed_all = False
        
        average = total_marks / len(SUBJECTS)
        self.students[self.current_students][0] = first_name + " " + last_name
        self.students[self.current_students][1] = student_id
        self.students[self.current_students][2] = section
        self.students[self.current_students][9] = str(total_marks)
        self.students[self.current_students][10] = str(average)
        self.students[self.current_students][11] = "Pass" if passed_all else "Fail"
        
        self.current_students += 1
        messagebox.showinfo("Success", "Student Grade Calculated and Saved!")

    def get_grade_report(self):
        report_window = tk.Toplevel(self.root)
        report_window.title("Grade Report")
        report_window.geometry("600x500")
        report_window.config(bg="#34495e")

        report_type_label = tk.Label(report_window, text="Choose report type:", font=("Arial", 14), fg="white", bg="#34495e")
        report_type_label.pack(pady=10)

        report_type_var = tk.StringVar()

        individual_report_radio = tk.Radiobutton(report_window, text="Individual Student Report (by ID)", variable=report_type_var, value="individual", bg="#34495e", fg="white", font=("Arial", 12))
        individual_report_radio.pack(pady=5)

        class_report_radio = tk.Radiobutton(report_window, text="Class Report (by Section)", variable=report_type_var, value="class", bg="#34495e", fg="white", font=("Arial", 12))
        class_report_radio.pack(pady=5)

        id_entry_label = tk.Label(report_window, text="Enter Student ID:", font=("Arial", 12), fg="white", bg="#34495e")
        id_entry_label.pack(pady=10)
        self.id_entry_report = tk.Entry(report_window)
        self.id_entry_report.pack(pady=10)

        section_entry_label = tk.Label(report_window, text="Enter Section:", font=("Arial", 12), fg="white", bg="#34495e")
        section_entry_label.pack(pady=10)
        self.section_entry_report = tk.Entry(report_window)
        self.section_entry_report.pack(pady=10)

        submit_report_btn = tk.Button(report_window, text="Generate Report", command=lambda: self.generate_report(report_type_var.get()), font=("Arial", 14), bg="#1abc9c", fg="white")
        submit_report_btn.pack(pady=20)

    def generate_report(self, report_type):
        if report_type == "individual":
            student_id = self.id_entry_report.get().strip()
            found = False
            for student in self.students:
                if student[1].strip() == student_id:
                    report = f"Name: {student[0]}\nID: {student[1]}\nSection: {student[2]}\n"
                    report += "******* RESULT *******\n"
                    for i, subject in enumerate(SUBJECTS):
                        report += f"{subject}: {student[i+3]}\n"
                    report += f"Total Marks: {student[9]}\nAverage Marks: {student[10]}\nStatus: {student[11]}"
                    messagebox.showinfo("Student Report", report)
                    found = True
                    break
            if not found:
                messagebox.showerror("Error", "Student ID not found!")
        elif report_type == "class":
            section = self.section_entry_report.get().strip()
            found = False
            report = f"Class Report for Section: {section}\n\n"
            for student in self.students:
                if student[2].strip() == section:
                    report += f"Name: {student[0]}\nID: {student[1]}\n"
                    report += "******* RESULT *******\n"
                    for i, subject in enumerate(SUBJECTS):
                        report += f"{subject}: {student[i+3]}\n"
                    report += f"Total Marks: {student[9]}\nAverage Marks: {student[10]}\nStatus: {student[11]}\n\n"
                    found = True
            if found:
                messagebox.showinfo("Class Report", report)
            else:
                messagebox.showerror("Error", "No students found in this section!")

# Main application entry point
if __name__ == "__main__":
    root = tk.Tk()
    app = StudentGradingSystem(root)
    root.mainloop()

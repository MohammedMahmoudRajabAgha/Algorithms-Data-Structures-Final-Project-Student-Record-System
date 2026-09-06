#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

enum enChoice
{
    eAddStudentToQueue = 1,
    eProcessQueue,
    eAddGradeToStudent,
    eEditGrade,
    eDeleteGrade,
    eDisplayStudentInfo,
    eDisplayStudentGrade,
    eDisplayAllStudents,
    eSortStudents,
    eShowSearchHistory,
    eExit
};

class GradeNode {
public:
    double grade;
    GradeNode* next;

    GradeNode(double g) {
        grade = g;
        next = nullptr;
    }
};

class Student {
public:
    string name;
    string studentID;
    double GPA;
    int completedHours;
    GradeNode* gradesHead;
    Student* next;

    Student(string FullName, string ID) {
        name = FullName;
        studentID = ID;
        GPA = 0.0;
        completedHours = 0;
        gradesHead = nullptr;
        next = nullptr;
    }

    void calculateGPA()
    {
        int count = 0;
        double sum = 0;
        completedHours = 0;

        GradeNode* t = gradesHead;
        while (t)
        {
            double G = t->grade;
            double p = 0;

            if (G >= 90) p = 4.0;
            else if (G >= 85) p = 3.5;
            else if (G >= 80) p = 3.0;
            else if (G >= 75) p = 2.5;
            else if (G >= 70) p = 2.0;
            else if (G >= 60) p = 1.5;
            else if (G >= 50) p = 1.0;
            else p = 0;

            sum += p;
            count++;

            if (G >= 50)
                completedHours += 3;

            t = t->next;
        }

        GPA = (count > 0) ? sum / count : 0;
    }

    void addGrade(double Grade)
    {
        if (Grade < 0) Grade = 0;
        if (Grade > 100) Grade = 100;

        GradeNode* node = new GradeNode(Grade);

        if (!gradesHead)
            gradesHead = node;

        else
        {
            GradeNode* t = gradesHead;

            while (t->next)
            {
                t = t->next;
            }
            t->next = node;
        }

        calculateGPA();
    }

    void editGrade(int index, double NewGrade)
    {
        if (!gradesHead) {
            cout << "No grades available.\n";
            return;
        }

        GradeNode* t = gradesHead;
        int i = 1;

        while (t && i < index) {
            t = t->next;
            i++;
        }

        if (!t) {
            cout << "Invalid index.\n";
            return;
        }

        t->grade = NewGrade;
        calculateGPA();
    }

    void deleteGrade(int index)
    {
        if (!gradesHead) {
            cout << "No grades.\n";
            return;
        }

        if (index == 1)
        {
            GradeNode* del = gradesHead;
            gradesHead = gradesHead->next;
            delete del;
            calculateGPA();
            return;
        }

        GradeNode* t = gradesHead;
        int i = 1;

        while (t->next && i < index - 1)
        {
            t = t->next;
            i++;
        }

        if (!t->next) {
            cout << "Invalid index.\n";
            return;
        }

        GradeNode* del = t->next;
        t->next = del->next;
        delete del;

        calculateGPA();
    }

    void displayGrades()
    {
        if (!gradesHead) {
            cout << "No grades.\n";
            return;
        }

        GradeNode* t = gradesHead;
        int i = 1;

        while (t)
        {
            cout << i++ << ") " << t->grade << endl;
            t = t->next;
        }
    }

    int getGradeCount()
    {
        int count = 0;
        GradeNode* t = gradesHead;
        while (t)
        {
            count++;
            t = t->next;
        }
        return count;
    }

    ~Student()
    {
        GradeNode* g = gradesHead;
        while (g)
        {
            GradeNode* temp = g;
            g = g->next;
            delete temp;
        }
    }
};

class Queue {
public:
    Student* front;
    Student* rear;
    short size;

    Queue() {
        front = rear = nullptr;
        size = 0;
    }

    bool is_Empty()
    {
        return front == nullptr;
    }

    void enqueue(const string& name, const string& id)
    {
        Student* S = new Student(name, id);

        if (!rear)
            front = rear = S;
        else {
            rear->next = S;
            rear = S;
        }
        size++;
    }

    Student* dequeue()
    {
        if (!front) return nullptr;

        Student* t = front;
        front = front->next;

        if (!front)
            rear = nullptr;

        t->next = nullptr;

        size--;

        return t;
    }

    ~Queue()
    {
        while (front)
        {
            Student* temp = front;
            front = front->next;
            delete temp;
        }
    }
};

class HistoryNode {
public:
    string id, op;
    HistoryNode* next;

    HistoryNode(string i, string o) {
        id = i;
        op = o;
        next = nullptr;
    }
};

class HistoryStack {
public:
    HistoryNode* top;
    short count;

    HistoryStack() {
        top = nullptr;
        count = 0;
    }

    void push(string id, string op)
    {
        HistoryNode* n = new HistoryNode(id, op);
        n->next = top;
        top = n;
        count++;
    }

    void display()
    {
        HistoryNode* t = top;

        while (t)
        {
            cout << t->id << " - " << t->op << endl;
            t = t->next;
        }
        cout << "Total: " << count << endl;
    }

    ~HistoryStack()
    {
        HistoryNode* h = top;
        while (h)
        {
            HistoryNode* temp = h;
            h = h->next;
            delete temp;
        }
    }
};

class StudentManager {
public:
    Student* head;

    StudentManager() {
        head = nullptr;
    }

    bool exists(const string& id)
    {
        Student* t = head;
        while (t)
        {
            if (t->studentID == id)
                return true;
            t = t->next;
        }
        return false;
    }

    void insertStudent(Student* s)
    {
        s->next = nullptr; // مهم جداً

        if (!head) {
            head = s;
            return;
        }

        Student* t = head;
        while (t->next)
            t = t->next;

        t->next = s;
    }

    Student* findStudent(const string& id)
    {
        Student* t = head;
        while (t)
        {
            if (t->studentID == id)
                return t;
            t = t->next;
        }
        return nullptr;
    }

    void displayAll()
    {
        Student* t = head;
        int i = 1;

        cout << "___________________________________________________________________\n";
        cout << left << setw(20) << "Full Name" << "|"
            << left << setw(10) << "ID" << "|"
            << left << setw(10) << "GPA" << "|"
            << left << setw(15) << "Total Hours" << "|"
            << left << setw(10) << "Grades Count" << endl;
        cout << "___________________________________________________________________\n";

        while (t)
        {
            cout << left << setw(20) << t->name
                << "|" << left << setw(10) << t->studentID
                << "|" << left << setw(10) << fixed << setprecision(2) << t->GPA
                << "|" << left << setw(15) << t->completedHours
                << "|" << left << setw(10) << t->getGradeCount() << endl;

            t = t->next;
        }
        cout << "___________________________________________________________________\n";
    }

    void sortByGPA()
    {
        if (!head || !head->next) return;

        bool swapped;
        do {
            swapped = false;
            Student* curr = head;
            Student* prev = nullptr;

            while (curr->next)
            {
                if (curr->GPA < curr->next->GPA)
                {
                    Student* nxt = curr->next;

                    curr->next = nxt->next;
                    nxt->next = curr;

                    if (!prev)
                        head = nxt;
                    else
                        prev->next = nxt;

                    swapped = true;
                    prev = nxt;
                }
                else {
                    prev = curr;
                    curr = curr->next;
                }
            }
        } while (swapped);
    }

    void sortByHours()
    {
        if (!head || !head->next) return;

        bool swapped;

        do {
            swapped = false;
            Student* curr = head;
            Student* prev = nullptr;

            while (curr->next)
            {
                if (curr->completedHours < curr->next->completedHours)
                {
                    Student* nxt = curr->next;

                    curr->next = nxt->next;
                    nxt->next = curr;

                    if (!prev)
                        head = nxt;
                    else
                        prev->next = nxt;

                    swapped = true;
                    prev = nxt;
                }
                else {
                    prev = curr;
                    curr = curr->next;
                }
            }
        } while (swapped);
    }

    ~StudentManager()
    {
        Student* s = head;
        while (s)
        {
            Student* temp = s;
            s = s->next;
            delete temp;
        }
    }
};

void safeInput(double& x)
{
    while (!(cin >> x))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter again: ";
    }
}

void safeInput(int& x)
{
    cout << "Please enter Your Choice : [1 -> 11] : ";

    while (!(cin >> x))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter again: ";
    }
}



void AddStudentToQueue(Queue& queue)
{
    string FullName, id;


    while (true)
    {
        cout << "Name: ";
        getline(cin >> ws, FullName);

        if (!FullName.empty())
            break;
        cout << "Error: Name cannot be empty. Please enter a valid name.\n";
    }


    while (true)
    {
        cout << "ID: ";
        getline(cin >> ws, id);

        if (!id.empty())
            break;
        cout << "Error: Student ID cannot be empty. Please enter a valid ID.\n";
    }

    queue.enqueue(FullName, id);
    cout << "Added to queue. Queue size: " << queue.size << endl;
}

void ProcessQueue(Queue& queue, StudentManager& SM)
{
    system("cls");
    cout << "\t\t=== Processing Waiting Queue ===\n\n";

    int transferred = 0;
    int skipped = 0;

    while (!queue.is_Empty()) {
        Student* s = queue.dequeue();

        if (SM.exists(s->studentID)) {
            cout << "Skipped (Duplicate ID): "
                << s->studentID << " - "
                << s->name << "\n";

            delete s;
            skipped++;
            continue;
        }

        s->next = nullptr;

        SM.insertStudent(s);

        cout << "Transferred: "
            << s->studentID << " - "
            << s->name << "\n";

        transferred++;
    }

    cout << "\n=======================================\n";
    cout << "Students Transferred : " << transferred << "\n";
    cout << "Duplicates Skipped   : " << skipped << "\n";
    cout << "=======================================\n";

}

void AddGradeToStudent(HistoryStack& HS, StudentManager& SM)
{
    string id;
    cout << "Student ID: ";
    cin >> id;

    Student* s = SM.findStudent(id);
    if (!s)
    {
        cout << "Not found.\n";
        return;
    }

    double Grade;
    cout << "Grade: ";
    safeInput(Grade);

    HS.push(id, "Add Grade");
    s->addGrade(Grade);

    cout << "Grade added.\n";
}

void EditGrade(HistoryStack& HS, StudentManager& SM)
{
    string id;
    cout << "Student ID: ";
    cin >> id;

    Student* s = SM.findStudent(id);
    if (!s)
    {
        cout << "Not found.\n";
        return;
    }

    s->displayGrades();

    int Index;
    double Grade;

    cout << "Index: ";
    safeInput(Index);

    cout << "New grade: ";
    safeInput(Grade);

    HS.push(id, "Edit Grade");
    s->editGrade(Index, Grade);
}

void DeleteGrade(HistoryStack& HS, StudentManager& SM)
{
    string id;
    cout << "Student ID: ";
    cin >> id;

    Student* s = SM.findStudent(id);
    if (!s)
    {
        cout << "Not found.\n";
        return;
    }

    s->displayGrades();

    int Index;
    cout << "Index: ";
    safeInput(Index);

    HS.push(id, "Delete Grade");
    s->deleteGrade(Index);
}

void DisplayStudentInfo(HistoryStack& HS, StudentManager& SM)
{
    string id;
    cout << "Student ID: ";
    cin >> id;

    Student* s = SM.findStudent(id);
    if (!s)
    {
        cout << "Error: Student not found.\n";
        return;
    }

    HS.push(id, "Display Info");

    cout << "\n=======================================\n";
    cout << "Name: " << s->name << endl
        << "ID: " << s->studentID << endl
        << "GPA: " << fixed << setprecision(2) << s->GPA << endl
        << "Completed Hours: " << s->completedHours << endl
        << "Number of Grades: " << s->getGradeCount() << endl;
    cout << "=======================================\n";
}

void DisplayStudentGrade(HistoryStack& HS, StudentManager& SM)
{
    string id;
    cout << "Student ID: ";
    cin >> id;

    Student* s = SM.findStudent(id);
    if (!s)
    {
        cout << "Not found.\n";
        return;
    }

    HS.push(id, "Show Grades");
    s->displayGrades();
}

void DisplayAllStudents(StudentManager& SM)
{
    SM.displayAll();
}

void SortStudents(StudentManager& SM)
{
    short Choice;

    cout << "1) Sort by Hours\n";
    cout << "2) Sort by GPA\n";
    cin >> Choice;

    if (Choice == 1)
        SM.sortByHours();
    else
        SM.sortByGPA();
}

void ShowSearchHistory(HistoryStack& HS)
{
    HS.display();
}

void ShowEndScreen()
{
    cout << "\n=================================\n";
    cout << "\tGood Bye!\n";
    cout << "=================================\n";
}

void MainMenueScreen()
{
    cout << "\n===== Student Record System =====\n";
    cout << "1) Add Student to Queue\n";
    cout << "2) Process Queue\n";
    cout << "3) Add Grade to Student\n";
    cout << "4) Edit Grade\n";
    cout << "5) Delete Grade\n";
    cout << "6) Display Student Info\n";
    cout << "7) Display Student Grades\n";
    cout << "8) Display All Students\n";
    cout << "9) Sort Students\n";
    cout << "10) Show Search History\n";
    cout << "11) Exit\n";
    cout << "=================================\n";
}

int main()
{
    StudentManager studentManager;
    Queue queue;
    HistoryStack historyStack;

    while (true)
    {
        system("cls");
        MainMenueScreen();

        int choiceInt;
        safeInput(choiceInt);
        enChoice choice = enChoice(choiceInt);

        if (choice == eExit)
        {
            system("cls");
            ShowEndScreen();
            break;
        }

        system("cls");

        switch (choice)
        {
        case eAddStudentToQueue:
            AddStudentToQueue(queue);
            break;

        case eProcessQueue:
            ProcessQueue(queue, studentManager);
            break;

        case eAddGradeToStudent:
            AddGradeToStudent(historyStack, studentManager);
            break;

        case eEditGrade:
            EditGrade(historyStack, studentManager);
            break;

        case eDeleteGrade:
            DeleteGrade(historyStack, studentManager);
            break;

        case eDisplayStudentInfo:
            DisplayStudentInfo(historyStack, studentManager);
            break;

        case eDisplayStudentGrade:
            DisplayStudentGrade(historyStack, studentManager);
            break;

        case eDisplayAllStudents:
            DisplayAllStudents(studentManager);
            break;

        case eSortStudents:
            SortStudents(studentManager);
            break;

        case eShowSearchHistory:
            ShowSearchHistory(historyStack);
            break;
        }

        cout << "\nPress any key to continue...\n";
        system("pause>0");
    }

    return 0;
}

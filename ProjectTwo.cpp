#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Class with its number title prerequisites
class Course {
public:
    string number;// Course number
    string title;// Course title
    vector<string> prereqs; // List prereqs

    //initialize course object
    Course(string num, string t, vector<string> p)
        : number(num),
        title(t), 
        prereqs(p) {}
};

// node in the binary search tree
class treeNode {
public:
    Course course;// data stored in the node
    treeNode* leftChild;// pointer left child
    treeNode* rightChild;// pointer right child

    // initialize a treeNode object
    treeNode(const Course& c)
        : course(c), 
        leftChild(nullptr),
        rightChild(nullptr) {}
};

// binary search tree of courses
class CourseTree {
public:
    treeNode* root;//root of the tree

    //initialize the tree with null
    CourseTree() : root(nullptr) {}

    // Insert course into the tree
    void insertCourse(const Course& course) {
        root = insertCourseRec(root, course);
    }

    // Print list of courses inorder traversal
    void printCourseList() const {
        printCourseListRec(root);
    }

    // return a node with course number
    treeNode* findCourse(const string& courseNum) const {
        return findCourseRec(root, courseNum);
    }

private:
    // function to insertcourse into the tree
    treeNode* insertCourseRec(treeNode* node, const Course& course) {
        //node is null new node with the course
        if (!node) {
            return new treeNode(course);
        }

        //into the left or right subtree based on course number
        if (course.number < node->course.number) {
            node->leftChild = insertCourseRec(node->leftChild, course);
        }
        else if (course.number > node->course.number) {
            node->rightChild = insertCourseRec(node->rightChild, course);
        }

        return node;
    }

    //function to print the list of courses
    void printCourseListRec(treeNode* node) const {
        if (node) {
            // Traverse the left subtree print the current node then traverse the right subtree
            printCourseListRec(node->leftChild);
            cout << node->course.number << ", " << node->course.title << endl;
            printCourseListRec(node->rightChild);
        }
    }

    //function to find a course by its number
    treeNode* findCourseRec(treeNode* node, const string& courseNum) const {
        //null if node is null or course number matches
        if (!node || node->course.number == courseNum) {
            return node;
        }

        //left and right search subtree based on the course number
        if (courseNum < node->course.number) {
            return findCourseRec(node->leftChild, courseNum);
        }
        else {
            return findCourseRec(node->rightChild, courseNum);
        }
    }
};

//load courses from a file insert into course tree
void loadCoursesFromFile(const string& fileName, CourseTree& courseTree) {
    ifstream file(fileName);

    // Check if the file was opened 
    if (!file.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    // read each line from file
    while (getline(file, line)) {
        vector<string> tokens;
        size_t start = 0, end;

        // Split the line by commas for course information
        while ((end = line.find(',', start)) != string::npos) {
            tokens.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(line.substr(start)); //add last tokenn

        // has number and title in line
        if (tokens.size() < 2) {
            cout << "Error: Invalid data format in line: " << line << endl;
            continue;
        }

        // get course number title prereq
        string courseNum = tokens[0];
        string courseTitle = tokens[1];
        vector<string> prereqs(tokens.begin() + 2, tokens.end());

        // create Course object and put into tree
        Course course(courseNum, courseTitle, prereqs);
        courseTree.insertCourse(course);
    }

    file.close(); // close file
}

//main menu
void Menu(CourseTree& courseTree) {
    int choice;
    while (true) {
        // menu options
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        if (choice == 1) {          
            cout << "Data structure loaded successfully." << endl;
        }
        else if (choice == 2) {
            cout << "Course List:" << endl;
            courseTree.printCourseList();
        }
        else if (choice == 3) {
            string courseNum;
            cout << "What course do you want to know about? ";
            cin >> courseNum;
            treeNode* node = courseTree.findCourse(courseNum);
            if (node) {
                // Print course details if found
                cout << node->course.number << ", " << node->course.title << endl;
                cout << "Prerequisites: ";
                for (const string& prereq : node->course.prereqs) {
                    cout << prereq << " ";
                }
                cout << endl;
            }
            else {
                //error msg
                cout << "Course not found." << endl;
            }
        }
        else if (choice == 9) {
            //exit
            cout << "Thank you for using the course planner!" << endl;
            break;
        }
        else {
            // invaldi chocies
            cout << "This is not a valid option." << endl;
        }
    }
}

int main() {
    CourseTree courseTree;   
    string fileName;         

    //enter the file name
    cout << "Enter the name of the file ";
    cin >> fileName;

    // Load courses from fileinsert into tree
    loadCoursesFromFile(fileName, courseTree);

    //menu for user interaction
    Menu(courseTree);

    return 0;
}

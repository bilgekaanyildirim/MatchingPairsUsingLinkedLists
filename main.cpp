//Bilge Kaan Yildirim 30607 CS204 HW2

#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#else
#include <mach-o/dyld.h>
#endif

using namespace std;

// Function to get the directory where the executable is running
string getExecutableDirectory()
{
    char path[PATH_MAX];

#ifdef _WIN32
    GetModuleFileNameA(NULL, path, PATH_MAX);
#else
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0)
    {
        cerr << "Error: Buffer too small!" << endl;
        return "";
    }
#endif

    string fullPath(path);
    size_t lastSlash = fullPath.find_last_of("/\\");
    return fullPath.substr(0, lastSlash); // Extract directory path
}

// Node for holding names of liker and liked ones for a linked list
struct NodeLike
{
    string subject;
    string object;
    NodeLike *next;

    // Default constructor
    NodeLike()
    {
        subject = "";
        object = "";
        next = nullptr;
    }

    // Constructor
    NodeLike (string subj, string obj, NodeLike *n)
    {
        subject = subj;
        object = obj;
        next = n;
    }
};

// Node for holding name of matched ones for a linked list
struct NodeTaken
{
    string name;
    NodeTaken *next;

    // Default constructor
    NodeTaken()
    {
        name = "";
        next = nullptr;
    }

    // Constructor
    NodeTaken (string na, NodeTaken *n)
    {
        name = na;
        next = n;
    }
};

bool AreInTaken(const string &subject, const string &object, NodeTaken *headTaken);

bool IsPairInLike(const string &subject, const string &object, NodeLike *headLike);

bool IsMatched(const string &subject, const string &object, NodeLike *headLike);

bool IsSubjExistInLike(const string &subject, NodeLike *headLike);

void DeleteNodesInLikes(const string &subject, const string &object, NodeLike *&headLike);

void DeleteOneNode(NodeLike * toBeDeleted, NodeLike * & head);

void InsertTaken(const string &name, NodeTaken *&headTaken);

void InsertLikeAccordingSubject(const string &subject, const string &object, NodeLike *&headLike);

void InsertLikeToEnd(const string &subject, const string &object, NodeLike *&headLike);

void PrintLikeList(NodeLike *headLike);

void PrintTakenList(NodeTaken *headTaken);

void DeleteLikeList(NodeLike *headLike);

void DeleteTakenList(NodeTaken *headTaken);

void DeleteTakenList(NodeTaken *headTaken)
{
    NodeTaken *ptr;

    while (headTaken != nullptr)
    {
        ptr = headTaken;
        headTaken = headTaken->next;
        delete ptr;
    }
}

void DeleteLikeList(NodeLike *headLike)
{
    NodeLike *ptr;

    while(headLike != nullptr)
    {
        ptr = headLike;
        headLike = headLike->next;
        delete ptr;
    }
}

void PrintLikeList(NodeLike *headLike)
{
    cout << "Likes list:";

    while (headLike != nullptr)
    {
        cout << " (" << headLike->subject << ", " << headLike->object << ")";
        headLike = headLike->next;
    }
    cout << endl;
}

void PrintTakenList(NodeTaken *headTaken)
{
    cout << "Taken list:";

    while (headTaken != nullptr)
    {
        cout << " " << headTaken->name;
        headTaken = headTaken->next;
    }
    cout << endl;
}

// Checks subject or the object in the taken linked list
bool AreInTaken(const string &subject, const string &object, NodeTaken *headTaken)
{
    while (headTaken != nullptr)
    {
        if(headTaken->name == subject || headTaken->name == object)
        {
            cout << "Either " << subject << " or " << object << " or both is/are already taken, so the like relation is not added." << endl;
            return true;
        }
        headTaken = headTaken->next;
    }
    return false;
}

// Checks subject-object pair is in the likes linked list
bool IsPairInLike(const string &subject, const string &object, NodeLike *headLike)
{
    while (headLike != nullptr)
    {
        if (headLike->subject == subject && headLike->object == object)
        {
            cout << subject << " likes " << object << " relation already exists in the likes list, so it is not added to it." << endl;
            return true;
        }
        headLike = headLike->next;
    }
    return false;
}

// Checks is there a match
bool IsMatched(const string &subject, const string &object, NodeLike *headLike)
{
    while (headLike != nullptr)
    {
        if (headLike->subject == object && headLike->object == subject)
        {
            cout << "Match found: " << subject << " likes " << object << " and " << object << " likes " << subject << "." << endl;
            return true;
        }
        headLike = headLike->next;
    }
    return false;
}

// Checks subject exists in at least one node in the likes linked list as subject
bool IsSubjExistInLike(const string &subject, NodeLike *headLike)
{
    while (headLike != nullptr)
    {
        if (headLike->subject == subject)
        {
            return true;
        }
        headLike = headLike->next;
    }
    return false;
}

/* Begin: code taken from 2.2-pointers-linkedlist.pptx and updated */
void DeleteOneNode (NodeLike * toBeDeleted, NodeLike *&head)
/* pre: toBeDeleted points to the node to be deleted from the list
   post: deletes the node pointed by toBeDeleted, updates head if changes */
{
    NodeLike *ptr;
    if (toBeDeleted == head)  //if the node to be deleted is the first node
    {     head = head->next;
        delete toBeDeleted;
    }
    else  //if the node to be deleted is in the middle or at the end
    {     ptr = head;
        while (ptr->next != toBeDeleted)
            ptr = ptr->next;
        //after while, ptr points to the node just before toBeDeleted

        //connect the previous node to the next node and delete
        ptr->next = toBeDeleted->next;
        delete toBeDeleted;
    }
}
/* End: code taken from 2.2-pointers-linkedlist.pptx and updated */

// Deletes each node in likes linked list which includes subject or object
void DeleteNodesInLikes(const string &subject, const string &object, NodeLike *&headLike)
{
    NodeLike *ptr;
    ptr = headLike;

    NodeLike *toBeDeleted = nullptr;

    while (ptr != nullptr)
    {
        if (ptr->subject == subject || ptr->subject == object || ptr->object == subject || ptr->object == object)
        {
            toBeDeleted = ptr;
            DeleteOneNode(toBeDeleted, headLike);
        }
        ptr = ptr->next;
    }
    cout << "Any node that has " << subject << " or " << object << " or both in it is removed from the likes list." << endl;
}

// Inserts name to taken list in ascending order
void InsertTaken(const string &name, NodeTaken *&headTaken)
{
    if (headTaken == nullptr)
    {
        NodeTaken *ptr = new NodeTaken(name, nullptr);
        headTaken = ptr;
        headTaken->next = nullptr;
    }
    else
    {
        if (headTaken->name > name)
        {
            NodeTaken *ptr = new NodeTaken(name, headTaken);
            headTaken = ptr;
        }
        else
        {
            NodeTaken *ptr = headTaken;

            while (ptr->next != nullptr && ptr->next->name < name)
            {
                ptr = ptr->next;
            }
            NodeTaken *tempPtr = new NodeTaken(name, ptr->next);
            ptr->next = tempPtr;
        }
    }
    cout << "Added to taken list: " << name << endl;
    //PrintTakenList(headTaken);
}

// Inserts new NodeLike after the last node in Like linked list that contains subject’s name in its subject field
void InsertLikeAccordingSubject(const string &subject, const string &object, NodeLike *&headLike)
{
    NodeLike *ptr = headLike;

    while (ptr->next != nullptr)
    {
        if (ptr->subject == subject && ptr->next->subject != subject)
        {
            NodeLike *tempPtr = new NodeLike(subject, object, ptr->next);
            ptr->next = tempPtr;
            cout << subject << " likes " << object << " relation has been added to the likes list." << endl;
            return;
        }
        ptr = ptr->next;
    }
    NodeLike *tempPtr = new NodeLike(subject, object, ptr->next);
    ptr->next = tempPtr;
    cout << subject << " likes " << object << " relation has been added to the likes list." << endl;
}

void InsertLikeToEnd(const string &subject, const string &object, NodeLike *&headLike)
{
    if (headLike == nullptr)
    {
        NodeLike *tempPtr = new NodeLike(subject, object, headLike);
        headLike = tempPtr;
    }
    else
    {
        NodeLike *ptr = headLike;

        while (ptr->next != nullptr)
        {
            ptr = ptr->next;
        }
        NodeLike *tempPtr = new NodeLike(subject, object, nullptr);
        ptr->next = tempPtr;
    }
    cout << subject << " likes " << object << " relation has been added to the likes list." << endl;
}

int main()
{
    string filename, line;

    const string MID_WORD = "likes";

    string execDir = getExecutableDirectory();

    if (execDir.empty())
    {
        cerr << "Error: Could not determine executable directory!" << endl;
        return 1;
    }

    // For debug
    //cout << "Executable is running from: " << execDir << endl;

    cout << "Please enter the file name:" << endl;
    cin >> filename;

    string filePath = execDir + "/Inputs/" + filename;
    ifstream input(filePath);

    // for debug
    //cout << "Whole file path: " << filePath << endl;

    while (input.fail())
    {
        cout << "Could not open the file. Please enter a valid file name:" << endl;
        cin >> filename;

        filePath = execDir + "/Inputs/" + filename;
        input.open(filePath);
    }

    //  Create empty linked list for both Like and Taken
    NodeLike *headLike = nullptr;
    NodeTaken *headTaken = nullptr;
    unsigned int lineNum = 1;

    while (getline(input, line))
    {
        // Separates subject and object from the line
        unsigned int objectStartIdx = line.find(MID_WORD) + MID_WORD.length() + 1;
        string subject = line.substr(0, line.find(' '));
        string object = line.substr(objectStartIdx, line.length() - objectStartIdx);

        cout << "Read line number (" << lineNum << "): " << line << endl;

        lineNum++;

        #ifdef WriteSubjectsAndObjects

        cout << subject << "-" << object << endl;

        #endif

        // If both subject and object are not in taken list
        if (!AreInTaken(subject, object, headTaken))
        {
            // If subject-object pair is not in the likes list
            if (!IsPairInLike(subject, object, headLike))
            {
                // If there is a match
                if (IsMatched(subject, object, headLike))
                {
                    InsertTaken(subject, headTaken);
                    InsertTaken(object, headTaken);
                    DeleteNodesInLikes(subject, object, headLike);
                }
                // There is no match but if subject exist in the likes list
                else if (IsSubjExistInLike(subject, headLike))
                {
                    InsertLikeAccordingSubject(subject, object, headLike);
                }
                // There is no match and subject not in likes list
                else
                {
                    InsertLikeToEnd(subject, object, headLike);
                }
            }
        }
        // Print Likes List and Taken List
        PrintLikeList(headLike);
        PrintTakenList(headTaken);
        cout << endl;
    }
    // Deleting Like and Taken list and ending program
    DeleteLikeList(headLike);
    DeleteTakenList(headTaken);
    cout << "Lists are deleted and the program ends successfully." << endl;
    return 0;
}
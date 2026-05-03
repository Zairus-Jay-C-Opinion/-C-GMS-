#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <cmath>

using namespace std;

struct Component{
    string name;
    double weight;
    double total_items;
    double score;
};

struct Subject{
    int id;
    string name;
    bool weight_validated;
    vector<Component> components;
};

double perent_to_numeric(double pct){
    if (pct >= 98) return 1.00;
    if (pct >= 94) return 1.25;
    if (pct >= 90) return 1.50;
    if (pct >= 88) return 1.75;
    if (pct >= 85) return 2.00;
    if (pct >= 85) return 2.25;
    if (pct >= 80) return 2.50;
    if (pct >= 78) return 2.75;
    if (pct >= 75) return 3.00;
    return 5.00;
}

double numeric_to_min_percent(double numeric){
    if (numeric <= 1.00) return 98;
    if (numeric <= 1.25) return 94;
    if (numeric <= 1.50) return 90;
    if (numeric <= 1.75) return 88;
    if (numeric <= 2.00) return 85;
    if (numeric <= 2.25) return 83;
    if (numeric <= 2.50) return 80;
    if (numeric <= 2.75) return 78;
    if (numeric <= 3.00) return 75;
    return 0;
}

string classify(double numeric){
    if (numeric >= 1.00 && numeric <= 1.50) return "Outstanding";
    if (numeric >= 1.75 && numeric <= 2.50) return "Outstanding";
    if (numeric >= 2.75 && numeric <= 3.00) return "Outstanding";
    return "Critical";
}

void register_user(string &username, string &password){
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "User registered successfully!" << endl;
}

bool login_user(string saved_username, string saved_password){
    string input_u, input_p;
    cout << "Enter username: ";
    cin >> input_u;
    cout << "Enter password: ";
    cin >> input_p;

    if(input_u == saved_username && input_p == saved_password){
        cout << "Login successful!" << endl;
        return true;
    }
    else{
        cout << "Invalid username or password!" << endl;
        return false;
    }
}

void create_subject(vector<Subject> &subjects) {
    Subject s;
    s.id = subjects.size() + 1;
    s.weight_validated = false;
    cout << "Enter subject name: ";
    cin >> s.name;
    subjects.push_back(s);
    cout << "Subject '" << s.name << "' created with ID " << s.id << "." << endl;
}

void list_subjects(const vector<Subject> &subjects) {
    if (subjects.empty()) {
        cout << "No subjects found." << endl;
        return;
    }
    cout << "--- Subjects ---" << endl;
    for (const Subject &s : subjects)
        cout << "[" << s.id << "] " << s.name << (s.weight_validated ? " (weights OK)" : " (weights unvalidated)") << endl;
}

Subject* find_subject(vector<Subject> &subjects, int id) {
    for (Subject &s : subjects)
        if (s.id == id) return &s;
    return nullptr;
}

double total_weight(const Subject &s) {
    double sum = 0;
    for (const Component &c : s.components) 
        sum += c.weight;
    return sum;
}

void scored_progress(const Subject &s, double &earned_pct, double &covered_weight) {
    earned_pct = 0;
    covered_weight = 0;
    for (const Component &c : s.components) {
        if (c.score < 0) continue;
        double pct = (c.total_items > 0) ? (c.score / c.total_items) * 100.0 : c.score;
        earned_pct += pct * (c.weight / 100.0);
        covered_weight += c.weight;
    }
}

void add_component(vector<Subject> &subjects) {
    if (subjects.empty()) {
        cout << "No subjects found! Create a subject first." << endl;
        return;
    }
    list_subjects(subjects);
    int subject_id;
    cout << "Enter subject ID to add component to: ";
    cin >> subject_id;

    Subject *s = find_subject(subjects, subject_id);
    if (!s) {
        cout << "Subject not found." << endl;
        return;
    }

    Component c;
    cout << "Enter component name: ";
    cin >> c.name;
    cout << "Enter component weight (0-100): ";
    cin >> c.weight;

    if (c.weight < 0 || c.weight > 100) {
        cout << "Invalid weight! Must be between 0 and 100." << endl;
        return;
    }

    c.score = -1;
    s->components.push_back(c);
    cout << "Component '" << c.name << "' added to subject '" << s->name << "' successfully!" << endl;
}

bool validate_weight();

void add_scores(vector<Subject> &subjects){
    if (subjects.empty()){
        cout << "No subjects found. Create a subject first" << endl;
        return;
    }

    list_subjects(subjects);
    cout << "Enter subject ID to add scores to: ";
    int subject_id;
    cin >> subject_id;

    Subject *s = find_subject(subjects, subject_id);
    if (!s) {
        cout << "Subject not found." << endl;
        return;
    }

    if (s->components.empty()){
        cout << "No components in '" << s->name << "'. Add a component first." << endl;
        return;
    }

    cout << "---Components in " << s->name << " ---" << endl;
    for (int i = 0; i < s->components.size(); i++){
        cout << "[" << i + 1 << "] " << s->components[i].name << endl;
    }

    cout << "Enter component number to add score to: ";
    int comp_num;
    cin >> comp_num;

    if (comp_num < 1 || comp_num > s->components.size()){
        cout << "Invalid component number!" << endl;
        return;
    }

    Component &c = s -> components[comp_num - 1];
    cout << "Enter score for '" << c.name << "' (0-100): ";
    cin >> c.score;

    if (c.score < 0 || c.score > 100){
        cout << "Invalid score!" << endl;
        c.score = -1;
        return;
    }

    cout << "Score of " << c.score << " added to component '" << c.name << "' successfully!" << endl;
}

bool required_scores();
double compute_subject_average();
double compute_final_average();
bool academic_performance();

void choice(){
    cout << endl;
    cout << "---WELCOME TO THE DYNAMIC GRADE SYSTEM!---" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << "------------------------------------------" << endl;
    cout << endl;
}

void dashboard(vector<Subject> &subjects){
    int option;
    while (true){
        cout << endl;
        cout << "---DASHBOARD---" << endl;
        cout << "1. Create Subject" << endl;
        cout << "2. Add Component" << endl;
        cout << "3. Add Scores" << endl;
        cout << "4. Required Scores" << endl;
        cout << "5. Academic Performance" << endl;
        cout << "6. Logout" << endl;
        cout << "Select an option: ";
        cin >> option;

        switch (option){
            case 1:
                create_subject(subjects);
                break;
            case 2:
                add_component(subjects);
                break;
            case 3:
                add_scores(subjects);
                break;
            case 4:
                required_scores();
                break;
            case 5:
                academic_performance();
                break;
            case 6:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid option!" << endl;
        }
    }
}


int main(){
    string username = "", password = "";
    vector<Subject> subjects;
    int option;

    while (true){
        choice();
        cin >> option;

        if (option==1){
            register_user(username, password);
        }
        else if (option == 2){
            if (username.empty() || password.empty()){
                cout << "No user registered! Please register first." << endl;
                continue;
            }
            else{
                login_user(username, password);

                int attempts = 3;
                bool success = false;

                while (attempts > 0){
                    if(login_user(username, password)){
                        success = true;
                        break;
                    }
                    attempts--;
                    cout << "Incorrect credentials! Attempts left: " << attempts << endl;
                }

                if (success){
                    dashboard(subjects);
                }
                else{
                    cout << "Too many failed attempts! Exiting..." << endl;
                    break;
                }
            }
            
        }
        else if (option == 3){
            cout << "Exiting..." << endl;
            break;
        }
        else{
            cout << "Invalid option!" << endl;
        }
    }
    

    return 0;
}
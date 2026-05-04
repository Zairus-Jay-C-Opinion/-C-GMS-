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

double percent_to_numeric(double pct){
    if (pct >= 98) return 1.00;
    if (pct >= 94) return 1.25;
    if (pct >= 90) return 1.50;
    if (pct >= 88) return 1.75;
    if (pct >= 85) return 2.00;
    if (pct >= 83) return 2.25;
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
    if (numeric >= 1.75 && numeric <= 2.50) return "Safe";
    if (numeric >= 2.75 && numeric <= 3.00) return "At Risk";
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

    if (s->weight_validated){
        cout << "Weights already validated for '" << s->name << "'. Cannot add more components." << endl;
        return;
    }

    Component c;
    c.score = -1;
    c.total_items = 0;

    cout << "Enter component name: ";
    cin >> c.name;
    cout << "Enter component weight (0-100): ";
    cin >> c.weight;

    if (c.weight < 0 || c.weight > 100) {
        cout << "Invalid weight! Must be between 0 and 100." << endl;
        return;
    }

    double current = total_weight(*s);
    if (current + c.weight > 100) {
        cout << "Adding this weight (" << c.weight << "%) would exceed 100%." << " Current total: " << current << "%. Remaining: " << (100 - current) << "%." << endl;
        return;

    }  
    
    cout << "Enter total items for '" << c.name << "' (0 if not defined): ";
    cin >> c.total_items;

    s->components.push_back(c);
    cout << "Component '" << c.name << "' added to subject '" << s->name << "' successfully! " << "Current total weight: " << total_weight(*s) << "%." << endl;
}

void validate_weight(vector<Subject> &subjects){
    if (subjects.empty()) {
        cout << "No subjects found. Create a subject first." << endl;
        return;
    }

    list_subjects(subjects);
    cout << "Enter subject ID to validate: ";
    int subject_id;
    cin >> subject_id;

    Subject *s = find_subject(subjects, subject_id);
    if (!s) {
        cout << "Subjects not found." << endl;
        return;
    }
    if (s->components.empty()){
        cout << "No components added yet." << endl;
        return;
    }
    if (s->weight_validated){
        cout << "Already validated." << endl;
        return;
    }

    double tw = total_weight(*s);
    if (tw != 100.0){
        cout << fixed << setprecision(2);
        cout << "Cannot validate. Total weight is " << tw << "%. Must be exactly 100%." << endl;
        cout << "Components:" << endl;
        for (const Component &c : s->components){
            cout << " " << c.name << " - " <<c.weight << "%" << endl;
        }
        return;
    }
    s->weight_validated = true;
    cout << "Weights validated for '" << s->name << "'! You may now add scores." << endl;
}

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
    if(!s->weight_validated) {
        cout << "Weights not yet validated for '" << s->name << "'. Validate weights before adding scores." << endl;    
        return;
    }

    cout << "---Components in " << s->name << " ---" << endl;
    for (int i = 0; i < s->components.size(); i++){
        const Component &c = s->components[i];
        cout << "[" << i + 1 << "] " << c.name << " (weight: " << c.weight << "%";
        if (c.total_items > 0){
            cout << ", total items: " << c.total_items;
        }
        if (c.score >= 0){
            cout << ", score: " << c.score;
        }
        cout << ")" << endl;
    }

    cout << "Enter component number to add score to: ";
    int comp_num;
    cin >> comp_num;

    if (comp_num < 1 || comp_num > s->components.size()){
        cout << "Invalid component number!" << endl;
        return;
    }

    Component &c = s -> components[comp_num - 1];
    if (c.total_items > 0){
        cout << "Enter raw score for '" << c.name << "' (0- " << c.total_items << "): ";
    }
    else{
        cout << "Enter percentage score for '" << c.name << "' (0-100): ";
    }
    cin >> c.score;

    double max_val = (c.total_items > 0) ? c.total_items : 100;
    if (c.score < 0 || c.score > max_val){
        cout << "Invalid score!" << endl;
        c.score = -1;
        return;
    }

    cout << "Score saved for '" << c.name << "' successfully!" << endl;
}

void required_scores(vector<Subject> &subjects){
    if (subjects.empty()) {
        cout << "No subjects found." << endl;
        return;
    }

    list_subjects(subjects);
    cout << "Enter subject ID: ";
    int subject_id;
    cin >> subject_id;

    Subject *s = find_subject(subjects, subject_id);
    if (!s) {
        cout << "Subject not found." << endl;
        return;
    }
    if(!s->weight_validated) {
        cout << "Weights not yet validated for '" << s->name << "'."<< endl;    
        return;
    }

    bool any_scored = false;
    for (const Component &c : s->components){
        if (c.score >= 0){
            any_scored = true;
            break;
        }
    }
    if(!any_scored){
        cout << "Enter at least one score before using required scores." << endl;
        return;
    }

    double earned_pct, covered_weight;
    scored_progress(*s, earned_pct, covered_weight);
    double remaining_weight = 100.0 - covered_weight;

    double max_possible_pct = earned_pct + (100.0 * (remaining_weight / 100.0));
    double max_numeric = percent_to_numeric(max_possible_pct);

    cout << fixed << setprecision(2);
    cout << "Best possible weighted average : " << max_possible_pct << "%" << endl;
    cout << "Best possible numeric grade    : " << max_numeric << " (" << classify(max_numeric) << ")" << endl;
    cout << endl;

    cout << "Enter target numeric grade (1.00 - 5.00): ";
    double target_numeric;
    cin >> target_numeric;

    double target_pct = numeric_to_min_percent(target_numeric);

    if (remaining_weight <= 0){
        cout << "All components already scored. Use compute subject average instead." << endl;
        return;
    }

    double shared_pct = (target_pct - earned_pct) / (remaining_weight / 100.0);

    cout << fixed << setprecision(2);
    cout << endl;
    cout << "Target grade  : " << target_numeric << " (min" << target_pct << "%)" << endl;
    cout << "Current earned: " << earned_pct << "%/ " << covered_weight << "% weight" << endl;
    cout << "Remaining     : " << remaining_weight << "% weight" << endl;
    cout << endl;

    if (shared_pct > 100){
        cout << "Target grade " << target_numeric << " may no longer achievable." << endl;
        cout << "You would need " << shared_pct << "% on remaining components, which exceeds 100%." << endl;
        cout << "Best possible grade is " << max_numeric << " (" << classify(max_numeric) << ")." << endl;
        return;
    }
    if (shared_pct < 0){
        cout << "Target grade " << target_numeric << " is already guranteed regardless of remaining scores." << endl;
        return;
    }

    cout << "To achieve target grade " << target_numeric << ", you need an average of at least " << shared_pct << "% on remaining components." << endl;
    cout << endl;

    cout << "--- Required Scores for Remaining Components ---" << endl;
    for (const Component &c : s-> components){
        if (c.score >= 0){
            continue;
        }
        cout << c.name << " (" << c.weight << "%):" << endl;
        if (c.total_items > 0){
            double raw = (shared_pct / 100.0) * c.total_items;
            cout << "  Need " << shared_pct << "% -> raw score of at least " << raw << " / " << c.total_items << endl;
        }
        else{
            cout << "  Need at least " << shared_pct << "%" << endl;
        }
    }
}

void compute_subject_average(vector<Subject> &subjects){
    if (subjects.empty()){
        cout << "No subjects found. Create a subject first." << endl;
        return;
    }

    list_subjects(subjects);
    cout << "Enter subject ID to compute average for: ";
    int subject_id;
    cin >> subject_id;

    Subject *s = find_subject(subjects, subject_id);
    if(!s){
         cout << "Subject not found." << endl;
        return;
    }
    if(!s->weight_validated) {
        cout << "Weights not yet validated for '" << s->name << "'."<< endl;    
        return;
    }

    bool any_scored = false;
    for (const Component &c : s->components){
        if (c.score >= 0){
            any_scored = true;
            break;
        }
    }
    if(!any_scored){
        cout << "No scores enetered yet for '" << s->name << "'." << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "--- " << s->name << " Score Breakdown ---" << endl;

    double weighted_sum = 0;
    double covered = 0;

    for (const Component &c : s->components){
        cout << c.name << " (" << c.weight << "%): ";
        if (c.score < 0){
            cout << "No score yet" << endl;
            continue;
        }
        double pct = (c.total_items > 0) ? (c.score / c.total_items) * 100.0 : c.score;
        double contribution = pct * (c.weight / 100.0);
        weighted_sum += contribution;
        covered += c.weight;
        cout << c.score;
        if (c.total_items > 0){
            cout << "/" << c.total_items << " = " << pct << "% -> contributes " << contribution << "%" << endl;
        }
        else{
            cout << "% -> contributes " << contribution << "%" << endl;
        }
    }

    cout << endl;
    cout << "Weighted average (scored components only): " << weighted_sum << "%" << endl;

    if (covered >= 100.0){
        double numeric = percent_to_numeric(weighted_sum);
        cout << "Numeric grade : " << numeric << endl;
        cout << "Performance   : " << classify(numeric) << endl;
    }
    else{
        cout << "(" << (100-covered) << "% of weight not yet scored)" << endl;
    }
}

void compute_final_average(vector<Subject> &subjects){
    if (subjects.empty()){
        cout << "No subjects found. Create a subject first." << endl;
        return;
    }

    vector<double> numeric_grades;
    cout << fixed << setprecision(2);
    cout << "--- Final Average ---" << endl;

    for (const Subject &s : subjects){
        if (!s.weight_validated){
            cout << s.name << ": weights not validated, skipped." << endl;
            continue;        
        }

        bool all_scored = true;
        for(const Component &c : s.components){
            if (c.score < 0){
                all_scored = false;
                break;
            }
        }

        if (!all_scored){
            cout << s.name << ": has unscored components, skipped." << endl;
            continue;
        }

        double weighted_sum = 0;
        for(const Component &c : s.components){
            double pct = (c.total_items > 0) ? (c.score / c.total_items) * 100.0 : c.score;
            weighted_sum += pct * (c.weight / 100.0);
        }

        double numeric = percent_to_numeric(weighted_sum);
        cout << s.name << ": " << weighted_sum << "% -> " << numeric << endl;
        numeric_grades.push_back(numeric);
    }

    if (numeric_grades.empty()){
        cout << "No fully scored subjects to compute final average." << endl;
        return;
    }

    double final_avg = 0;
    for (double g : numeric_grades) final_avg += g;
    final_avg /= numeric_grades.size();

    cout << endl;
    cout << "Final Average: " << final_avg << endl;
    cout << "Performance  : " << classify(final_avg) << endl;
}


void academic_performance(vector<Subject> &subjects){
    if (subjects.empty()){
        cout << "No subjects found. Create a subject first." << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "--- Academic Performance ---" << endl;

    vector<double> final_grades;

    for (const Subject &s : subjects){
        cout << endl << "Subject: " << s.name << endl;

        if (!s.weight_validated){
            cout << "  Status: weights not validated" << endl;
            continue;
        }

        bool all_scored = true;
        double weighted_sum = 0;

        for (const Component &c : s.components){
            if (c.score < 0){
                all_scored = false;
                break;
            }
            double pct = (c.total_items > 0) ? (c.score / c.total_items) * 100.0 : c.score;
            weighted_sum += pct * (c.weight / 100.0);
        }

        if (!all_scored){
            double earned_pct, covered_weight;
            scored_progress(s, earned_pct, covered_weight);
            cout << "  Status   : in progress (" << covered_weight << "% of weight scored)" << endl;
            cout << "  Earned   : " << earned_pct << "%" << endl;
            continue;
        }

        double numeric = percent_to_numeric(weighted_sum);
        final_grades.push_back(numeric);
        cout << "  Weighted Average: " << weighted_sum << "%" << endl;
        cout << "  Numeric Grade   : " << numeric << endl;
        cout << "  Performance     : " << classify(numeric) << endl;
    }

    if(!final_grades.empty()){
        double avg = 0;
        for (double g : final_grades) avg += g;
        avg /= final_grades.size();

        cout << endl;
        cout << "=== Overall Final Average ===" << endl;
        cout << "Average : " << avg << endl;
        cout << "Performance: " << classify(avg) << endl;
    }
}

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
        cout << "3. Valiate Weights" << endl;
        cout << "4. Add Scores" << endl;
        cout << "5. Required Scores" << endl;
        cout << "6. Subject Average" << endl;
        cout << "7. Final Average" << endl;
        cout << "8. Academic Performance" << endl;
        cout << "9. Logout" << endl;
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
                validate_weight(subjects);
                break;
            case 4:
                add_scores(subjects);
                break;
            case 5:
                required_scores(subjects);
                break;
            case 6:
                compute_subject_average(subjects);
                break;
            case 7:
                compute_final_average(subjects);
                break;
            case 8:
                academic_performance(subjects);
                break;
            case 9:
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
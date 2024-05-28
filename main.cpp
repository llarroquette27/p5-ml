#include <iostream>
#include <map>
#include <set>
#include <cmath>
#include "csvstream.hpp"
#include <string>

using namespace std;

class Classifier {
private:
    int postCount;
    int uniqueWordCount;
    set<string> uniqueWords;
    map<string, int> words;
    map<string, int> labels;
    map<pair<string, string>, int> labelAndWords; // pair<label, word>

    // EFFECTS: Updates uniqueWords
    void unique_words(const string &str) {
        istringstream source(str);
        string word;
        while (source >> word) {
            uniqueWords.insert(word);
        }
    }

    // EFFECTS: returns set of unique words
    set<string> get_unique_words(const string &str) {
        istringstream source(str);
        string word;
        set<string> unique;
        while (source >> word) {
            unique.insert(word);
        }
        return unique;
    }

    double log_likelihood(pair<string, string> p) {
        double curr_prob = 0;
        map<pair<string, string>, int>::iterator it = labelAndWords.find(p);
        if (it == labelAndWords.end()) {
            map<string, int>::iterator word_it = words.find(p.second);
            if (word_it == words.end()) {
                curr_prob += log((double)1 / postCount);
            }
            else {
                curr_prob += log((double)words[p.second] / postCount);
            }
         }
         else {
           curr_prob += log((double)labelAndWords[p] / labels[p.first]);
         }
         return curr_prob;
    }

public:
    Classifier() : postCount(0), uniqueWordCount(0){}

    // EFFECTS: Trains classifier by updating all private member variables
    void train_classifer(csvstream& csvin, bool debug) {
        if (debug) {
            cout << "training data:" << endl;
        }
        map<string, string> row;
        string post;
        string label;
        // for each post in training set
        while (csvin >> row) {
            // Increment postCount
            ++postCount;
        
            // Get string of post
            post = row["content"];

            // Update uniqueWords
            unique_words(post);
        
            // Update words - WRONG
            set<string> unique_post = get_unique_words(post);
            for (string word:unique_post) {
                map<string, int>::iterator it = words.find(word);
                if (it == words.end()) {
                    words.insert(pair<string, int>(word, 1));
                }
                else {
                    words[word] += 1;
                }
            }

            // Update labels
            label = row["tag"];
            map<string, int>::iterator it = labels.find(label);
            if(it == labels.end()) {
                labels.insert(pair<string, int>(label, 1));
            }
            else {
                labels[label] += 1;
            }

            // Updates labelAndWords
            for (string word:unique_post) {
                pair<string, string> p(label, word);
                map<pair<string, string>, int>::iterator it 
                = labelAndWords.find(p);
                if (it == labelAndWords.end()) {
                    labelAndWords.insert(
                        pair<pair<string, string>, int>(p, 1));
                }
                else {
                    labelAndWords[p] += 1;
                }
            }
            if (debug) {
                cout << "  label = " << label <<", content = " << post << endl;
            }
        }
        // Update uniqueWordCount
        uniqueWordCount = words.size();
        cout << "trained on " << postCount << " examples" << endl;
        if (debug) {
            cout << "vocabulary size = " << uniqueWordCount << endl;
        }
        cout << endl;

        if (debug) {
            cout << "classes:" << endl;
            for (auto l : labels) {
                cout << "  " << l.first << ", " << l.second 
                << " examples, log-prior = " 
                << log((double)l.second / postCount) << endl;
            }
            cout << "classifier parameters:" << endl;
            for (auto w : labelAndWords) {
                cout << "  " << w.first.first << ":" 
                << w.first.second <<", count = " <<
                w.second << ", log-likelihood = " 
                << log_likelihood(
                    pair<string, string> (w.first.first, w.first.second))
                << endl;
            }
            cout << endl;
        }
    }

// member function to predict post, returns a label
pair<string, double> predict_label(string& post) {
    string label = "";
    double highest_probability = 0;
    set<string> test_post = get_unique_words(post);


    // for each label
    double curr_prob = 0;
    int round = 0;
    for (auto s : labels) {
        // Calculate log-prior probability
        curr_prob = 0;
        curr_prob += log((double)s.second / postCount);
        // Calculate log-likelihoood of a word given label
        for (auto w : test_post) {
            curr_prob += log_likelihood(pair<string, string>(s.first, w));
        }
        // If highest label, change label
        if (curr_prob > highest_probability || round == 0) {
            highest_probability = curr_prob;
            label = s.first;
        }
        round++;
    }
    
    return pair<string, double> (label, highest_probability);
}
};

bool check_files(char* argv[]);
bool check_arguments(int argc, char* argv[]);
void test_data(Classifier& c, csvstream& csvin_test);



int main(int argc, char* argv[]) {
    cout.precision(3);
    // Check command line arguments
    if (check_arguments(argc, argv)) {
        return 1;
    }
    // Check files to be opened
    csvstream csvin_train(argv[1]);
    csvstream csvin_test(argv[2]);
    if (check_files(argv)) {
        return 1;
    }

    // Read in post from file to train classifer
    Classifier c;
    if (argc == 4) {
        c.train_classifer(csvin_train, true);
    }
    else {
        c.train_classifer(csvin_train, false);
    }

    // Read posts from file and test classifier
    test_data(c, csvin_test);
}

bool check_files(char* argv[]) {
    bool error = false;
    ifstream ins;
    ins.open(argv[1]);
    if (!ins.is_open()) {
        cout << "Error opening file: " << argv[1] << endl;
        error = true;
    }
    ins.close();

    ifstream ins_test;
    ins_test.open(argv[2]);
    if (!ins_test.is_open()) {
        cout << "Error opening file: " << argv[2] << endl;
        error = true;
    }
    ins_test.close();

    return error;
}

bool check_arguments(int argc, char* argv[]) {
    bool error = false;
    if (argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        error = true;
    }
    else if (argc == 4 && strcmp(argv[3], "--debug") != 0) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        error = true;
    }

    return error;
}

void test_data(Classifier& c, csvstream& csvin_test) {
    cout << "test data:" << endl;
    map<string, string> row;
    pair<string, double> results;
    int correct = 0;
    int post_count = 0;
    while (csvin_test >> row) {
        ++post_count;
        cout << "  correct = " << row["tag"] << ", ";
        results = c.predict_label(row["content"]);
        cout << "predicted = " << results.first <<
         ", log-probability score = " <<
        results.second << endl;
        cout << "  content = " << row["content"] << endl << endl;
        if (row["tag"] == results.first) {
            ++correct;
        }
    }
    cout << "performance: " << correct << " / " << post_count 
    << " posts predicted correctly";
    cout << endl;
}
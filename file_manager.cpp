#include <bits/stdc++.h>
using namespace std;

struct FileManager {
    unordered_map<string, pair<string, pair<int, int>>> readFiles; 
    unordered_map<string, string> markedFiles;

    void load(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string type, filename, group;
                int count, max_count;
                iss >> type >> filename;
                if (type == "read") {
                    iss >> group >> count >> max_count;
                    readFiles[filename] = {group, {count, max_count}};
                } else if (type == "marked") {
                    iss >> group;
                    markedFiles[filename] = group;
                }
            }
            file.close();
        }
    }

    void save(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& [filename, data] : readFiles) {
                file << "read " << filename << " " << data.first << " " << data.second.first << " " << data.second.second << "\n";
            }
            for (const auto& [filename, group] : markedFiles) {
                file << "marked " << filename << " " << group << "\n";
            }
            file.close();
        }
    }

    void removeFile(const string& action, const string& filename) {
        if (action == "read") {
            readFiles.erase(filename);
        } else if (action == "marked") {
            markedFiles.erase(filename);
        } else {
            cerr << "Invalid action. Use 'read' or 'marked'.\n";
        }
    }

    void viewFileInfo(const string& filename) const {
        auto readIt = readFiles.find(filename);
        if (readIt != readFiles.end()) {
            cout << "Filename: " << filename << "\n";
            cout << "Group: " << readIt->second.first << "\n";
            cout << "Progress: " << readIt->second.second.first << "/" << readIt->second.second.second << "\n";
            return;
        }

        auto markedIt = markedFiles.find(filename);
        if (markedIt != markedFiles.end()) {
            cout << "Filename: " << filename << "\n";
            cout << "Group: " << markedIt->second << "\n";
            return;
        }

        cout << "File not found in any list.\n";
    }
  void listAllGroups() const {
    unordered_set<string> groups;

    for (const auto& [filename, data] : readFiles) {
        groups.insert(data.first);
    }
    for (const auto& [filename, group] : markedFiles) {
        groups.insert(group);
    }

    if (groups.empty()) {
        cout << "No groups found.\n";
        return;
    }

    cout << "Groups:\n";
    for (const string& group : groups) {
        cout << group << "\n";
    }
  }

  void listFilesInGroup(const string& group) const {
    bool found = false;

    cout << "Files in group '" << group << "':\n";

    for (const auto& [filename, data] : readFiles) {
        if (data.first == group) {
            cout << filename << " - Progress: " << data.second.first << "/" << data.second.second << "\n";
            found = true;
        }
    }

    for (const auto& [filename, grp] : markedFiles) {
        if (grp == group) {
            cout << filename << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No files found in this group.\n";
    }
  }
};

void addFile(FileManager& fm, const string& action, const string& filename, const string& group, int count = 0, int max_count = 0) {
    if (action == "read") {
        if (group.length() != 5) {
            cerr << "Group must be exactly 5 characters long.\n";
            return;
        }
        fm.readFiles[filename] = {group, {count, max_count}};
    } else if (action == "marked") {
        if (group.length() != 5) {
            cerr << "Group must be exactly 5 characters long.\n";
            return;
        }
        fm.markedFiles[filename] = group;
    } else {
        cerr << "Invalid action. Use 'read' or 'marked'.\n";
    }
}

void listFiles(const FileManager& fm, const string& listType) {
    if (listType == "read") {
        for (const auto& [filename, data] : fm.readFiles) {
            cout << filename << " - Group: " << (data.first.empty() ? "None" : data.first)
                 << " - Progress: " << data.second.first << "/" << data.second.second << "\n";
        }
    } else if (listType == "marked") {
        for (const auto& [filename, group] : fm.markedFiles) {
            cout << filename << " - Group: " << (group.empty() ? "None" : group) << "\n";
        }
    } else {
        cerr << "Invalid list type. Use 'read' or 'marked'.\n";
    }
}

void printHelp() {
    cout << "Usage:\n";
    cout << "  add <action> <filename> <group> [count] [max_count] - Add a file to the read or marked list. For 'read', optional [count] and [max_count] can be provided.\n";
    cout << "  remove <action> <filename> - Remove a file from the read or marked list.\n";
    cout << "  view <filename>      - View information about a file (group, progress, etc.).\n";
    cout << "  list <type>          - List files in the 'read' or 'marked' list. 'type' should be 'read' or 'marked'.\n";
    cout << "  list-group [<group>] - List all groups if no argument is provided, or list files in a specific group if an argument is given.\n";
    cout << "  help                 - Display this help message.\n";
}


int main(int argc, char* argv[]) {
    FileManager fm;
    fm.load("file_manager_data.txt");

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <command> [args...]\n";
        printHelp();
        return 1;
    }

    string command = argv[1];

    if (command == "add") {
        if (argc < 5 || argc > 7) {
            cerr << "Usage: " << argv[0] << " add <action> <filename> <group> [count] [max_count]\n";
            return 1;
        }
        string action = argv[2];
        string filename = argv[3];
        string group = argv[4];
        int count = (argc > 5) ? stoi(argv[5]) : 0;
        int max_count = (argc > 6) ? stoi(argv[6]) : 0;
        addFile(fm, action, filename, group, count, max_count);
    } else if (command == "remove") {
        if (argc != 4) {
            cerr << "Usage: " << argv[0] << " remove <action> <filename>\n";
            return 1;
        }
        string action = argv[2];
        string filename = argv[3];
        fm.removeFile(action, filename);
    } else if (command == "view") {
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " view <filename>\n";
            return 1;
        }
        string filename = argv[2];
        fm.viewFileInfo(filename);
    } else if (command == "list") {
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " list <type>\n";
            return 1;
        }
        string type = argv[2];
        listFiles(fm, type);
    } else if (command == "list-group") {
        if (argc == 2) {
            fm.listAllGroups();
        } else if (argc == 3) {
            string group = argv[2];
            fm.listFilesInGroup(group);
        } else {
            cerr << "Usage: " << argv[0] << " list-group [<group>]\n";
            return 1;
        }
    } else if (command == "help") {
        printHelp();
    } else {
        cerr << "Unknown command.\n";
        printHelp();
        return 1;
    }

    fm.save("file_manager_data.txt");
    return 0;
}

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

class Pass {
public:
    int current_pass_receiver;
    int current_passer;
    int pass_start_time;
    int end_time;
};

// Helper function to parse a line into a Pass object
Pass parsePass(const string& line) {
    Pass pass;
    stringstream ss(line);
    string cell;

    getline(ss, cell, ',');
    pass.pass_start_time = stoi(cell);

    getline(ss, cell, ',');
    pass.end_time = stoi(cell);

    getline(ss, cell, ',');
    pass.current_passer = stoi(cell);

    getline(ss, cell, ',');
    pass.current_pass_receiver = stoi(cell);

    return pass;
}

// Function to load passes from a file
void loadPassData(const string& fileName, vector<Pass>& passes) {
    ifstream sorted_passes_file(fileName);
    if (!sorted_passes_file.is_open()) {
        cerr << "Error! File not opened\n";
        exit(-1);
    }

    string line;

    // Skip headers
    getline(sorted_passes_file, line);

    // Read each line and parse the pass
    for (; getline(sorted_passes_file, line);) {
        passes.push_back(parsePass(line));
    }

    cout << "Number of passes: " << passes.size() << endl;
    sorted_passes_file.close();
}

// Function to process the next pass in the chain
bool processNextPass(vector<Pass>& passes, Pass& currentPass, vector<Pass>& matches) {
    auto iteratorCurrentPasses = passes.begin();
    while (iteratorCurrentPasses != passes.end()) {
        if (iteratorCurrentPasses->current_passer == currentPass.current_pass_receiver) {
            if (iteratorCurrentPasses->pass_start_time >= currentPass.end_time) {
                currentPass = *iteratorCurrentPasses;
                matches.push_back(currentPass);
                passes.erase(iteratorCurrentPasses);
                return true;
            }
        }
        ++iteratorCurrentPasses;
    }
    return false;
}

// Function to process a single match
void processSingleMatch(vector<Pass>& passes, vector<Pass>& matches) {
    Pass currentPass = passes[0];
    matches.push_back(currentPass);
    passes.erase(passes.begin());

    for (bool continueSearching = true; continueSearching;) {
        continueSearching = processNextPass(passes, currentPass, matches);
    }
}

// Function to extract matches from passes
void extractMatches(vector<Pass>& passes, vector<Pass> matchData[14]) {
    int current_match = 0;

    for (; !passes.empty() && current_match < 14; ++current_match) {
        vector<Pass> matches;
        processSingleMatch(passes, matches);
        matchData[current_match] = matches;
    }
}

// Function to divide matches into teams
void divideTeams(const vector<Pass> matchData[14], vector<Pass> team1Data[14], vector<Pass> team2Data[14]) {
    for (int i = 0; i < 14; i++) {
        for (const auto& match : matchData[i]) {
            if (match.current_passer >= 1 && match.current_pass_receiver <= 14) {
                if (none_of(team1Data[i].begin(), team1Data[i].end(), [&](const Pass& p) {
                        return p.current_passer == match.current_passer && p.current_pass_receiver == match.current_pass_receiver;
                    })) {
                    team1Data[i].push_back(match);
                }
            } 
            else if (match.current_passer >= 15 && match.current_pass_receiver >= 15) {
                if (none_of(team2Data[i].begin(), team2Data[i].end(), [&](const Pass& p) {
                        return p.current_passer == match.current_passer && p.current_pass_receiver == match.current_pass_receiver;
                    })) {
                    team2Data[i].push_back(match);
                }
            }
        }
    }
}

// Function to create files in the required format
void create_files_ober_format(vector<Pass> teamData[], bool isTeam1) {
    int fileIndex = isTeam1 ? 1 : 15;

    for (int match = 0; match < 14; ++match) {
        string fileName = "dg" + to_string(fileIndex) + ".txt";
        ofstream outputFile(fileName);

        if (!outputFile) {
            cerr << "Error opening file: " << fileName << endl;
            continue;
        }

        // Write the total number of vertices (14)
        outputFile << 14 << '\n';

        // Write vertex labels
        for (int vertex = 0; vertex < 14; ++vertex) {
            outputFile << vertex << " \"V" << (vertex + 1) << "\"\n";
        }

        // Write edge counts and connections
        for (int vertex = 0; vertex < 14; ++vertex) {
            int connectionCount = 0;
            vector<pair<int, int>> connections;

            for (const auto& pass : teamData[match]) {
                if (!isTeam1 && pass.current_passer - 15 == vertex) {
                    connectionCount++;
                    connections.emplace_back(pass.current_passer - 15, pass.current_pass_receiver - 15);
                } else if (isTeam1 && pass.current_passer - 1 == vertex) {
                    connectionCount++;
                    connections.emplace_back(pass.current_passer - 1, pass.current_pass_receiver - 1);
                }
            }

            outputFile << connectionCount << '\n';

            for (const auto& connection : connections) {
                outputFile << connection.first << " " << connection.second << "\n";
            }
        }

        fileIndex++;
    }
}

// Function to process matches and divide teams
void processMatchesAndTeams(vector<Pass>& passes) {
    vector<Pass> matchData[14];
    cout << "Extracting matches..." << endl;
    extractMatches(passes, matchData);

    vector<Pass> team1Data[14], team2Data[14];
    divideTeams(matchData, team1Data, team2Data);

    create_files_ober_format(team1Data, true);
    create_files_ober_format(team2Data, false);
}

// Main function
int main() {
    vector<Pass> passes;

    loadPassData("passes.csv", passes);
    processMatchesAndTeams(passes);

    return 0;
}
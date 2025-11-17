#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* 
    Function: isSafeState
    ---------------------
    Checks whether the system is currently in a safe state.
    Uses the Banker's safety algorithm.

    Parameters:
        avail - vector of available resources
        alloc - current allocation matrix
        need  - remaining need matrix
        safeSequence - vector to hold safe sequence (for debugging)

    Returns:
        true if the system is in a safe state, false otherwise
*/

bool isSafeState(vector<int> avail, vector< vector<int> > alloc, vector< vector<int> > need, vector<int> &safeSequence) {
    int P = alloc.size();   // number of processes
    int R = avail.size();   // number of resource types
    vector<int> work = avail;           // work = available
    vector<bool> finish(P, false);      // track if each process can finish
    safeSequence.clear();               // clear any old sequence

    bool progress = true;

    while (progress) {
        progress = false;

        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                bool canProceed = true;

                // Check if need[i] <= work for all resources
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canProceed = false;
                        break;
                    }
                }

                // If process i can proceed, release its resources to work
                if (canProceed) {
                    for (int j = 0; j < R; j++) {
                        work[j] += alloc[i][j];
                    }
                    finish[i] = true;
                    safeSequence.push_back(i);
                    progress = true;
                }
            }
        }
    }

    // If any process cannot finish, unsafe
    for (int i = 0; i < P; i++) {
        if (!finish[i]) return false;
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    string token;

    // Continue reading until end of file
    while (cin >> token) {
        if (token != "R") break;  // expect 'R'

        int R; // number of resources
        cin >> R;

        string pLabel;
        cin >> pLabel; // expect 'P'
        int P; // number of processes
        cin >> P;

        string s;
        cin >> s; // expect 'Available'

        // Read Available vector
        vector<int> Available(R);
        for (int i = 0; i < R; i++)
            cin >> Available[i];

        cin >> s; // expect 'Max'
        vector< vector<int> > Max(P, vector<int>(R));
        for (int i = 0; i < P; i++)
            for (int j = 0; j < R; j++)
                cin >> Max[i][j];

        cin >> s; // expect 'Allocation'
        vector< vector<int> > Allocation(P, vector<int>(R));
        for (int i = 0; i < P; i++)
            for (int j = 0; j < R; j++)
                cin >> Allocation[i][j];

        // Read the process request line (e.g. P1 1 0 2)
        string procStr;
        cin >> procStr;

        int reqProc = -1;
        if (procStr[0] == 'P') {
            reqProc = atoi(procStr.substr(1).c_str());
        }

        vector<int> Request(R);
        for (int j = 0; j < R; j++)
            cin >> Request[j];

        // ----- Compute Need = Max - Allocation -----
        vector< vector<int> > Need(P, vector<int>(R));
        for (int i = 0; i < P; i++)
            for (int j = 0; j < R; j++)
                Need[i][j] = Max[i][j] - Allocation[i][j];

        // Check if system initially safe
        vector<int> dummySeq;
        bool initialSafe = isSafeState(Available, Allocation, Need, dummySeq);

        if (!initialSafe) {
            cout << "The current system is in unsafe state." << "\n";
            continue;
        } else {
            cout << "Before granting the request of " << procStr << ", the system is in safe state." << "\n";
        }

        // ----- Simulate granting the request -----
        cout << "Simulating granting " << procStr << "'s request." << "\n";

        // Apply the request temporarily
        vector< vector<int> > newAlloc = Allocation;
        vector< vector<int> > newNeed = Need;
        vector<int> newAvail = Available;

        for (int j = 0; j < R; j++) {
            newAlloc[reqProc][j] += Request[j];
            newNeed[reqProc][j] -= Request[j];
            newAvail[j] -= Request[j];
        }

        // Print new Need matrix
        cout << "New Need\n";
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < R; j++) {
                if (j) cout << " ";
                cout << newNeed[i][j];
            }
            cout << "\n";
        }

        // If new available is negative, unsafe
        bool negative = false;
        for (int j = 0; j < R; j++)
            if (newAvail[j] < 0)
                negative = true;

        if (negative) {
            cout << procStr << "'s request cannot be granted. The system will be in unsafe state." << "\n";
            continue;
        }

        // Check if system safe after request
        vector<int> seqAfter;
        bool safeAfter = isSafeState(newAvail, newAlloc, newNeed, seqAfter);

        if (safeAfter) {
            cout << procStr << "'s request can be granted. The system will be in safe state." << "\n";
        } else {
            cout << procStr << "'s request cannot be granted. The system will be in unsafe state." << "\n";
        }
    }

    return 0;
}
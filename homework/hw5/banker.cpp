#include <iostream>
#include <vector>

using namespace std;

#define PS_CNT 5 // process count
#define RS_CNT 4 // resource count

// current allocation matrix
vector <vector<int>> C = {
        {3, 0, 1, 1},
        {0, 1, 0, 0},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {0, 0, 0, 0}
};

// request matrix
vector <vector<int>> R = {
        {2, 1, 0, 0},
        {0, 1, 1, 1},
        {3, 1, 0, 0},
        {0, 0, 1, 0},
        {3, 1, 1, 0}
};

// available matrix
vector<int> A = {1, 1, 2, 0};

// existing matrix
vector<int> E = {6, 3, 4, 2};

#define S_RUNNING 1
#define S_SUCCESS 2
#define S_FAIL 0

// check if request R_i can be satisfied
bool isRequestSatisfied(vector<int> R, vector<int> A) {
    for (int i = 0; i < RS_CNT; ++i) {
        if (R[i] > A[i]) {
            return false;
        }
    }
    return true;
}

// update available source
void freeSrc(vector<int> C, vector<int> &A) {
    for (int i = 0; i < RS_CNT; ++i) {
        A[i] += C[i];
    }
}

bool isTerminated(int PID, vector<int> terminated) {
    for (int p: terminated) {
        if (p == PID) {
            return true;
        }
    }
    return false;
}


int main() {
    int currPSCnt = PS_CNT;
    int currState = S_RUNNING;
    vector<int> terminated;

    while (currState == S_RUNNING) {
        // find a process whose request can be satisfied,
        int PID;
        for (PID = 0; PID < PS_CNT; ++PID) {
            // if the process has already been satisfied
            if (isTerminated(PID, terminated)) {
                continue;
            }

            // if an unsatisfied process can be satisfied
            if (isRequestSatisfied(R[PID], A)) {
                break;
            }
        }

        // mark it as terminated, and free all of its resources
        if (PID < PS_CNT) {
            terminated.push_back(PID);
            std::cout << "executing process #" << PID << "...\n";
            freeSrc(C[PID], A);
            currPSCnt--;
        } else { // not find
            currState = S_FAIL;
            std::cout << "fail execute all the processes\n";

        }

        // check all requests are satisfied
        if (currPSCnt == 0) {
            currState = S_SUCCESS;
            std::cout << "successfully execute all the processes\n";
        }
    }

    return 0;
}

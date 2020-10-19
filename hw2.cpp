#include <bits/stdc++.h>
using namespace std;

struct MessageObj {
    int priority;
    int pre_priority;
    double trans;
    double period;
    MessageObj() : priority(-1), pre_priority(-1), trans(0.0), period(0.0) {}
    MessageObj(int x, double y, double z) : priority(x), pre_priority(x), trans(y), period(z) {}
};

double findMaxTrans(vector<MessageObj*> vec_msg, int bottom){
    double ret = vec_msg[bottom]->trans;
    for(int i=bottom+1; i<vec_msg.size(); ++i){
        if(vec_msg[i]->trans > ret) ret = vec_msg[i]->trans;
    }
    return ret;
}
double wsResponseTime(int iters, double tau, vector<MessageObj*> &vec_msg) {
    vector<double> ans;
    double ret=0.0;
    for(int i=0; i<iters; ++i){
        double max_trans = findMaxTrans(vec_msg, i);
        if(i==0) {
            ans.push_back(max_trans + vec_msg[i]->trans); 
            //cout << max_trans+vec_msg[i]->trans << endl;
            continue;
        }
        double lhs = -1.0;
        double rhs = max_trans;
        int counter=0;
        while(lhs != rhs){
            counter++;
            lhs = rhs;
            rhs = max_trans;
            for(int j=0; j<i; ++j){
                rhs += ceil((lhs+tau) / vec_msg[j]->period)*vec_msg[j]->trans;
            }
        }
        //cout << lhs+vec_msg[i]->trans << endl;
        ans.push_back(lhs+vec_msg[i]->trans);
    }
    for(int i=0; i<ans.size(); ++i) {
        ret += ans[i];
    }
    return ret;
}
int genRandomNum(int base) { return rand()%base; }
void whoToBeSwap(int base, int num, vector<int> &vec_index_swap) {
    vec_index_swap.clear();
    auto check_and_push = [&]() {
        int input = genRandomNum(base);
        for(int j=0; j<vec_index_swap.size(); ++j) {
            if(input == vec_index_swap[j]) return false;
        }
        vec_index_swap.push_back(input);
        return true;
    };
    for(int i=0; i<num; ++i) {
        while(!check_and_push()) {}
    }
}
void getSwap(vector<MessageObj*> &vec_msg, vector<int> &vec_index_swap) {
    for(int i=0; i<vec_index_swap.size(); i+=2) {
        int id_1 = vec_index_swap[i];
        int id_2 = vec_index_swap[i+1];
        MessageObj* tmp = vec_msg[id_1];
        vec_msg[id_1] = vec_msg[id_2];
        vec_msg[id_2] = tmp;
    }
}

void restoreVec(vector<MessageObj*> &vec_msg, vector<int> &vec_index_swap) { getSwap(vec_msg, vec_index_swap); }

int main(int argc, char* argv[]){
    ifstream file;
    string input_str;
    vector<vector<double>> vec;
    vector<MessageObj*> vec_msg;
    vector<MessageObj*> vec_msg_optimal;
    double tmp;
    file.open("Input.dat", ios::in);
    if(file.is_open()){
        int round=0;
        while(getline(file, input_str)){
            ++round;
            vector<double> vec_tmp;
            stringstream strmm(input_str);
            while(strmm >> tmp){
                vec_tmp.emplace_back(tmp);
            }
            if(round != 1 && round != 2){
                MessageObj* member = new MessageObj(vec_tmp[0], vec_tmp[1], vec_tmp[2]);
                vec_msg.emplace_back(member);
            }
            vec.emplace_back(vec_tmp);
        }
        vec_msg_optimal = vec_msg;
        int iters = vec[0][0];
        double tao = vec[1][0];
        int counter=0;
        double cur_cost = wsResponseTime(iters, tao, vec_msg_optimal);
        double best_cost = cur_cost;
        double local_cost = 0.0;
        double temperature = 100.0;
        double anneal_rate = 0.98;
        vector<int> vec_index_swap;
        srand(time(nullptr));
        while(temperature > 10.0) {
            counter++;
            whoToBeSwap(iters, 4, vec_index_swap);
            getSwap(vec_msg, vec_index_swap);
            local_cost = wsResponseTime(iters, tao, vec_msg);
            if(local_cost < best_cost) {
                best_cost = local_cost;
                vec_msg_optimal = vec_msg;
            }
            double cost_diff = local_cost - cur_cost;
            if(cost_diff <= 0) {
                cur_cost = local_cost;
            }
            else {
                double prob = exp(-1 * abs(cost_diff)/temperature);
                if(((double)rand() / RAND_MAX) < prob) {
                    restoreVec(vec_msg, vec_index_swap);
                }else{
                    cur_cost = local_cost;
                }
            }
            temperature *= anneal_rate;
            // for(int i=0; i<vec_index_swap.size(); ++i) cout << vec_index_swap[i] << endl;
        }
        for(int i=0; i<vec_msg_optimal.size(); ++i) {
            cout << vec_msg_optimal[i]->priority << endl;
        }
        cout << "Total iteration: " << counter << ", and final worst case response time: " << best_cost << endl;
    }else{
        cout << "Failed to open file.\n";
    }
    return 0;
}


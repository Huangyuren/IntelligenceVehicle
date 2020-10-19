#include <bits/stdc++.h>

using namespace std;

double findMaxTrans(vector<double> trans, int bottom){
    double ret = trans[bottom];
    for(int i=bottom+1; i<trans.size(); ++i){
        if(trans[i] > ret) ret = trans[i];
    }
    return ret;
}
vector<double> wsResponseTime(vector<int> pri, vector<double> trans, vector<double> period, int iters, double tau){
    vector<double> ans;
    for(int i=0; i<iters; ++i){
        double max_trans = findMaxTrans(trans, i);
        if(i==0) {
            ans.push_back(max_trans + trans[i]); 
            cout << max_trans+trans[i] << endl;
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
                rhs += ceil((lhs+tau)/period[j])*trans[j];
            }
        }
        cout << lhs+trans[i] << endl;
        ans.push_back(lhs+trans[i]);
    }
    return ans;
}
int main(int argc, char* argv[]){
    ifstream file;
    string input_str;
    vector<vector<double>> vec;
    vector<int> priority;
    vector<double> transTime;
    vector<double> period;
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
                for(int i=0; i<vec_tmp.size(); ++i){
                    if(i==0) priority.push_back(vec_tmp[i]);
                    if(i==1) transTime.push_back(vec_tmp[i]);
                    if(i==2) period.push_back(vec_tmp[i]);
                }
            }
            vec.emplace_back(vec_tmp);
        }
        int iters = vec[0][0];
        double tao = vec[1][0];
        wsResponseTime(priority, transTime, period, iters, tao);
    }else{
        cout << "Failed to open file.\n";
    }
    return 0;
}

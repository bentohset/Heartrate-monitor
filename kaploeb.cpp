//use hashtable for O(1)
//more than 1 hashtable

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

typedef long long ll;
#define fast_cin() ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)

bool sortbyrank(const pair<int,int>& a, const pair<int,int>& b){
    if(get<1>(a) != get<1>(b)) 
            return get<1>(a) < get<1>(b);
    /* if(get<2>(a) != get<2>(b))
        return get<2>(a) < get<2>(b); */
    return get<0>(a) < get<0>(b);
}

int main(){
    fast_cin();
    int l,k,s; cin >> l >> k >> s;
    unordered_map<int,int> lapsDone;
    unordered_map<int,int> lapTimes;
    while (l--){
        int si; cin >> si;
        string time; cin>>time;
        lapsDone[si]++;                                                             //store frequency of lap timings as no. of laps done in hashmap
        int minutes = (time[0]-'0')*10+(time[1]-'0');                               //convert minutes into integer
        //cout << "min1=" << time[0] << " min2=" << time[1] << " totalmin=" << minutes << endl;
        int seconds = ((time[3]-'0')*10+(time[4]-'0')) + minutes*60;                //convert seconds into integer and minutes in seconds
        //cout << "sec1=" << time[3] << "sec2=" << time[4] << "totalsec=" << seconds << endl;
        lapTimes[si] += seconds;                                                    //store total time in seconds to hashmap
    }
    //cout << lapTimes[2];
    vector<pair<int,int>> pairs;                       //vector of pair<start no., lap time>
    for (auto kv: lapsDone){
        if (kv.second == k)
            pairs.push_back(make_pair(kv.first,lapTimes[kv.first]));             //create pair for each start no. and store into vector
    }
    stable_sort(pairs.begin(),pairs.end(),sortbyrank);          //stable sort according to sorting sequence 1: lap time, 2: start no.
    for (auto i:pairs) cout << get<0>(i) << endl;               //print vector
    return 0;
}
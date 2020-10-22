#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <limits>
#include <utility> // pair
#include <tuple>
#include <algorithm>
#include <iterator>     // std::distance
#include <list>         // std::list

#define MINUS_INT_MAX INT_MAX * (-1)

std::pair<int, int> get_index_pair(int previous_color) {
    std::pair<int, int> index_pair;
    index_pair.first = 0;
    index_pair.second = 2;
    if(previous_color == 0) {
        index_pair.first = 1;
        index_pair.second = 2;
    } else if(previous_color == 1) {
        index_pair.first = 0;
        index_pair.second = 2;
    } else if(previous_color == 2) {
        index_pair.first = 0;
        index_pair.second = 1;
    }
    return index_pair;
}

std::vector<int> check_exist_same_value(std::vector<std::vector<int>> costs) {
    std::vector<int> tmp_same_vec;
    for(int i = 0 ; i < costs[i].size(); i++) {
        for(int j = i+1 ; j < costs[i].size(); j++) {
            if(costs[i][i] == costs[i][j]) {
                if(find(tmp_same_vec.begin(), tmp_same_vec.end(), costs[i][i]) == tmp_same_vec.end())
                    tmp_same_vec.push_back(i);
                if(find(tmp_same_vec.begin(), tmp_same_vec.end(), costs[i][j]) == tmp_same_vec.end())
                    tmp_same_vec.push_back(j);
                break;
            }
        }
        if(tmp_same_vec.size() == 3){
                break;
        }
    }
    return tmp_same_vec;
}

class Solution {
public:
    int minCost(std::vector<std::vector<int>>& costs) {
        if(costs.size() <= 0){
            return MINUS_INT_MAX;
        }
        int min_cost = INT_MAX;
        int previous_color = 0; // 0:red, 1:blue, 2:green
        std::vector<std::vector<int>> same_val_color_candidate;
        std::vector<int> min_cost_accumuated_array(costs.size(), INT_MAX);
        std::vector<int> min_candidate{costs[0][0], costs[0][1], costs[0][2]};
        int min_cost_index = std::min_element(min_candidate.begin(), min_candidate.end()) - min_candidate.begin();
        previous_color = min_cost_index;
        printf("index: %d\n", min_cost_index);
        min_cost = costs[0][min_cost_index];
        min_cost_accumuated_array[0] = min_cost;

        // find if there are some same color cost exist in costs arr
        std::vector<int> tmp_same_vec;
        tmp_same_vec = check_exist_same_value(costs);

        if(tmp_same_vec.size() > 0) {
            same_val_color_candidate.push_back(tmp_same_vec);
        }

        printf("min_cost:%d, previous_color:%d\n", min_cost, previous_color);

        for(int i = 1 ; i < costs.size() ; i++) {
            std::pair<int, int> index;

            // if the previous house has the same cost of color 
            if(same_val_color_candidate.size() > 0){

                std::vector<int> same_val_color_index_arr = same_val_color_candidate.back();
                same_val_color_candidate.pop_back();

                int tmp_min = INT_MAX;
                int min_val = 0;
                int previous_min_color_value = 0;
                printf("=============\n");

                // check which item can get minimum costs
                for(int j = 0 ; j < same_val_color_index_arr.size() ; j++){
                    previous_min_color_value = tmp_min;
                    previous_color = same_val_color_index_arr[j];
                    printf("[%d] previous_color: %d\n", j, previous_color);
                    index = get_index_pair(previous_color); // the index which is not include the previous index
                    min_val = std::min(costs[i][index.first] + costs[i-1][previous_color], costs[i][index.second] + costs[i-1][previous_color]);
                    tmp_min = std::min(min_val, tmp_min);

                    // determine which item as a previous color could get minimum costs
                    if(tmp_min != previous_min_color_value){
                        previous_color = min_val == costs[i][index.first] + costs[i-1][previous_color] ? index.first : index.second;
                    }
                    printf("previous_color:%d, tmp_min:%d\n", previous_color, tmp_min);

                }
                printf("=============\n");
                min_cost -= costs[i-1][previous_color];
                min_cost += tmp_min;

                // check if there are some same color value exist in current house
                std::vector<int> sub_tmp_same_vec;
                sub_tmp_same_vec = check_exist_same_value(costs);
                if(sub_tmp_same_vec.size() > 0) {
                    same_val_color_candidate.push_back(sub_tmp_same_vec);
                }

                printf("min_cost:%d, previous_color:%d\n", min_cost, previous_color);
            } else {

                index = get_index_pair(previous_color);
                // check if there are some same color value exist in current house
                if(costs[i][index.first] == costs[i][index.second])
                    same_val_color_candidate.push_back({index.first, index.second});

                int min_val = std::min(costs[i][index.first], costs[i][index.second]);
                previous_color = min_val == costs[i][index.first] ? index.first : index.second;
                min_cost += min_val;
                printf("min_val:%d, previous_color:%d\n", min_val, previous_color);
            }
        }
        return min_cost;
    }
};

int main(int argc, char *argv[]) {
    Solution *s = new Solution();
    std::vector<int> c0{ 2, 2, 2 };
    std::vector<int> c1{ 1, 1, 1 };
    std::vector<int> c2{ 8, 2, 2 };
    std::vector<int> c3{ 10,2, 1 };
    std::vector< std::vector<int> > cost{ c0, c1, c2, c3 };
    printf("output:%d\n", s -> minCost(cost));
}

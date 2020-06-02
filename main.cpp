#include <iostream>
#include <cstring>
#include <vector>
#include "main.h"

using namespace std;


typedef struct Task {
    int head;
    int tail;
    int demand;
} Task;


typedef struct Cycle {
    vector<int> task;
    int cycle_cost;
    int cycle_demand;
} Cycle;


typedef struct Individual {
    vector<Cycle> solution;
    int total_cost;
//    int total_demand;
} Individual;


int depot;
int vertex_num;
int task_num;
int no_task_num;
int vehicle_num;
int capacity;
int temp_node1, temp_node2, temp_cost, temp_demand;

// 两点间的最短花费路径
int min_cost[MAX_NODE_NUM][MAX_NODE_NUM];
// 保存任意两点间的路径
int path[MAX_NODE_NUM][MAX_NODE_NUM];

// 两点间的花费
int cost[MAX_NODE_NUM][MAX_NODE_NUM];
// 两点间的服务
//int demand[MAX_NODE_NUM][MAX_NODE_NUM];
int edge_id[MAX_NODE_NUM][MAX_NODE_NUM];
// 判断任务是否被服务过了
//int served[MAX_NODE_NUM][MAX_NODE_NUM];

// node: [nei_node1, nei_node2, ...], 1: [3, 14], 2: [4, 3]..
// 任意一个点为起点的任务
//vector<vector<int> > adjacency_list(MAX_NODE_NUM);

Task task[MAX_TASK_TAG_LENGTH];
int served[MAX_TASK_TAG_LENGTH];

char dummy_string[50];

char input_file[100] = "../instance/gdb1.dat"; // the instance can be changed here


void init() {
    for (int i = 1; i <= vertex_num; i++) {
        for (int j = 1; j <= vertex_num; j++) {
            min_cost[i][j] = i == j ? 0 : INF;
            path[i][j] = 0;
//            cost[i][j] = 0;
//            demand[i][j] = 0;
//            served[i][j] = 0;
//            edge_id[i][j] = 0;
        }
//        adjacency_list[i].clear();
    }
}


void floyd() {
    int i, j, k;
    for (k = 1; k <= vertex_num; k++) {
        for (i = 1; i <= vertex_num; i++) {
            for (j = 1; j <= vertex_num; j++) {
                if (min_cost[i][k] + min_cost[k][j] < min_cost[i][j]) {
                    min_cost[i][j] = min_cost[i][k] + min_cost[k][j];
                    path[i][j] = k;
                }
            }
        }
    }
}


void get_path(int i, int j, vector<int> &rst) {
    if (i == j) return;
    if (path[i][j] == 0) {
        rst.push_back(j);
    } else {
        get_path(i, path[i][j], rst);
        get_path(path[i][j], j, rst);
    }
}


vector<int> get_path(int start, int end) {
    vector<int> rst;
    get_path(start, end, rst);
    return rst;
}


void print_array(int m[MAX_NODE_NUM][MAX_NODE_NUM]) {
    for (int i = 1; i <= vertex_num; i++) {
        for (int j = 1; j <= vertex_num; j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

//
//void print_adjacency_list() {
//    for (int i = 1; i <= vertex_num; i++) {
//        cout << i << ": ";
//        for (int j = 0; j < adjacency_list[i].size(); j++) {
//            cout << adjacency_list[i][j] << " ";
//        }
//        cout << endl;
//    }
//}


void read_data() {
    FILE *fp;
    fp = fopen(input_file, "r");

    while (true) {
        fscanf(fp, "%s", dummy_string);
        //printf("dummy_string = %s\n", dummy_string);

        if (strcmp(dummy_string, "VERTICES") == 0) {
            fscanf(fp, "%s", dummy_string);
            fscanf(fp, "%d", &vertex_num);
            init();
        } else if (strcmp(dummy_string, "ARISTAS_REQ") == 0) {
            fscanf(fp, "%s", dummy_string);
            fscanf(fp, "%d", &task_num);
        } else if (strcmp(dummy_string, "ARISTAS_NOREQ") == 0) {
            fscanf(fp, "%s", dummy_string);
            fscanf(fp, "%d", &no_task_num);
        } else if (strcmp(dummy_string, "VEHICULOS") == 0) {
            fscanf(fp, "%s", dummy_string);
            fscanf(fp, "%d", &vehicle_num);
        } else if (strcmp(dummy_string, "CAPACIDAD") == 0) {
            fscanf(fp, "%s", dummy_string);
            fscanf(fp, "%d", &capacity);
        } else if (strcmp(dummy_string, "LISTA_ARISTAS_REQ") == 0) {
            fscanf(fp, "%s", dummy_string);

            for (int i = 1; i <= task_num; i++) {
                fscanf(fp, "%s", dummy_string);
                fscanf(fp, "%d,", &temp_node1);
                fscanf(fp, "%d)", &temp_node2);
                fscanf(fp, "%s", dummy_string);
                fscanf(fp, "%d", &temp_cost);
                fscanf(fp, "%s", dummy_string);
                fscanf(fp, "%d", &temp_demand);
//                cout << temp_node1 << "," << temp_node2 << endl;
//                cout << temp_node2 << "," << temp_node1 << endl;

                cost[temp_node1][temp_node2] = temp_cost;
                cost[temp_node2][temp_node1] = temp_cost;
                min_cost[temp_node1][temp_node2] = temp_cost;
                min_cost[temp_node2][temp_node1] = temp_cost;
//                demand[temp_node1][temp_node2] = temp_demand;
//                demand[temp_node2][temp_node1] = temp_demand;
                edge_id[temp_node1][temp_node2] = i;
                edge_id[temp_node2][temp_node1] = i;
//                served[temp_node1][temp_node2] = 0;
//                served[temp_node2][temp_node1] = 0;

                task[i * 2 - 2].head = temp_node1;
                task[i * 2 - 2].tail = temp_node2;
                task[i * 2 - 2].demand = temp_demand;
                task[i * 2 - 1].head = temp_node1;
                task[i * 2 - 1].tail = temp_node2;
                task[i * 2 - 1].demand = temp_demand;

//                adjacency_list[temp_node1].push_back(temp_node2);
//                adjacency_list[temp_node2].push_back(temp_node1);
            }
        } else if (strcmp(dummy_string, "LISTA_ARISTAS_NOREQ") == 0) {
            fscanf(fp, "%s", dummy_string);
            for (int i = task_num + 1; i <= task_num + no_task_num; i++) {
                fscanf(fp, "%s", dummy_string);
                fscanf(fp, "%d,", &temp_node1);
                fscanf(fp, "%d)", &temp_node2);
                fscanf(fp, "%s", dummy_string);
                fscanf(fp, "%d", &temp_cost);

                cost[temp_node1][temp_node2] = temp_cost;
                cost[temp_node2][temp_node1] = temp_cost;
                min_cost[temp_node1][temp_node2] = temp_cost;
                min_cost[temp_node2][temp_node1] = temp_cost;
//                demand[temp_node1][temp_node2] = 0;
//                demand[temp_node2][temp_node1] = 0;
                edge_id[temp_node1][temp_node2] = i;
                edge_id[temp_node2][temp_node1] = i;
            }
        } else if (strcmp(dummy_string, "DEPOSITO") == 0) {
            fscanf(fp, "%s", dummy_string);
            fscanf(fp, "%d", &depot);
            break;
        }
    }

    fclose(fp);
}


void greedy() {
}


Individual rand_init_individual() {
    Individual individual;
    int cur_task_num = 0, cur_capacity = 0;
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 0;
    }
    Cycle cycle;
    while (cur_task_num < task_num) {
        bool found_task = false;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0 && cur_capacity + task[t].demand <= capacity) {
                cycle.task.push_back(t);
                cur_capacity += task[t].demand;
                found_task = true;
                served[t] = 1;
                if (t % 2 == 0) {
                    served[t + 1] = 1;
                } else {
                    served[t - 1] = 1;
                }
                break;
            }
        }

        if (found_task) {
            cur_task_num++;
        } else {
            individual.solution.push_back(cycle);
            Cycle t;
            cycle = t;
            cur_capacity = 0;
        }
    }

    return individual;
}


int main() {
    read_data();
    floyd();
    Individual individual = rand_init_individual();
    for (const auto& c : individual.solution) {
        for (int t : c.task) {
            cout << task[t].head << " " << task[t].tail << " ";
        }
        cout << endl;
    }


    /*
    print_array(cost);
    cout << endl;
    print_array(demand);
    cout << endl;
    print_array(min_cost);
    cout << endl;
    print_array(edge_id);
    cout << endl;
    print_array(served);
    cout << endl;
    print_adjacency_list();

    vector<int> rst = get_path(1, 3);
    for (int i : rst) {
        cout << i << " ";
    }
     */
    return 0;
}

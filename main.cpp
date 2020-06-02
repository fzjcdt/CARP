#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_TASK_TAG_LENGTH 1001
#define MAX_NODE_NUM 300
const int INF = 0x3f3f3ff3;

typedef struct Task {
    int head;
    int tail;
    int demand;
} Task;


typedef struct Cycle {
    vector<int> task_index;
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
int total_demand;

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

//char input_file[100] = "../instance/gdb/gdb1.dat"; // the instance can be changed here
char input_files1[][100] = {
        {"../instance/gdb/gdb1.dat"},
//        {"../instance/gdb/gdb2.dat"},
};

char input_files[][100] = {
        {"../instance/gdb/gdb1.dat"},
        {"../instance/gdb/gdb2.dat"},
        {"../instance/gdb/gdb3.dat"},
        {"../instance/gdb/gdb4.dat"},
        {"../instance/gdb/gdb5.dat"},
        {"../instance/gdb/gdb6.dat"},
        {"../instance/gdb/gdb7.dat"},
        {"../instance/gdb/gdb8.dat"},
        {"../instance/gdb/gdb9.dat"},
        {"../instance/gdb/gdb10.dat"},
        {"../instance/gdb/gdb11.dat"},
        {"../instance/gdb/gdb12.dat"},
        {"../instance/gdb/gdb13.dat"},
        {"../instance/gdb/gdb14.dat"},
        {"../instance/gdb/gdb15.dat"},
        {"../instance/gdb/gdb16.dat"},
        {"../instance/gdb/gdb17.dat"},
        {"../instance/gdb/gdb18.dat"},
        {"../instance/gdb/gdb19.dat"},
        {"../instance/gdb/gdb20.dat"},
        {"../instance/gdb/gdb21.dat"},
        {"../instance/gdb/gdb22.dat"},
        {"../instance/gdb/gdb23.dat"},
        // ---------------------------
        {"../instance/val/val1A.dat"},
        {"../instance/val/val1B.dat"},
        {"../instance/val/val1C.dat"},
        {"../instance/val/val2A.dat"},
        {"../instance/val/val2B.dat"},
        {"../instance/val/val2C.dat"},
        {"../instance/val/val3A.dat"},
        {"../instance/val/val3B.dat"},
        {"../instance/val/val3C.dat"},
        {"../instance/val/val4A.dat"},
        {"../instance/val/val4B.dat"},
        {"../instance/val/val4C.dat"},
        {"../instance/val/val4D.dat"},
        {"../instance/val/val5A.dat"},
        {"../instance/val/val5B.dat"},
        {"../instance/val/val5C.dat"},
        {"../instance/val/val5D.dat"},
        {"../instance/val/val6A.dat"},
        {"../instance/val/val6B.dat"},
        {"../instance/val/val6C.dat"},
        {"../instance/val/val7A.dat"},
        {"../instance/val/val7B.dat"},
        {"../instance/val/val7C.dat"},
        {"../instance/val/val8A.dat"},
        {"../instance/val/val8B.dat"},
        {"../instance/val/val8C.dat"},
        {"../instance/val/val9A.dat"},
        {"../instance/val/val9B.dat"},
        {"../instance/val/val9C.dat"},
        {"../instance/val/val9D.dat"},
        {"../instance/val/val10A.dat"},
        {"../instance/val/val10B.dat"},
        {"../instance/val/val10C.dat"},
        {"../instance/val/val10D.dat"},
        // ---------------------------
        {"../instance/egl/egl-e1-A.dat"},
        {"../instance/egl/egl-e1-B.dat"},
        {"../instance/egl/egl-e1-C.dat"},
        {"../instance/egl/egl-e2-A.dat"},
        {"../instance/egl/egl-e2-B.dat"},
        {"../instance/egl/egl-e2-C.dat"},
        {"../instance/egl/egl-e3-A.dat"},
        {"../instance/egl/egl-e3-B.dat"},
        {"../instance/egl/egl-e3-C.dat"},
        {"../instance/egl/egl-e4-A.dat"},
        {"../instance/egl/egl-e4-B.dat"},
        {"../instance/egl/egl-e4-C.dat"},
        {"../instance/egl/egl-s1-A.dat"},
        {"../instance/egl/egl-s1-B.dat"},
        {"../instance/egl/egl-s1-C.dat"},
        {"../instance/egl/egl-s2-A.dat"},
        {"../instance/egl/egl-s2-B.dat"},
        {"../instance/egl/egl-s2-C.dat"},
        {"../instance/egl/egl-s3-A.dat"},
        {"../instance/egl/egl-s3-B.dat"},
        {"../instance/egl/egl-s3-C.dat"},
        {"../instance/egl/egl-s4-A.dat"},
        {"../instance/egl/egl-s4-B.dat"},
        {"../instance/egl/egl-s4-C.dat"},
};


int file_num = sizeof(input_files) / sizeof(char[100]);

//char input_file[100] = "../instance/egl/egl-e1-A.dat"; // the instance can be changed here


int random_num(int range) {
    return rand() % (range);
}


void init() {
    total_demand = 0;
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


void read_data(int file_index) {
    FILE *fp;
//    fp = fopen(input_file, "r");
    fp = fopen(input_files[file_index], "r");

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
                total_demand += temp_demand;

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


Individual greedy_init_individual_min_dis() {
    Individual individual;
    int cur_task_num = 0, cur_capacity = 0;
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 0;
    }

    Cycle cycle;
    int cur_node = depot, cur_min_dis, next_task_index;
    while (cur_task_num < task_num) {
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0) {
                if (min_cost[cur_node][task[t].head] < cur_min_dis && cur_capacity + task[t].demand <= capacity) {
                    cur_min_dis = min_cost[cur_node][task[t].head];
                    next_task_index = t;
                }
            }
        }

        // 头部为起点，且不是第一个任务，则不加入这条任务，重新派一辆车
        if (next_task_index != -1) {
            if (task[next_task_index].head == depot and !cycle.task_index.empty()) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
                continue;
            }
            cycle.task_index.push_back(next_task_index);
            cur_capacity += task[next_task_index].demand;

            // 标记为服务过了
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;

            // 回到了起点，派新的车
            if (task[next_task_index].tail == depot) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
            }
        } else {
            // 没有找到可以装得下的任务
            individual.solution.push_back(cycle);
            Cycle temp_cycle;
            cycle = temp_cycle;
            cur_capacity = 0;
            cur_node = depot;
        }
    }

    return individual;
}


Individual greedy_init_individual_min_dis_early_stop() {
    Individual individual;
    int cur_task_num = 0, cur_capacity = 0;
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 0;
    }

    Cycle cycle;
    int cur_node = depot, cur_min_dis, next_task_index;
    while (cur_task_num < task_num) {
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0) {
                if (min_cost[cur_node][task[t].head] < cur_min_dis && cur_capacity + task[t].demand <= capacity) {
                    cur_min_dis = min_cost[cur_node][task[t].head];
                    next_task_index = t;
                }
            }
        }

        // 头部为起点，且不是第一个任务，则不加入这条任务，重新派一辆车
        // 当前任务到下个任务起点的距离 大于 回到起点并从起点到下个任务的距离，则直接回去
        if (next_task_index != -1) {
//            if ((task[next_task_index].head == depot and !cycle.task_index.empty()) ||
//            min_cost[cur_node][depot] + min_cost[depot][task[next_task_index].head] < cur_min_dis) {
            if (!cycle.task_index.empty() && (task[next_task_index].head == depot ||
                                              min_cost[cur_node][depot] + min_cost[depot][task[next_task_index].head] <=
                                              cur_min_dis)) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
                continue;
            }
            cycle.task_index.push_back(next_task_index);
            cur_capacity += task[next_task_index].demand;

            // 标记为服务过了
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;

            // 回到了起点，派新的车
            if (task[next_task_index].tail == depot) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
            }
        } else {
            // 没有找到可以装得下的任务
            individual.solution.push_back(cycle);
            Cycle temp_cycle;
            cycle = temp_cycle;
            cur_capacity = 0;
            cur_node = depot;
        }
    }

    return individual;
}


Individual greedy_init_individual_min_dis_early_stop_large_demand() {
    Individual individual;
    int cur_task_num = 0, cur_capacity = 0;
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 0;
    }

    Cycle cycle;
    int cur_node = depot, cur_min_dis, next_task_index;
    while (cur_task_num < task_num) {
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0) {
//                if (min_cost[cur_node][task[t].head] < cur_min_dis && cur_capacity + task[t].demand <= capacity) {
                if (cur_capacity + task[t].demand <= capacity) {
                    if (min_cost[cur_node][task[t].head] < cur_min_dis ||
                        (next_task_index != -1 && min_cost[cur_node][task[t].head] == cur_min_dis &&
                         task[t].demand > task[next_task_index].demand)) {
                        cur_min_dis = min_cost[cur_node][task[t].head];
                        next_task_index = t;
                    }
                }
            }
        }

        // 头部为起点，且不是第一个任务，则不加入这条任务，重新派一辆车
        // 当前任务到下个任务起点的距离 大于 回到起点并从起点到下个任务的距离，则直接回去
        if (next_task_index != -1) {
//            if ((task[next_task_index].head == depot and !cycle.task_index.empty()) ||
//            min_cost[cur_node][depot] + min_cost[depot][task[next_task_index].head] < cur_min_dis) {
            if (!cycle.task_index.empty() && (task[next_task_index].head == depot ||
                                              min_cost[cur_node][depot] + min_cost[depot][task[next_task_index].head] <=
                                              cur_min_dis)) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
                continue;
            }
            cycle.task_index.push_back(next_task_index);
            cur_capacity += task[next_task_index].demand;

            // 标记为服务过了
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;

            // 回到了起点，派新的车
            if (task[next_task_index].tail == depot) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
            }
        } else {
            // 没有找到可以装得下的任务
            individual.solution.push_back(cycle);
            Cycle temp_cycle;
            cycle = temp_cycle;
            cur_capacity = 0;
            cur_node = depot;
        }
    }

    return individual;
}


Individual greedy_init_individual(int mode) {
    Individual individual;
    int cur_task_num = 0, cur_capacity = 0;
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 0;
    }

    Cycle cycle;
    int cur_node = depot, cur_min_dis, next_task_index;
    while (cur_task_num < task_num) {
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0 && cur_capacity + task[t].demand <= capacity) {
                // 第t个任务距离cur_node更近，则选第t个任务
                if (min_cost[cur_node][task[t].head] < cur_min_dis) {
                    cur_min_dis = min_cost[cur_node][task[t].head];
                    next_task_index = t;
                } else if (min_cost[cur_node][task[t].head] == cur_min_dis) {
                    // 一样近时，五种方法选择
                    switch (mode) {
                        case 1:
                            if (cost[task[t].head][task[t].tail] * 1.0 / task[t].demand <
                                cost[task[next_task_index].head][task[next_task_index].tail] * 1.0 /
                                task[next_task_index].demand) {
                                next_task_index = t;
                            }
                            break;
                        case 2:
                            if (cost[task[t].head][task[t].tail] * 1.0 / task[t].demand >
                                cost[task[next_task_index].head][task[next_task_index].tail] * 1.0 /
                                task[next_task_index].demand) {
                                next_task_index = t;
                            }
                            break;
                        case 3:
                            if (min_cost[task[t].tail][depot] < min_cost[task[next_task_index].tail][depot]) {
                                next_task_index = t;
                            }
                            break;
                        case 4:
                            if (min_cost[task[t].tail][depot] > min_cost[task[next_task_index].tail][depot]) {
                                next_task_index = t;
                            }
                            break;
                        case 5:
                            if (cur_capacity < capacity / 2) {
                                if (min_cost[task[t].tail][depot] > min_cost[task[next_task_index].tail][depot]) {
                                    next_task_index = t;
                                }
                            } else {
                                if (min_cost[task[t].tail][depot] < min_cost[task[next_task_index].tail][depot]) {
                                    next_task_index = t;
                                }
                            }
                            break;
                    }
                }
            }
        }

        // 头部为起点，且不是第一个任务，则不加入这条任务，重新派一辆车
        // 当前任务到下个任务起点的距离 大于 回到起点并从起点到下个任务的距离，则直接回去
        if (next_task_index != -1) {
//            if ((task[next_task_index].head == depot and !cycle.task_index.empty()) ||
//            min_cost[cur_node][depot] + min_cost[depot][task[next_task_index].head] < cur_min_dis) {
            if (!cycle.task_index.empty() && (task[next_task_index].head == depot ||
                                              min_cost[cur_node][depot] +
                                              min_cost[depot][task[next_task_index].head] <=
                                              cur_min_dis)) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
                continue;
            }
            cycle.task_index.push_back(next_task_index);
            cur_capacity += task[next_task_index].demand;

            // 标记为服务过了
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;

            // 回到了起点，派新的车 || 所有任务都完成了，不加会漏掉最后一条回路
            if (task[next_task_index].tail == depot || cur_task_num == task_num) {
                individual.solution.push_back(cycle);
                Cycle temp_cycle;
                cycle = temp_cycle;
                cur_capacity = 0;
                cur_node = depot;
            }
        } else {
            // 没有找到可以装得下的任务
            individual.solution.push_back(cycle);
            Cycle temp_cycle;
            cycle = temp_cycle;
            cur_capacity = 0;
            cur_node = depot;
        }
    }

    return individual;
}


void calc_cost(Individual &individual) {
    int total_cost = 0, demand_so_far = 0;
    for (auto c : individual.solution) {
        int cur_cost = 0, cur_demand = 0;
        int pre_node = depot;
        for (auto t : c.task_index) {
            cur_cost += min_cost[pre_node][task[t].head];
            cur_cost += cost[task[t].head][task[t].tail];
            cur_demand += task[t].demand;
            pre_node = task[t].tail;
        }

        cur_cost += min_cost[pre_node][depot];
        c.cycle_cost = cur_cost;
        total_cost += cur_cost;

        if (cur_demand > capacity) {
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! capacity" <<endl;
        }
        c.cycle_demand = cur_demand;
        demand_so_far += cur_demand;
    }
    if (demand_so_far != total_demand) {
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! total_demand" <<endl;
    }

    individual.total_cost = total_cost;
}


void run() {
    for (int file_index = 0; file_index < file_num; file_index++) {
        cout << input_files[file_index] << endl;
        read_data(file_index);
        floyd();

        for (int m = 1; m <= 5; m++) {
//            Individual individual = greedy_init_individual(m);
            Individual individual = greedy_init_individual(m);
            calc_cost(individual);
            cout << individual.total_cost << " " << individual.solution.size() << ", ";
        }
        cout << endl;
    }
//    for (const auto& c : individual.solution) {
//        for (int t : c.task_index) {
//            cout << task[t].head << " " << task[t].tail << " ";
//        }
//        cout << endl;
//    }
}


int main() {
    run();
    return 0;
}

#include <iostream>
#include <algorithm>
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

vector<Individual> population;

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

vector<vector<int>> split_result;
vector<int> large_cycle;

char dummy_string[50];

//char input_file[100] = "../instance/gdb/gdb1.dat"; // the instance can be changed here
char input_files[][100] = {
        {"../instance/gdb/gdb9.dat"},
        {"../instance/gdb/gdb13.dat"},
//        {"../instance/egl/egl-e1-A.dat"},
};

char input_files1[][100] = {
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
                task[i * 2 - 1].head = temp_node2;
                task[i * 2 - 1].tail = temp_node1;
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


void ulusoy_split() {
    split_result.clear();
    int cycle_task_num = large_cycle.size();
    int dis[cycle_task_num * 2][cycle_task_num * 2];
    for (int i = 0; i < cycle_task_num * 2; i++) {
        for (int j = 0; j < cycle_task_num * 2; j++) {
            if (i == j || (i % 2 == 1 && i == j - 1)) {
                dis[i][j] = 0;
            } else {
                dis[i][j] = INF;
            }
        }
    }

    int cur_demand, cur_cost, pre_node;
    for (int start_index = 0; start_index < cycle_task_num; start_index++) {
        cur_demand = 0;
        cur_cost = 0;
        for (int end_index = start_index; end_index < cycle_task_num; end_index++) {
            pre_node = start_index == end_index ? depot : task[large_cycle[end_index - 1]].tail;
            cur_demand += task[large_cycle[end_index]].demand;
            if (cur_demand <= capacity) {
                cur_cost += min_cost[pre_node][task[large_cycle[end_index]].head];
                cur_cost += cost[task[large_cycle[end_index]].head][task[large_cycle[end_index]].tail];
                dis[start_index * 2][end_index * 2 + 1] = cur_cost + min_cost[task[large_cycle[end_index]].tail][depot];
            } else {
                break;
            }
        }
    }

    // Dijkstra
    int min_dis[cycle_task_num * 2], p[cycle_task_num * 2], cur_min, u;
    bool book[cycle_task_num * 2];
    for (int i = 0; i < cycle_task_num * 2; i++) {
        min_dis[i] = dis[0][i];
        p[i] = 0;
        book[i] = false;
    }
    book[0] = true;
    for (int i = 0; i < cycle_task_num * 2; i++) {
        cur_min = INF;
        for (int j = 0; j < cycle_task_num * 2; j++) {
            if (!book[j] && min_dis[j] < cur_min) {
                cur_min = min_dis[j];
                u = j;
            }
        }
        book[u] = true;

        for (int v = 0; v < cycle_task_num * 2; v++) {
            if (!book[v] && dis[u][v] < INF) {
                if (min_dis[v] > min_dis[u] + dis[u][v]) {
                    min_dis[v] = min_dis[u] + dis[u][v];
                    p[v] = u;
                }
            }
        }
    }
//
//    for (int i = 0; i < cycle_task_num * 2; i++) {
//        for (int j = 0; j < cycle_task_num * 2; j++) {
//            if (dis[i][j] != 1061109747) {
//                cout << dis[i][j] << "\t";
//            } else {
//                cout << "-\t";
//            }
//        }
//        cout << endl;
//    }
//    cout << endl;
//    for (int i = 0; i < cycle_task_num * 2; i++) {
//        cout << min_dis[i] << ", ";
//    }
//    cout << endl;
//    cout << endl;
//    for (int i = 0; i < cycle_task_num * 2; i++) {
//        cout << p[i] << ", ";
//    }
//
    int pre = p[cycle_task_num * 2 - 1];
//    while (pre != 0) {
//        cout << pre << ", ";
//        pre = p[pre];
//    }
//    cout << endl;
    vector<int> min_path;
    min_path.push_back(cycle_task_num * 2 - 1);
    pre = p[cycle_task_num * 2 - 1];
    while (pre != 0) {
        min_path.push_back(pre);
        pre = p[pre];
    }
    min_path.push_back(0);
    for (int i = min_path.size() - 1; i >= 0; i -= 2) {
        vector<int> temp_cycle;
        for (int j = min_path[i] / 2; j < min_path[i - 1] / 2 + 1; j++) {
            temp_cycle.push_back(large_cycle[j]);
        }
        split_result.push_back(temp_cycle);
    }

    // 分割
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
                // 有一定概率不选最短的
                if (min_cost[cur_node][task[t].head] < cur_min_dis &&
                    (next_task_index == -1 || random_num(10) < 8)) {
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
                        default:
                            // 随机选五个中的一个
                            switch (random_num(5) + 1) {
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
                                    if (min_cost[task[t].tail][depot] <
                                        min_cost[task[next_task_index].tail][depot]) {
                                        next_task_index = t;
                                    }
                                    break;
                                case 4:
                                    if (min_cost[task[t].tail][depot] >
                                        min_cost[task[next_task_index].tail][depot]) {
                                        next_task_index = t;
                                    }
                                    break;
                                case 5:
                                    if (cur_capacity < capacity / 2) {
                                        if (min_cost[task[t].tail][depot] >
                                            min_cost[task[next_task_index].tail][depot]) {
                                            next_task_index = t;
                                        }
                                    } else {
                                        if (min_cost[task[t].tail][depot] <
                                            min_cost[task[next_task_index].tail][depot]) {
                                            next_task_index = t;
                                        }
                                    }
                                    break;
                            }
                    }
                }
            }
        }

        // 头部为起点，且不是第一个任务，则不加入这条任务，重新派一辆车
        // 当前任务到下个任务起点的距离 大于 回到起点并从起点到下个任务的距离，则直接回去
        if (next_task_index != -1) {
//            if ((task[next_task_index].head == depot and !cycle.task_index.empty()) ||
//            min_cost[cur_node][depot] + min_cost[depot][task[next_task_index].head] < cur_min_dis) {
            if ((!cycle.task_index.empty() && (task[next_task_index].head == depot ||
                                               min_cost[cur_node][depot] +
                                               min_cost[depot][task[next_task_index].head] <=
                                               cur_min_dis)) ||
                (cur_capacity > capacity * 4 / 5) && random_num(10) < 2) {
                // 容量大于4/5时有一定概率直接作为新的回路
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


Individual greedy_init_individual_split(int mode) {
    int cur_task_num = 0;
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 0;
    }
    large_cycle.clear();

    Cycle cycle;
    int cur_node = depot, cur_min_dis, next_task_index;
    while (cur_task_num < task_num) {
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0) {
                // 第t个任务距离cur_node更近，则选第t个任务
                // 有一定概率不选最短的
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
                            if (random_num(2) < 1) {
                                if (min_cost[task[t].tail][depot] > min_cost[task[next_task_index].tail][depot]) {
                                    next_task_index = t;
                                }
                            } else {
                                if (min_cost[task[t].tail][depot] < min_cost[task[next_task_index].tail][depot]) {
                                    next_task_index = t;
                                }
                            }
                            break;
                        default:
                            // 随机选五个中的一个
                            switch (random_num(5) + 1) {
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
                                    if (min_cost[task[t].tail][depot] <
                                        min_cost[task[next_task_index].tail][depot]) {
                                        next_task_index = t;
                                    }
                                    break;
                                case 4:
                                    if (min_cost[task[t].tail][depot] >
                                        min_cost[task[next_task_index].tail][depot]) {
                                        next_task_index = t;
                                    }
                                    break;
                                case 5:
                                    if (random_num(2) < 1) {
                                        if (min_cost[task[t].tail][depot] >
                                            min_cost[task[next_task_index].tail][depot]) {
                                            next_task_index = t;
                                        }
                                    } else {
                                        if (min_cost[task[t].tail][depot] <
                                            min_cost[task[next_task_index].tail][depot]) {
                                            next_task_index = t;
                                        }
                                    }
                                    break;
                            }
                    }
                }
            }
        }

        // 头部为起点，且不是第一个任务，则不加入这条任务，重新派一辆车
        // 当前任务到下个任务起点的距离 大于 回到起点并从起点到下个任务的距离，则直接回去
        if (next_task_index != -1) {
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;
            large_cycle.push_back(next_task_index);
        }
    }


    Individual individual;
    ulusoy_split();
    for (int i = 0; i < split_result.size(); i++) {
        Cycle temp_c;
        for (int j = 0; j < split_result[i].size(); j++) {
            temp_c.task_index.push_back(split_result[i][j]);
        }
        individual.solution.push_back(temp_c);
    }

    return individual;
}


void calc_cost(Individual &individual) {
    int total_cost = 0, demand_so_far = 0;
//    for (auto c : individual.solution) {
    for (int cycle_num = 0; cycle_num < individual.solution.size(); cycle_num++) {
        int cur_cost = 0, cur_demand = 0;
        int pre_node = depot;
        for (auto t : individual.solution[cycle_num].task_index) {
            cur_cost += min_cost[pre_node][task[t].head];
            cur_cost += cost[task[t].head][task[t].tail];
            cur_demand += task[t].demand;
            pre_node = task[t].tail;
        }

        cur_cost += min_cost[pre_node][depot];
        individual.solution[cycle_num].cycle_cost = cur_cost;
        total_cost += cur_cost;

        if (cur_demand > capacity) {
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! capacity: " << cur_demand << ", " << capacity << endl;
        }
        individual.solution[cycle_num].cycle_demand = cur_demand;
        demand_so_far += cur_demand;
    }
    if (demand_so_far != total_demand) {
        cout << demand_so_far << ", dd " << total_demand << endl;
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! total_demand" << endl;
    }

    individual.total_cost = total_cost;
}


bool reverse_head_tail(Individual &individual) {
    int pre_node, next_node, cycle_len, cost_change;
    bool improved = true, improved_so_far = false;
    while (improved) {
        improved = false;
        for (int cycle_num = 0; cycle_num < individual.solution.size(); cycle_num++) {
            cycle_len = individual.solution[cycle_num].task_index.size();
            for (int i = 0; i < cycle_len; i++) {
                pre_node = i == 0 ? depot : task[individual.solution[cycle_num].task_index[i - 1]].tail;
                next_node =
                        i == cycle_len - 1 ? depot : task[individual.solution[cycle_num].task_index[i + 1]].head;
                // 现在的cost减去变化后的cost，如果大于0，说明变化能减少cost
                cost_change = min_cost[pre_node][task[individual.solution[cycle_num].task_index[i]].head] +
                              min_cost[task[individual.solution[cycle_num].task_index[i]].tail][next_node]
                              - min_cost[pre_node][task[individual.solution[cycle_num].task_index[i]].tail] -
                              min_cost[task[individual.solution[cycle_num].task_index[i]].head][next_node];

                if (cost_change > 0) {
                    if (individual.solution[cycle_num].task_index[i] % 2 == 0) {
                        individual.solution[cycle_num].task_index[i] =
                                individual.solution[cycle_num].task_index[i] + 1;
                    } else {
                        individual.solution[cycle_num].task_index[i] =
                                individual.solution[cycle_num].task_index[i] - 1;
                    }
                    improved = true;
                    improved_so_far = true;
                }
            }
        }
    }

    return improved_so_far;
}


bool reverse_task(Individual &individual) {
    int pre_node, next_node, cycle_len, cost_change, l, r, temp_l, temp_r, temp;
    bool improved = true, improved_so_far = false;
    while (improved) {
        improved = false;
        for (int cycle_num = 0; cycle_num < individual.solution.size(); cycle_num++) {
            cycle_len = individual.solution[cycle_num].task_index.size();
            for (int reverse_len = 1; reverse_len < cycle_len - 1; reverse_len++) {
                for (int start = 0; start + reverse_len < cycle_len; start++) {
                    pre_node = start == 0 ? depot : task[individual.solution[cycle_num].task_index[start - 1]].tail;
                    next_node =
                            start + reverse_len == cycle_len - 1 ? depot
                                                                 : task[individual.solution[cycle_num].task_index[
                                    start + reverse_len +
                                    1]].head;
                    cost_change = min_cost[pre_node][task[individual.solution[cycle_num].task_index[start]].head] +
                                  min_cost[task[individual.solution[cycle_num].task_index[start +
                                                                                          reverse_len]].tail][next_node]
                                  - min_cost[pre_node][task[individual.solution[cycle_num].task_index[start +
                                                                                                      reverse_len]].tail] -
                                  min_cost[task[individual.solution[cycle_num].task_index[start]].head][next_node];

                    if (cost_change > 0) {
                        l = start;
                        r = start + reverse_len;
                        while (l < r) {
                            temp_l = individual.solution[cycle_num].task_index[l];
                            temp_r = individual.solution[cycle_num].task_index[r];
                            temp_l = temp_l % 2 == 0 ? temp_l + 1 : temp_l - 1;
                            temp_r = temp_r % 2 == 0 ? temp_r + 1 : temp_r - 1;
                            individual.solution[cycle_num].task_index[l] = temp_r;
                            individual.solution[cycle_num].task_index[r] = temp_l;
                            l++;
                            r--;
                        }

                        // 奇数个时，中间的需要单独reverse
                        if (l == r) {
                            temp_l = individual.solution[cycle_num].task_index[l];
                            temp_l = temp_l % 2 == 0 ? temp_l + 1 : temp_l - 1;
                            individual.solution[cycle_num].task_index[l] = temp_l;
                        }
                        improved = true;
                        improved_so_far = true;
                    }
                }
            }
        }
    }

    return improved_so_far;
}


bool swap_in_cycle(Individual &individual) {
    int l_pre_node, l_next_node, r_pre_node, r_next_node, cycle_len, cost_change, temp, l_head, l_tail, r_head, r_tail;
    bool improved = true, improved_so_far = false;
    while (improved) {
        improved = false;
        for (int cycle_num = 0; cycle_num < individual.solution.size(); cycle_num++) {
            cycle_len = individual.solution[cycle_num].task_index.size();
            for (int left = 0; left < cycle_len - 2; left++) {
                for (int right = left + 2; right < cycle_len; right++) {
                    l_pre_node = left == 0 ? depot : task[individual.solution[cycle_num].task_index[left - 1]].tail;
                    l_next_node = task[individual.solution[cycle_num].task_index[left + 1]].head;
                    r_pre_node = task[individual.solution[cycle_num].task_index[right - 1]].tail;
                    r_next_node =
                            right == cycle_len - 1 ? depot : task[individual.solution[cycle_num].task_index[right +
                                                                                                            1]].head;
                    l_head = task[individual.solution[cycle_num].task_index[left]].head;
                    l_tail = task[individual.solution[cycle_num].task_index[left]].tail;
                    r_head = task[individual.solution[cycle_num].task_index[right]].head;
                    r_tail = task[individual.solution[cycle_num].task_index[right]].tail;

                    cost_change = min_cost[l_pre_node][l_head] + min_cost[l_tail][l_next_node] +
                                  min_cost[r_pre_node][r_head] + min_cost[r_tail][r_next_node] -
                                  min_cost[l_pre_node][r_head] - min_cost[r_tail][l_next_node] -
                                  min_cost[r_pre_node][l_head] - min_cost[l_tail][r_next_node];

                    if (cost_change > 0) {
                        temp = individual.solution[cycle_num].task_index[left];
                        individual.solution[cycle_num].task_index[left] = individual.solution[cycle_num].task_index[right];
                        individual.solution[cycle_num].task_index[right] = temp;
                        improved = true;
                        improved_so_far = true;
                    }
                }
            }
        }
    }

    return improved_so_far;
}


bool swap_between_cycle(Individual &individual) {
    calc_cost(individual);
    int fir_cycle_num, sec_cycle_num, fir_cycle_len, sec_cycle_len, fir_task, sec_task,
            fir_cycle_demand, sec_cycle_demand, fir_task_demand, sec_task_demand,
            fir_pre_node, fir_next_node, sec_pre_node, sec_next_node, cost_change,
            fir_head, fir_tail, sec_head, sec_tail, temp;
    bool improved = true, improved_so_far = false;
    while (improved) {
        improved = false;
        for (fir_cycle_num = 0; fir_cycle_num < individual.solution.size() - 1; fir_cycle_num++) {
//            calc_cost(individual);
            fir_cycle_len = individual.solution[fir_cycle_num].task_index.size();
            for (fir_task = 0; fir_task < fir_cycle_len; fir_task++) {
                fir_pre_node =
                        fir_task == 0 ? depot : task[individual.solution[fir_cycle_num].task_index[fir_task -
                                                                                                   1]].tail;
                fir_next_node =
                        fir_task == fir_cycle_len - 1 ? depot : task[individual.solution[fir_cycle_num].task_index[
                                fir_task + 1]].head;
                fir_head = task[individual.solution[fir_cycle_num].task_index[fir_task]].head;
                fir_tail = task[individual.solution[fir_cycle_num].task_index[fir_task]].tail;
                fir_cycle_demand = individual.solution[fir_cycle_num].cycle_demand;
                fir_task_demand = task[individual.solution[fir_cycle_num].task_index[fir_task]].demand;
                for (sec_cycle_num = fir_cycle_num + 1;
                     sec_cycle_num < individual.solution.size() && !improved; sec_cycle_num++) {
                    sec_cycle_len = individual.solution[sec_cycle_num].task_index.size();
                    for (sec_task = 0; sec_task < sec_cycle_len && !improved; sec_task++) {
                        sec_cycle_demand = individual.solution[sec_cycle_num].cycle_demand;
                        sec_task_demand = task[individual.solution[sec_cycle_num].task_index[sec_task]].demand;
                        if (fir_cycle_demand - fir_task_demand + sec_task_demand <= capacity &&
                            sec_cycle_demand - sec_task_demand + fir_task_demand <= capacity) {
                            sec_pre_node =
                                    sec_task == 0 ? depot : task[individual.solution[sec_cycle_num].task_index[
                                            sec_task -
                                            1]].tail;
                            sec_next_node = sec_task == sec_cycle_len - 1 ? depot
                                                                          : task[individual.solution[sec_cycle_num].task_index[
                                            sec_task + 1]].head;
                            sec_head = task[individual.solution[sec_cycle_num].task_index[sec_task]].head;
                            sec_tail = task[individual.solution[sec_cycle_num].task_index[sec_task]].tail;
                            cost_change = min_cost[fir_pre_node][fir_head] + min_cost[fir_tail][fir_next_node] +
                                          min_cost[sec_pre_node][sec_head] + min_cost[sec_tail][sec_next_node] -
                                          min_cost[fir_pre_node][sec_head] - min_cost[sec_tail][fir_next_node] -
                                          min_cost[sec_pre_node][fir_head] - min_cost[fir_tail][sec_next_node];
                            if (cost_change > 0) {
                                temp = individual.solution[fir_cycle_num].task_index[fir_task];
                                individual.solution[fir_cycle_num].task_index[fir_task] =
                                        individual.solution[sec_cycle_num].task_index[sec_task];
                                individual.solution[sec_cycle_num].task_index[sec_task] = temp;

                                individual.solution[fir_cycle_num].cycle_demand =
                                        fir_cycle_demand - fir_task_demand + sec_task_demand;
                                individual.solution[sec_cycle_num].cycle_demand =
                                        sec_cycle_demand - sec_task_demand + fir_task_demand;
                                improved = true;
                                improved_so_far = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return improved_so_far;
}


bool insert_between_cycle(Individual &individual) {
    calc_cost(individual);
    int fir_cycle_num, sec_cycle_num, fir_cycle_len, sec_cycle_len, fir_task, sec_task,
            fir_cycle_demand, sec_cycle_demand, fir_task_demand, sec_task_demand,
            fir_pre_node, fir_next_node, sec_pre_node, cost_change,
            fir_head, fir_tail, sec_head, temp;
    bool improved = true, improved_so_far = false;
    while (improved) {
        improved = false;
        for (fir_cycle_num = 0; fir_cycle_num < individual.solution.size(); fir_cycle_num++) {
//            calc_cost(individual);
            fir_cycle_len = individual.solution[fir_cycle_num].task_index.size();
            for (fir_task = 0; fir_task < fir_cycle_len; fir_task++) {
                fir_pre_node =
                        fir_task == 0 ? depot : task[individual.solution[fir_cycle_num].task_index[fir_task -
                                                                                                   1]].tail;
                fir_next_node =
                        fir_task == fir_cycle_len - 1 ? depot : task[individual.solution[fir_cycle_num].task_index[
                                fir_task + 1]].head;
                fir_head = task[individual.solution[fir_cycle_num].task_index[fir_task]].head;
                fir_tail = task[individual.solution[fir_cycle_num].task_index[fir_task]].tail;
                fir_cycle_demand = individual.solution[fir_cycle_num].cycle_demand;
                fir_task_demand = task[individual.solution[fir_cycle_num].task_index[fir_task]].demand;
                for (sec_cycle_num = 0;
                     sec_cycle_num != fir_cycle_num && sec_cycle_num < individual.solution.size() &&
                     !improved; sec_cycle_num++) {
                    sec_cycle_demand = individual.solution[sec_cycle_num].cycle_demand;
                    if (sec_cycle_demand + fir_task_demand <= capacity) {
                        sec_cycle_len = individual.solution[sec_cycle_num].task_index.size();
                        for (sec_task = 0; sec_task < sec_cycle_len && !improved; sec_task++) {
                            sec_pre_node =
                                    sec_task == 0 ? depot : task[individual.solution[sec_cycle_num].task_index[
                                            sec_task -
                                            1]].tail;
                            sec_head = task[individual.solution[sec_cycle_num].task_index[sec_task]].head;
                            cost_change = min_cost[fir_pre_node][fir_head] + min_cost[fir_tail][fir_next_node] +
                                          min_cost[sec_pre_node][sec_head] - min_cost[fir_pre_node][fir_next_node] -
                                          min_cost[sec_pre_node][fir_head] - min_cost[fir_tail][sec_head];
                            if (cost_change > 0) {
                                temp = individual.solution[fir_cycle_num].task_index[fir_task];
                                individual.solution[fir_cycle_num].task_index.erase(
                                        individual.solution[fir_cycle_num].task_index.begin() + fir_task);
                                individual.solution[sec_cycle_num].task_index.insert(
                                        individual.solution[sec_cycle_num].task_index.begin() + sec_task, temp);
                                individual.solution[fir_cycle_num].cycle_demand =
                                        fir_cycle_demand - fir_task_demand;
                                individual.solution[sec_cycle_num].cycle_demand =
                                        sec_cycle_demand + fir_task_demand;
                                fir_cycle_len--;
                                improved = true;
                                improved_so_far = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return improved_so_far;
}


bool individual_ulusoy_split(Individual &individual) {
    large_cycle.clear();
    calc_cost(individual);
    int before = individual.total_cost;
    for (int t = 0; t < individual.solution.size(); t++) {
        for (int i = 0; i < individual.solution[t].task_index.size(); i++) {
            large_cycle.push_back(individual.solution[t].task_index[i]);
        }
    }

    ulusoy_split();

    individual.solution.clear();
    for (int i = 0; i < split_result.size(); i++) {
        Cycle temp_c;
        for (int j = 0; j < split_result[i].size(); j++) {
            temp_c.task_index.push_back(split_result[i][j]);
        }
        individual.solution.push_back(temp_c);
    }
    calc_cost(individual);
    int after = individual.total_cost;
    if (before < after) {
        cout << "NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN" << endl;
    }

    return before < after;
}


void merge(vector<int> &two_cycle_task, int mode, bool direction) {
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 1;
    }

    for (int t = 0; t < two_cycle_task.size(); t++) {
        served[two_cycle_task[t]] = 0;
        if (!direction) {
            if (two_cycle_task[t] % 2 == 0) {
                served[two_cycle_task[t] + 1] = 0;
            } else {
                served[two_cycle_task[t] - 1] = 0;
            }
        }
    }

    large_cycle.clear();

    int cur_node = depot, cur_min_dis, next_task_index, cur_task_num = 0;
    while (cur_task_num < two_cycle_task.size()) {
//        cout << cur_task_num << ", " << two_cycle_task.size() << endl;
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0) {
                // 第t个任务距离cur_node更近，则选第t个任务
                // 有一定概率不选最短的
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
                            if (random_num(2) < 1) {
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
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;
            large_cycle.push_back(next_task_index);
        }
    }

    ulusoy_split();
}


void merge_with_capacity(vector<int> &two_cycle_task, int mode, bool direction) {
    for (int i = 0; i < task_num * 2; i++) {
        served[i] = 1;
    }

    for (int t = 0; t < two_cycle_task.size(); t++) {
        served[two_cycle_task[t]] = 0;
        if (!direction) {
            if (two_cycle_task[t] % 2 == 0) {
                served[two_cycle_task[t] + 1] = 0;
            } else {
                served[two_cycle_task[t] - 1] = 0;
            }
        }
    }

    large_cycle.clear();

    int cur_node = depot, cur_min_dis, next_task_index, cur_task_num = 0, cur_capacity = 0;
    while (cur_task_num < two_cycle_task.size()) {
//        cout << cur_task_num << ", " << two_cycle_task.size() << endl;
        cur_min_dis = INF;
        next_task_index = -1;
        for (int t = 0; t < task_num * 2; t++) {
            if (served[t] == 0 && cur_capacity + task[t].demand <= capacity) {
                // 第t个任务距离cur_node更近，则选第t个任务
                // 有一定概率不选最短的
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
                            if (random_num(2) < 1) {
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
            served[next_task_index] = 1;
            if (next_task_index % 2 == 0) {
                served[next_task_index + 1] = 1;
            } else {
                served[next_task_index - 1] = 1;
            }
            cur_task_num++;
            cur_node = task[next_task_index].tail;
            large_cycle.push_back(next_task_index);
            cur_capacity += task[next_task_index].demand;
        } else {
            cur_capacity = 0;
            cur_node = depot;
        }
    }

    ulusoy_split();
}


int calc_split_result() {
    int total_cost = 0;
//    for (auto c : individual.solution) {
    for (int cycle_num = 0; cycle_num < split_result.size(); cycle_num++) {
        int cur_cost = 0;
        int pre_node = depot;
        for (auto t : split_result[cycle_num]) {
            cur_cost += min_cost[pre_node][task[t].head];
            cur_cost += cost[task[t].head][task[t].tail];
            pre_node = task[t].tail;
        }

        cur_cost += min_cost[pre_node][depot];
        total_cost += cur_cost;
    }

    return total_cost;
}


bool merge_split(Individual &individual) {
    vector<int> two_cycle_task;
    vector<vector<int>> temp_split_rst;
    int best_split_cost = INF, old_cost;
    for (int i = 0; i < individual.solution.size() - 1; i++) {
        for (int j = i + 1; j < individual.solution.size(); j++) {
            best_split_cost = INF;
            two_cycle_task.clear();
            split_result.clear();
            vector<int> temp_cycle1;
            vector<int> temp_cycle2;
            for (int t = 0; t < individual.solution[i].task_index.size(); t++) {
                two_cycle_task.push_back(individual.solution[i].task_index[t]);
                temp_cycle1.push_back(individual.solution[i].task_index[t]);
            }
            split_result.push_back(temp_cycle1);
            for (int t = 0; t < individual.solution[j].task_index.size(); t++) {
                two_cycle_task.push_back(individual.solution[j].task_index[t]);
                temp_cycle2.push_back(individual.solution[j].task_index[t]);
            }
            split_result.push_back(temp_cycle2);
            best_split_cost = calc_split_result();
            old_cost = best_split_cost;
            split_result.clear();

            for (int m = 1; m <= 5; m++) {
                merge(two_cycle_task, m, true);
                int temp = calc_split_result();
                if (temp < best_split_cost) {
                    best_split_cost = temp;
                    temp_split_rst.clear();
                    for (int c = 0; c < split_result.size(); c++) {
                        vector<int> temp_cycle;
                        for (int t = 0; t < split_result[c].size(); t++) {
                            temp_cycle.push_back(split_result[c][t]);
                        }
                        temp_split_rst.push_back(temp_cycle);
                    }
                }
            }

            for (int m = 1; m <= 5; m++) {
                merge(two_cycle_task, m, false);
                int temp = calc_split_result();
                if (temp < best_split_cost) {
                    best_split_cost = temp;
                    temp_split_rst.clear();
                    for (int c = 0; c < split_result.size(); c++) {
                        vector<int> temp_cycle;
                        for (int t = 0; t < split_result[c].size(); t++) {
                            temp_cycle.push_back(split_result[c][t]);
                        }
                        temp_split_rst.push_back(temp_cycle);
                    }
                }
            }


            for (int m = 1; m <= 5; m++) {
                merge_with_capacity(two_cycle_task, m, false);
                int temp = calc_split_result();
                if (temp < best_split_cost) {
                    best_split_cost = temp;
                    temp_split_rst.clear();
                    for (int c = 0; c < split_result.size(); c++) {
                        vector<int> temp_cycle;
                        for (int t = 0; t < split_result[c].size(); t++) {
                            temp_cycle.push_back(split_result[c][t]);
                        }
                        temp_split_rst.push_back(temp_cycle);
                    }
                }
            }


            for (int m = 1; m <= 5; m++) {
                merge_with_capacity(two_cycle_task, m, true);
                int temp = calc_split_result();
                if (temp < best_split_cost) {
                    best_split_cost = temp;
                    temp_split_rst.clear();
                    for (int c = 0; c < split_result.size(); c++) {
                        vector<int> temp_cycle;
                        for (int t = 0; t < split_result[c].size(); t++) {
                            temp_cycle.push_back(split_result[c][t]);
                        }
                        temp_split_rst.push_back(temp_cycle);
                    }
                }
            }
//            cout << "--------------------1" << endl;
//            for (int c = 0; c < temp_split_rst.size(); c++) {
//                for (int t = 0; t < temp_split_rst[c].size(); t++) {
//                    cout << temp_split_rst[c][t] << ", ";
//                }
//                cout << endl;
//            }
//            cout << "--------------------2" << endl;
//            calc_cost(individual);
//            int before = individual.total_cost;
            if (best_split_cost < old_cost) {
                if (temp_split_rst.size() == 2) {
                    individual.solution[i].task_index.clear();
                    for (int t = 0; t < temp_split_rst[0].size(); t++) {
                        individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                    }
                    individual.solution[j].task_index.clear();
                    for (int t = 0; t < temp_split_rst[1].size(); t++) {
                        individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                    }
                } else if (temp_split_rst.size() == 1) {
//                cout << "1111111111111111111111111111111111111" << endl;
                    individual.solution[i].task_index.clear();
                    for (int t = 0; t < temp_split_rst[0].size(); t++) {
                        individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                    }
//                individual.solution[j].task_index.clear();
                    individual.solution.erase(individual.solution.begin() + j);
                } else if (temp_split_rst.size() > 2) {
                    individual.solution[i].task_index.clear();
                    for (int t = 0; t < temp_split_rst[0].size(); t++) {
                        individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                    }
                    individual.solution[j].task_index.clear();
                    for (int t = 0; t < temp_split_rst[1].size(); t++) {
                        individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                    }
                    for (int t = 2; t < temp_split_rst.size(); t++) {
                        Cycle cycle;
                        for (int task_i = 0; task_i < temp_split_rst[t].size(); task_i++) {
                            cycle.task_index.push_back(temp_split_rst[t][task_i]);
                        }
                        individual.solution.push_back(cycle);
                    }
                }
            }
//            calc_cost(individual);
//            int after = individual.total_cost;
//            if (before > after) {
//                cout << "wefhjopasdijfhioasdjufljadfal;" << before << ", " << after << endl;
//            }
        }
    }

    return true;
}


bool merge_split_three(Individual &individual) {
    vector<int> two_cycle_task;
    vector<vector<int>> temp_split_rst;
    int best_split_cost = INF, old_cost;
    for (int i = 0; i < individual.solution.size() - 2; i++) {
        for (int j = i + 1; j < individual.solution.size() - 1; j++) {
            for (int k = j + 1; k < individual.solution.size(); k++) {
                best_split_cost = INF;
                two_cycle_task.clear();
                split_result.clear();
                vector<int> temp_cycle1;
                vector<int> temp_cycle2;
                vector<int> temp_cycle3;
                for (int t = 0; t < individual.solution[i].task_index.size(); t++) {
                    two_cycle_task.push_back(individual.solution[i].task_index[t]);
                    temp_cycle1.push_back(individual.solution[i].task_index[t]);
                }
                split_result.push_back(temp_cycle1);

                for (int t = 0; t < individual.solution[j].task_index.size(); t++) {
                    two_cycle_task.push_back(individual.solution[j].task_index[t]);
                    temp_cycle2.push_back(individual.solution[j].task_index[t]);
                }
                split_result.push_back(temp_cycle2);

                for (int t = 0; t < individual.solution[k].task_index.size(); t++) {
                    two_cycle_task.push_back(individual.solution[k].task_index[t]);
                    temp_cycle3.push_back(individual.solution[k].task_index[t]);
                }
                split_result.push_back(temp_cycle3);

                best_split_cost = calc_split_result();
                old_cost = best_split_cost;
                split_result.clear();

                for (int m = 1; m <= 5; m++) {
                    merge(two_cycle_task, m, true);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }

                for (int m = 1; m <= 5; m++) {
                    merge(two_cycle_task, m, false);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }


                for (int m = 1; m <= 5; m++) {
                    merge_with_capacity(two_cycle_task, m, false);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }


                for (int m = 1; m <= 5; m++) {
                    merge_with_capacity(two_cycle_task, m, true);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }
//            cout << "--------------------1" << endl;
//            for (int c = 0; c < temp_split_rst.size(); c++) {
//                for (int t = 0; t < temp_split_rst[c].size(); t++) {
//                    cout << temp_split_rst[c][t] << ", ";
//                }
//                cout << endl;
//            }
//            cout << "--------------------2" << endl;
//            calc_cost(individual);
//            int before = individual.total_cost;
                if (best_split_cost < old_cost) {
                    if (temp_split_rst.size() == 3) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution[j].task_index.clear();
                        for (int t = 0; t < temp_split_rst[1].size(); t++) {
                            individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                        }
                        individual.solution[k].task_index.clear();
                        for (int t = 0; t < temp_split_rst[2].size(); t++) {
                            individual.solution[k].task_index.push_back(temp_split_rst[2][t]);
                        }
                    } else if (temp_split_rst.size() == 1) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution.erase(individual.solution.begin() + k);
                        individual.solution.erase(individual.solution.begin() + j);
                    } else if (temp_split_rst.size() == 2) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution[j].task_index.clear();
                        for (int t = 0; t < temp_split_rst[1].size(); t++) {
                            individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                        }
                        individual.solution.erase(individual.solution.begin() + k);
                    } else if (temp_split_rst.size() > 3) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution[j].task_index.clear();
                        for (int t = 0; t < temp_split_rst[1].size(); t++) {
                            individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                        }
                        individual.solution[k].task_index.clear();
                        for (int t = 0; t < temp_split_rst[2].size(); t++) {
                            individual.solution[k].task_index.push_back(temp_split_rst[2][t]);
                        }
                        for (int t = 3; t < temp_split_rst.size(); t++) {
                            Cycle cycle;
                            for (int task_i = 0; task_i < temp_split_rst[t].size(); task_i++) {
                                cycle.task_index.push_back(temp_split_rst[t][task_i]);
                            }
                            individual.solution.push_back(cycle);
                        }
                    }
                }
            }
//            calc_cost(individual);
//            int after = individual.total_cost;
//            if (before > after) {
//                cout << "wefhjopasdijfhioasdjufljadfal;" << before << ", " << after << endl;
//            }
        }
    }

    return true;
}



bool merge_split_four(Individual &individual) {
    vector<int> two_cycle_task;
    vector<vector<int>> temp_split_rst;
    int best_split_cost = INF, old_cost;
    for (int i = 0; i < individual.solution.size() - 2; i++) {
        for (int j = i + 1; j < individual.solution.size() - 1; j++) {
            for (int k = j + 1; k < individual.solution.size(); k++) {
                best_split_cost = INF;
                two_cycle_task.clear();
                split_result.clear();
                vector<int> temp_cycle1;
                vector<int> temp_cycle2;
                vector<int> temp_cycle3;
                for (int t = 0; t < individual.solution[i].task_index.size(); t++) {
                    two_cycle_task.push_back(individual.solution[i].task_index[t]);
                    temp_cycle1.push_back(individual.solution[i].task_index[t]);
                }
                split_result.push_back(temp_cycle1);

                for (int t = 0; t < individual.solution[j].task_index.size(); t++) {
                    two_cycle_task.push_back(individual.solution[j].task_index[t]);
                    temp_cycle2.push_back(individual.solution[j].task_index[t]);
                }
                split_result.push_back(temp_cycle2);

                for (int t = 0; t < individual.solution[k].task_index.size(); t++) {
                    two_cycle_task.push_back(individual.solution[k].task_index[t]);
                    temp_cycle3.push_back(individual.solution[k].task_index[t]);
                }
                split_result.push_back(temp_cycle3);

                best_split_cost = calc_split_result();
                old_cost = best_split_cost;
                split_result.clear();

                for (int m = 1; m <= 5; m++) {
                    merge(two_cycle_task, m, true);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }

                for (int m = 1; m <= 5; m++) {
                    merge(two_cycle_task, m, false);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }


                for (int m = 1; m <= 5; m++) {
                    merge_with_capacity(two_cycle_task, m, false);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }


                for (int m = 1; m <= 5; m++) {
                    merge_with_capacity(two_cycle_task, m, true);
                    int temp = calc_split_result();
                    if (temp < best_split_cost) {
                        best_split_cost = temp;
                        temp_split_rst.clear();
                        for (int c = 0; c < split_result.size(); c++) {
                            vector<int> temp_cycle;
                            for (int t = 0; t < split_result[c].size(); t++) {
                                temp_cycle.push_back(split_result[c][t]);
                            }
                            temp_split_rst.push_back(temp_cycle);
                        }
                    }
                }
//            cout << "--------------------1" << endl;
//            for (int c = 0; c < temp_split_rst.size(); c++) {
//                for (int t = 0; t < temp_split_rst[c].size(); t++) {
//                    cout << temp_split_rst[c][t] << ", ";
//                }
//                cout << endl;
//            }
//            cout << "--------------------2" << endl;
//            calc_cost(individual);
//            int before = individual.total_cost;
                if (best_split_cost < old_cost) {
                    if (temp_split_rst.size() == 3) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution[j].task_index.clear();
                        for (int t = 0; t < temp_split_rst[1].size(); t++) {
                            individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                        }
                        individual.solution[k].task_index.clear();
                        for (int t = 0; t < temp_split_rst[2].size(); t++) {
                            individual.solution[k].task_index.push_back(temp_split_rst[2][t]);
                        }
                    } else if (temp_split_rst.size() == 1) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution.erase(individual.solution.begin() + k);
                        individual.solution.erase(individual.solution.begin() + j);
                    } else if (temp_split_rst.size() == 2) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution[j].task_index.clear();
                        for (int t = 0; t < temp_split_rst[1].size(); t++) {
                            individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                        }
                        individual.solution.erase(individual.solution.begin() + k);
                    } else if (temp_split_rst.size() > 3) {
                        individual.solution[i].task_index.clear();
                        for (int t = 0; t < temp_split_rst[0].size(); t++) {
                            individual.solution[i].task_index.push_back(temp_split_rst[0][t]);
                        }
                        individual.solution[j].task_index.clear();
                        for (int t = 0; t < temp_split_rst[1].size(); t++) {
                            individual.solution[j].task_index.push_back(temp_split_rst[1][t]);
                        }
                        individual.solution[k].task_index.clear();
                        for (int t = 0; t < temp_split_rst[2].size(); t++) {
                            individual.solution[k].task_index.push_back(temp_split_rst[2][t]);
                        }
                        for (int t = 3; t < temp_split_rst.size(); t++) {
                            Cycle cycle;
                            for (int task_i = 0; task_i < temp_split_rst[t].size(); task_i++) {
                                cycle.task_index.push_back(temp_split_rst[t][task_i]);
                            }
                            individual.solution.push_back(cycle);
                        }
                    }
                }
            }
//            calc_cost(individual);
//            int after = individual.total_cost;
//            if (before > after) {
//                cout << "wefhjopasdijfhioasdjufljadfal;" << before << ", " << after << endl;
//            }
        }
    }

    return true;
}


void print_solution(Individual &individual) {
    for (const auto &c : individual.solution) {
        for (int t : c.task_index) {
            cout << task[t].head << " " << task[t].tail << " ";
        }
        cout << endl;
    }
}


bool sort_fun(const Individual &indiv1, const Individual &indiv2) {
    return indiv1.total_cost < indiv2.total_cost;
}


void init_population() {
    population.clear();

    for (int m = 1; m <= 10; m++) {
        Individual individual = greedy_init_individual(m);
        reverse_task(individual);
        reverse_head_tail(individual);
        swap_in_cycle(individual);
        swap_between_cycle(individual);
        insert_between_cycle(individual);
        merge_split(individual);
//        merge_split_three(individual);
        individual_ulusoy_split(individual);
        calc_cost(individual);
        population.push_back(individual);
    }

    for (int m = 1; m <= 10; m++) {
        Individual individual = greedy_init_individual_split(m);
        reverse_task(individual);
        reverse_head_tail(individual);
        swap_in_cycle(individual);
        swap_between_cycle(individual);
        insert_between_cycle(individual);
        merge_split(individual);
//        merge_split_three(individual);
        individual_ulusoy_split(individual);
        calc_cost(individual);
        population.push_back(individual);
    }
}

void local_search() {
    merge_split_three(population[0]);
    int luckey = random_num(population.size() - 1) + 1;
    merge_split_three(population[luckey]);
    for (int i = 0; i < population.size(); i++) {
        calc_cost(population[i]);

        reverse_task(population[i]);
        reverse_head_tail(population[i]);
        swap_in_cycle(population[i]);
        swap_between_cycle(population[i]);
        insert_between_cycle(population[i]);
        merge_split(population[i]);
        individual_ulusoy_split(population[i]);
        calc_cost(population[i]);
    }
}


void run() {
    for (int file_index = 0; file_index < file_num; file_index++) {
//        cout << input_files[file_index] << endl;
        read_data(file_index);
        floyd();

        int best = INF, best_m;
        init_population();
        sort(population.begin(), population.end(), sort_fun);

        for (int ite = 0; ite < 1000; ite++) {
//            cout << ite << endl;
            local_search();
            sort(population.begin(), population.end(), sort_fun);
            if (population[0].total_cost < best) {
                best = population[0].total_cost;
                best_m = ite;
//                cout << "best: " << best << endl;
            }
            population.erase(population.begin() + 10, population.end());
//            init_population();
//            population.erase(population.begin(), population.begin() + 50);
            for (int add = 0; add < 10; add++) {
                Individual indiv;
                if (random_num(2) < 1) {
                    indiv = greedy_init_individual(random_num(20) + 8);
                } else {
                    indiv = greedy_init_individual_split(random_num(20) + 9);
                }
                calc_cost(indiv);
                population.push_back(indiv);
            }
        }

        sort(population.begin(), population.end(), sort_fun);
//        cout << "best: " << best << ", " << best_m << endl;
        cout << best << endl;
//        cout << population[0].total_cost << ", " << population[0].solution.size() << ", " << vehicle_num << endl;
//        for (int m = 1; m <= 100; m++) {
////            Individual individual = greedy_init_individual(m);
//            Individual individual = greedy_init_individual_split(m);
//            calc_cost(individual);
//            int before = individual.total_cost;
//            bool improve = true;
//            int time = 0;
//            while (improve) {
//                time++;
//                if (time > 1000) break;
////                cout << time << endl;
//                improve = false;
//                improve = reverse_task(individual) ? true : improve;
////                improve = individual_ulusoy_split(individual) ? true : improve;
//
//                improve = reverse_head_tail(individual) ? true : improve;
////                improve = individual_ulusoy_split(individual) ? true : improve;
//
//                improve = swap_in_cycle(individual) ? true : improve;
////                improve = individual_ulusoy_split(individual) ? true : improve;
//
//                improve = swap_between_cycle(individual) ? true : improve;
////                improve = individual_ulusoy_split(individual) ? true : improve;
//
//                improve = insert_between_cycle(individual) ? true : improve;
//                merge_split(individual);
//                improve = individual_ulusoy_split(individual) ? true : improve;
//                if (individual.total_cost < best) {
//                    best = individual.total_cost;
//                    best_m = individual.solution.size();
//                    cout << best << endl;
//                }
//            }
//
//            calc_cost(individual);
//            if (individual.total_cost < best) {
//                best = individual.total_cost;
//                best_m = individual.solution.size();
//            }
//
//            if (true) {
//                calc_cost(individual);
//                if (before < individual.total_cost) {
////                    cout << "noooooooooooooooooooooooooooooooooooo improve" << endl;
//                }
////                cout << before << " " << individual.total_cost << endl;
//            } else {
//                if (before != individual.total_cost) {
//                    cout << "no improve, but no same !!!!!!!!!!!!!!! " << before << " " << individual.total_cost
//                         << endl;
//                }
//            }
//        }
//        cout << "best: " << best << ", " << best_m << ", " << vehicle_num << endl;
    }

}


int main() {
    run();
    return 0;
}


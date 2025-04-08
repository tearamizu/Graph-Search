#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX 100

typedef struct Node {
    int vertex;
    int cost;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} Graph[MAX];

typedef struct {
    int vertex;
    int cost;
} PQNode;

typedef struct {
    PQNode data[MAX];
    int size;
} PriorityQueue;

void firstGraph();
void secondGraph();
void initPQ(PriorityQueue* pq);
void pushPQ(PriorityQueue* pq, int vertex, int cost);
PQNode popPQ(PriorityQueue* pq);
bool isEmpty(PriorityQueue* pq);
void addEdge(Graph graph, int from, int to, int cost);
void breadthFirstSearch(Graph graph, int start, int goal, int numVertices, char* labels[]);
void depthFirstSearch(Graph graph, int start, int goal, int numVertices, char* labels[]);
bool depthFirstSearchHelper(Graph graph, int vertex, int goal, bool* visited, char* labels[]);
void bestFirstSearch(Graph graph, int start, int goal, char* labels[]);
void hillClimbing(Graph graph, int start, int goal, char* labels[]);
void branchAndBound(Graph graph, int start, int goal, char* labels[]);
void dynamicProgramming(Graph graph, int start, int goal, int numVertices, char* labels[]);

int main() {
    int choice;

    do {
        printf("1. Graph 1\n");
        printf("2. Graph 2\n");
        printf("3. Exit\n");
        printf("Masukkan pilihan: ");
        fflush(stdin);
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\n");
                printf("===================================\n");
                printf("             Graph 1\n");
                printf("===================================\n");
                firstGraph();
                break;
            case 2:
                printf("\n");
                printf("===================================\n");
                printf("             Graph 2\n");
                printf("===================================\n");
                secondGraph();
                break;
            case 3:
                printf("\n");
                printf("===================================\n");
                printf("         Exiting Aplication\n");
                printf("===================================\n");
                break;
            default:
                printf("\n");
                printf("===================================\n");
                printf("          Invalid Choice\n");
                printf("===================================\n");
                break;
        }
        printf("\n\n");
    } while (choice != 3);
    return 0;
}

void firstGraph() {
    Graph graph;
    for (int i = 0; i < MAX; i++) {
        graph[i].head = NULL;
    }

    char* labels[] = {"S", "A", "B", "C", "D", "Z"};

    addEdge(graph, 0, 1, 4);
    addEdge(graph, 0, 2, 2);
    addEdge(graph, 1, 2, 1);
    addEdge(graph, 1, 4, 3);
    addEdge(graph, 2, 3, 7);
    addEdge(graph, 3, 4, 2);
    addEdge(graph, 3, 5, 2);
    addEdge(graph, 4, 5, 5);

    int start = 0;
    int goal = 5;
    int numVertices = 6;

    breadthFirstSearch(graph, start, goal, numVertices, labels);
    depthFirstSearch(graph, start, goal, numVertices, labels);
    bestFirstSearch(graph, start, goal, labels);
    hillClimbing(graph, start, goal, labels);
    branchAndBound(graph, start, goal, labels);
    dynamicProgramming(graph, start, goal, numVertices, labels);
}

void secondGraph() {
    Graph graph;
    for (int i = 0; i < MAX; i++) {
        graph[i].head = NULL;
    }

    char* labels[] = {"A", "B", "C", "D", "E", "F", "G", "Z"};

    addEdge(graph, 0, 1, 3);
    addEdge(graph, 0, 2, 5);
    addEdge(graph, 1, 2, 6);
    addEdge(graph, 1, 3, 4);
    addEdge(graph, 2, 4, 3);
    addEdge(graph, 3, 4, 5);
    addEdge(graph, 3, 5, 3);
    addEdge(graph, 4, 6, 2);
    addEdge(graph, 5, 6, 6);
    addEdge(graph, 5, 7, 2);
    addEdge(graph, 6, 7, 3);

    int start = 0;
    int goal = 7;
    int numVertices = 8;

    breadthFirstSearch(graph, start, goal, numVertices, labels);
    depthFirstSearch(graph, start, goal, numVertices, labels);
    bestFirstSearch(graph, start, goal, labels);
    hillClimbing(graph, start, goal, labels);
    branchAndBound(graph, start, goal, labels);
    dynamicProgramming(graph, start, goal, numVertices, labels);
}

void initPQ(PriorityQueue* pq) {
    pq->size = 0;
}

void pushPQ(PriorityQueue* pq, int vertex, int cost) {
    int i = pq->size++;
    while (i > 0 && pq->data[(i - 1) / 2].cost > cost) {
        pq->data[i] = pq->data[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    pq->data[i].vertex = vertex;
    pq->data[i].cost = cost;
}

PQNode popPQ(PriorityQueue* pq) {
    PQNode min = pq->data[0];
    pq->size--;
    PQNode last = pq->data[pq->size];

    int i = 0, child;
    while (i * 2 + 1 < pq->size) {
        child = i * 2 + 1;
        if (child + 1 < pq->size && pq->data[child + 1].cost < pq->data[child].cost)
            child++;
        if (last.cost <= pq->data[child].cost)
            break;
        pq->data[i] = pq->data[child];
        i = child;
    }
    pq->data[i] = last;
    return min;
}

bool isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

void addEdge(Graph graph, int from, int to, int cost) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = to;
    newNode->cost = cost;
    newNode->next = graph[from].head;
    graph[from].head = newNode;

    newNode = (Node*) malloc(sizeof(Node));
    newNode->vertex = from;
    newNode->cost = cost;
    newNode->next = graph[to].head;
    graph[to].head = newNode;
}

void breadthFirstSearch(Graph graph, int start, int goal, int numVertices, char* labels[]) {
    bool visited[MAX] = {false};
    int queue[MAX];
    int front = 0, rear = 0;

    visited[start] = true;
    queue[rear++] = start;

    printf("Breadth First Search: ");
    while (front < rear) {
        int current = queue[front++];
        printf("%s ", labels[current]);

        if (current == goal) {
            printf("\n");
            return;
        }

        Node* temp = graph[current].head;
        while (temp) {
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = true;
                queue[rear++] = temp->vertex;
            }
            temp = temp->next;
        }
    }
    printf("\nGoal tidak ditemukan.\n");
}

void depthFirstSearch(Graph graph, int start, int goal, int numVertices, char* labels[]) {
    bool visited[MAX] = {false};
    printf("Depth First Search: ");
    if (!depthFirstSearchHelper(graph, start, goal, visited, labels)) {
        printf("\nGoal tidak ditemukan.\n");
    }
}

bool depthFirstSearchHelper(Graph graph, int vertex, int goal, bool* visited, char* labels[]) {
    visited[vertex] = true;
    printf("%s ", labels[vertex]);

    if (vertex == goal) {
        printf("\n");
        return true;
    }

    Node* temp = graph[vertex].head;
    while (temp) {
        if (!visited[temp->vertex]) {
            if (depthFirstSearchHelper(graph, temp->vertex, goal, visited, labels)) {
                return true;
            }
        }
        temp = temp->next;
    }
    return false;
}

void bestFirstSearch(Graph graph, int start, int goal, char* labels[]) {
    bool visited[MAX] = {false};
    PriorityQueue pq;
    initPQ(&pq);

    pushPQ(&pq, start, 0);
    visited[start] = true;

    printf("Best First Search: ");
    while (!isEmpty(&pq)) {
        PQNode current = popPQ(&pq);
        printf("%s ", labels[current.vertex]);

        if (current.vertex == goal) break;

        Node* temp = graph[current.vertex].head;
        while (temp) {
            if (!visited[temp->vertex]) {
                visited[temp->vertex] = true;
                pushPQ(&pq, temp->vertex, temp->cost);
            }
            temp = temp->next;
        }
    }
    printf("\n");
}

void hillClimbing(Graph graph, int start, int goal, char* labels[]) {
    bool visited[MAX] = {false};
    int current = start;
    visited[current] = true;

    printf("Hill Climbing: ");
    printf("%s ", labels[current]);

    while (current != goal) {
        Node* temp = graph[current].head;
        int next = -1;
        int minCost = INT_MAX;

        while (temp) {
            if (!visited[temp->vertex] && temp->cost < minCost) {
                minCost = temp->cost;
                next = temp->vertex;
            }
            temp = temp->next;
        }

        if (next == -1) {
            printf("\nLocal maxima tercapai.\n");
            return;
        }

        current = next;
        visited[current] = true;
        printf("%s ", labels[current]);
    }
    printf("\n");
}

void branchAndBound(Graph graph, int start, int goal, char* labels[]) {
    bool visited[MAX] = {false};
    PriorityQueue pq;
    initPQ(&pq);

    pushPQ(&pq, start, 0);

    printf("Branch and Bound: ");
    while (!isEmpty(&pq)) {
        PQNode current = popPQ(&pq);

        if (visited[current.vertex]) continue;
        visited[current.vertex] = true;

        printf("%s ", labels[current.vertex]);

        if (current.vertex == goal) {
            printf("\nGoal tercapai dengan cost: %d\n", current.cost);
            return;
        }

        Node* temp = graph[current.vertex].head;
        while (temp) {
            if (!visited[temp->vertex]) {
                pushPQ(&pq, temp->vertex, current.cost + temp->cost);
            }
            temp = temp->next;
        }
    }
    printf("\nGoal tidak ditemukan.\n");
}

void dynamicProgramming(Graph graph, int start, int goal, int numVertices, char* labels[]) {
    int dist[MAX];
    bool visited[MAX] = {false};
    int prev[MAX];
    for (int i = 0; i < numVertices; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;

    PriorityQueue pq;
    initPQ(&pq);
    pushPQ(&pq, start, 0);

    while (!isEmpty(&pq)) {
        PQNode current = popPQ(&pq);
        int u = current.vertex;

        if (visited[u]) continue;
        visited[u] = true;

        Node* temp = graph[u].head;
        while (temp) {
            int v = temp->vertex;
            int weight = temp->cost;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pushPQ(&pq, v, dist[v]);
            }
            temp = temp->next;
        }
    }

    printf("Shortest Path (Dijkstra): ");
    int path[MAX], pathSize = 0;
    for (int at = goal; at != -1; at = prev[at]) {
        path[pathSize++] = at;
    }
    for (int i = pathSize - 1; i >= 0; i--) {
        printf("%s ", labels[path[i]]);
    }
    printf("\nShortest Path Cost (Dijkstra): %d\n", dist[goal]);
}
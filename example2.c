#include <stdio.h>
#include <stdlib.h>

// ======================== Structuri ========================
typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Graph {
    int vertices;
    int *visited;
    Node **adjacency_lists;
} Graph;

// ======================== Funcții utilitare ========================
Node *create_node(int value) {
    Node *node = malloc(sizeof(Node));
    if (!node) {
        perror("Eroare la alocarea nodului");
        exit(EXIT_FAILURE);
    }
    node->data = value;
    node->next = NULL;
    return node;
}

Graph *create_graph(int vertices) {
    Graph *graph = malloc(sizeof(Graph));
    if (!graph) {
        perror("Eroare la alocarea grafului");
        exit(EXIT_FAILURE);
    }

    graph->vertices = vertices;
    graph->adjacency_lists = calloc(vertices, sizeof(Node *));
    graph->visited = calloc(vertices, sizeof(int));

    if (!graph->adjacency_lists || !graph->visited) {
        perror("Eroare la alocarea listelor sau vectorului de vizitare");
        exit(EXIT_FAILURE);
    }

    return graph;
}

void add_edge(Graph *graph, int src, int dest) {
    Node *node = create_node(dest);
    node->next = graph->adjacency_lists[src];
    graph->adjacency_lists[src] = node;

    // Neorientat: adăugăm și invers
    node = create_node(src);
    node->next = graph->adjacency_lists[dest];
    graph->adjacency_lists[dest] = node;
}

void insert_edges(Graph *graph, int num_edges) {
    int src, dest;
    printf("Adauga %d muchii (format: src dest):\n", num_edges);
    for (int i = 0; i < num_edges; i++) {
        printf("Muchia %d: ", i + 1);
        scanf("%d %d", &src, &dest);
        add_edge(graph, src, dest);
    }
}

void reset_visited(Graph *graph) {
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
}

// ======================== DFS ========================
void DFS(Graph *graph, int vertex) {
    graph->visited[vertex] = 1;
    printf("%d ", vertex);

    Node *temp = graph->adjacency_lists[vertex];
    while (temp) {
        int neighbor = temp->data;
        if (!graph->visited[neighbor]) {
            DFS(graph, neighbor);
        }
        temp = temp->next;
    }
}

// ======================== BFS ========================
int is_empty(Node *queue) {
    return queue == NULL;
}

void enqueue(Node **queue, int value) {
    Node *node = create_node(value);
    if (is_empty(*queue)) {
        *queue = node;
    } else {
        Node *temp = *queue;
        while (temp->next) temp = temp->next;
        temp->next = node;
    }
}

int dequeue(Node **queue) {
    if (is_empty(*queue)) return -1;
    Node *temp = *queue;
    int value = temp->data;
    *queue = temp->next;
    free(temp);
    return value;
}

void BFS(Graph *graph, int start_vertex) {
    Node *queue = NULL;
    graph->visited[start_vertex] = 1;
    enqueue(&queue, start_vertex);

    while (!is_empty(queue)) {
        int current = dequeue(&queue);
        printf("%d ", current);

        Node *temp = graph->adjacency_lists[current];
        while (temp) {
            int neighbor = temp->data;
            if (!graph->visited[neighbor]) {
                graph->visited[neighbor] = 1;
                enqueue(&queue, neighbor);
            }
            temp = temp->next;
        }
    }
}

// ======================== Main ========================
int main() {
    int num_vertices, num_edges, start;

    printf("Numar noduri: ");
    scanf("%d", &num_vertices);

    printf("Numar muchii: ");
    scanf("%d", &num_edges);

    Graph *graph = create_graph(num_vertices);
    insert_edges(graph, num_edges);

    printf("Nod de start pentru DFS: ");
    scanf("%d", &start);
    printf("Parcurgere DFS: ");
    DFS(graph, start);

    reset_visited(graph);

    printf("\nNod de start pentru BFS: ");
    scanf("%d", &start);
    printf("Parcurgere BFS: ");
    BFS(graph, start);

   return 0;
}

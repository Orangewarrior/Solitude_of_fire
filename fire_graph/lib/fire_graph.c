#include "fire_graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file fire_graph.c
 * @brief Internal implementation of the weighted graph, Bellman-Ford, and Floyd-Warshall.
 *
 * Design notes:
 * - adjacency lists are used for graph ownership and simple debug printing
 * - a separate compact edge table is kept for Bellman-Ford relaxations
 * - Floyd-Warshall uses dynamically allocated flattened matrices
 * - limits are enforced at graph creation time to avoid silent unbounded growth
 */

typedef struct fire_graph_edge_node {
    size_t to;
    int weight;
    struct fire_graph_edge_node *next;
} fire_graph_edge_node;

typedef struct fire_graph_edge_ref {
    size_t from;
    size_t to;
    int weight;
} fire_graph_edge_ref;

struct fire_graph {
    size_t vertex_count;
    size_t edge_count;
    size_t max_vertices;
    size_t max_edges;
    char **labels;
    fire_graph_edge_node **adj;
    fire_graph_edge_ref *edges;
};

static char *fire_graph_strdup(const char *s)
{
    size_t len;
    char *copy;
    if (s == NULL) return NULL;
    len = strlen(s) + 1U;
    copy = (char *)malloc(len);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len);
    return copy;
}

static int checked_add_int(int a, int b, int *out)
{
    long long tmp;
    if (out == NULL) return FIRE_GRAPH_ERR_INVAL;
    tmp = (long long)a + (long long)b;
    if (tmp > INT_MAX || tmp < INT_MIN) return FIRE_GRAPH_ERR_OVERFLOW;
    *out = (int)tmp;
    return FIRE_GRAPH_OK;
}

static size_t fw_index(size_t n, size_t i, size_t j)
{
    return i * n + j;
}

static void bf_result_zero(fire_bellmanford_result *out)
{
    if (out == NULL) return;
    out->vertex_count = 0U;
    out->distance = NULL;
    out->predecessor = NULL;
    out->source_index = 0U;
    out->has_negative_cycle = 0;
}

static void fw_result_zero(fire_floydwarshall_result *out)
{
    if (out == NULL) return;
    out->vertex_count = 0U;
    out->distance = NULL;
    out->next = NULL;
    out->has_negative_cycle = 0;
}

int fire_graph_create(fire_graph **out_graph, size_t max_vertices, size_t max_edges)
{
    fire_graph *g;
    if (out_graph == NULL || max_vertices == 0U || max_edges == 0U) return FIRE_GRAPH_ERR_INVAL;
    *out_graph = NULL;
    g = (fire_graph *)calloc(1U, sizeof(*g));
    if (g == NULL) return FIRE_GRAPH_ERR_NOMEM;
    g->labels = (char **)calloc(max_vertices, sizeof(*g->labels));
    g->adj = (fire_graph_edge_node **)calloc(max_vertices, sizeof(*g->adj));
    g->edges = (fire_graph_edge_ref *)calloc(max_edges, sizeof(*g->edges));
    if (g->labels == NULL || g->adj == NULL || g->edges == NULL) {
        free(g->labels); free(g->adj); free(g->edges); free(g);
        return FIRE_GRAPH_ERR_NOMEM;
    }
    g->max_vertices = max_vertices;
    g->max_edges = max_edges;
    *out_graph = g;
    return FIRE_GRAPH_OK;
}

void fire_graph_destroy(fire_graph **graph)
{
    size_t i;
    fire_graph *g;
    if (graph == NULL || *graph == NULL) return;
    g = *graph;
    for (i = 0U; i < g->vertex_count; i++) {
        fire_graph_edge_node *cur = g->adj[i];
        while (cur != NULL) {
            fire_graph_edge_node *next = cur->next;
            free(cur);
            cur = next;
        }
        free(g->labels[i]);
    }
    free(g->labels);
    free(g->adj);
    free(g->edges);
    free(g);
    *graph = NULL;
}

int fire_graph_add_vertex(fire_graph *graph, const char *label, size_t *out_index)
{
    char *copy;
    size_t i;
    if (graph == NULL || label == NULL || label[0] == '\0') return FIRE_GRAPH_ERR_INVAL;
    if (graph->vertex_count >= graph->max_vertices) return FIRE_GRAPH_ERR_LIMIT;
    for (i = 0U; i < graph->vertex_count; i++) {
        if (strcmp(graph->labels[i], label) == 0) {
            if (out_index != NULL) *out_index = i;
            return FIRE_GRAPH_OK;
        }
    }
    copy = fire_graph_strdup(label);
    if (copy == NULL) return FIRE_GRAPH_ERR_NOMEM;
    graph->labels[graph->vertex_count] = copy;
    if (out_index != NULL) *out_index = graph->vertex_count;
    graph->vertex_count++;
    return FIRE_GRAPH_OK;
}

int fire_graph_find_vertex(const fire_graph *graph, const char *label, size_t *out_index)
{
    size_t i;
    if (graph == NULL || label == NULL || out_index == NULL) return FIRE_GRAPH_ERR_INVAL;
    for (i = 0U; i < graph->vertex_count; i++) {
        if (strcmp(graph->labels[i], label) == 0) {
            *out_index = i;
            return FIRE_GRAPH_OK;
        }
    }
    return FIRE_GRAPH_ERR_NOTFOUND;
}

int fire_graph_add_edge(fire_graph *graph, size_t from, size_t to, int weight)
{
    fire_graph_edge_node *node;
    if (graph == NULL) return FIRE_GRAPH_ERR_INVAL;
    if (from >= graph->vertex_count || to >= graph->vertex_count) return FIRE_GRAPH_ERR_INVAL;
    if (graph->edge_count >= graph->max_edges) return FIRE_GRAPH_ERR_LIMIT;
    node = (fire_graph_edge_node *)calloc(1U, sizeof(*node));
    if (node == NULL) return FIRE_GRAPH_ERR_NOMEM;
    node->to = to;
    node->weight = weight;
    node->next = graph->adj[from];
    graph->adj[from] = node;
    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;
    return FIRE_GRAPH_OK;
}

void fire_graph_print(const fire_graph *graph)
{
    size_t i;
    if (graph == NULL) return;
    for (i = 0U; i < graph->vertex_count; i++) {
        fire_graph_edge_node *cur = graph->adj[i];
        printf("%s:", graph->labels[i]);
        while (cur != NULL) {
            printf(" -> %s(%d)", graph->labels[cur->to], cur->weight);
            cur = cur->next;
        }
        printf("\n");
    }
}

void fire_graph_print_edges(const fire_graph *graph)
{
    size_t i;
    if (graph == NULL) return;
    puts("Edges (from -> to : weight)");
    for (i = 0U; i < graph->edge_count; i++) {
        printf("  %s -> %s : %d\n",
               graph->labels[graph->edges[i].from],
               graph->labels[graph->edges[i].to],
               graph->edges[i].weight);
    }
}

int fire_graph_get_edge_weight(const fire_graph *graph, size_t from, size_t to, int *out_weight)
{
    size_t i;
    if (graph == NULL || out_weight == NULL) return FIRE_GRAPH_ERR_INVAL;
    for (i = 0U; i < graph->edge_count; i++) {
        if (graph->edges[i].from == from && graph->edges[i].to == to) {
            *out_weight = graph->edges[i].weight;
            return FIRE_GRAPH_OK;
        }
    }
    return FIRE_GRAPH_ERR_NOTFOUND;
}

int fire_bellmanford_run(const fire_graph *graph, size_t source_index, fire_bellmanford_result *out)
{
    const int INF = INT_MAX / 4;
    size_t i, pass;
    int changed;
    if (graph == NULL || out == NULL || source_index >= graph->vertex_count) return FIRE_GRAPH_ERR_INVAL;
    bf_result_zero(out);
    out->distance = (int *)malloc(graph->vertex_count * sizeof(*out->distance));
    out->predecessor = (int *)malloc(graph->vertex_count * sizeof(*out->predecessor));
    if (out->distance == NULL || out->predecessor == NULL) {
        fire_bellmanford_result_free(out);
        return FIRE_GRAPH_ERR_NOMEM;
    }
    out->vertex_count = graph->vertex_count;
    out->source_index = source_index;
    for (i = 0U; i < graph->vertex_count; i++) {
        out->distance[i] = INF;
        out->predecessor[i] = -1;
    }
    out->distance[source_index] = 0;

    for (pass = 0U; pass + 1U < graph->vertex_count; pass++) {
        changed = 0;
        for (i = 0U; i < graph->edge_count; i++) {
            size_t from = graph->edges[i].from;
            size_t to = graph->edges[i].to;
            int candidate;
            if (out->distance[from] >= INF) continue;
            if (checked_add_int(out->distance[from], graph->edges[i].weight, &candidate) != FIRE_GRAPH_OK) continue;
            if (candidate < out->distance[to]) {
                out->distance[to] = candidate;
                out->predecessor[to] = (int)from;
                changed = 1;
            }
        }
        if (!changed) break;
    }

    for (i = 0U; i < graph->edge_count; i++) {
        size_t from = graph->edges[i].from;
        size_t to = graph->edges[i].to;
        int candidate;
        if (out->distance[from] >= INF) continue;
        if (checked_add_int(out->distance[from], graph->edges[i].weight, &candidate) != FIRE_GRAPH_OK) continue;
        if (candidate < out->distance[to]) {
            out->has_negative_cycle = 1;
            return FIRE_GRAPH_ERR_NEGATIVE_CYCLE;
        }
    }
    return FIRE_GRAPH_OK;
}

void fire_bellmanford_result_free(fire_bellmanford_result *result)
{
    if (result == NULL) return;
    free(result->distance);
    free(result->predecessor);
    bf_result_zero(result);
}

int fire_bellmanford_build_path(const fire_bellmanford_result *result, size_t target_index, size_t **out_path, size_t *out_length)
{
    size_t len = 0U, i;
    int cur;
    size_t *path;
    const int INF = INT_MAX / 4;
    if (result == NULL || out_path == NULL || out_length == NULL) return FIRE_GRAPH_ERR_INVAL;
    if (target_index >= result->vertex_count) return FIRE_GRAPH_ERR_INVAL;
    if (result->distance == NULL || result->predecessor == NULL) return FIRE_GRAPH_ERR_INVAL;
    if (result->distance[target_index] >= INF) return FIRE_GRAPH_ERR_UNREACHABLE;
    cur = (int)target_index;
    while (cur >= 0) { len++; cur = result->predecessor[cur]; }
    path = (size_t *)malloc(len * sizeof(*path));
    if (path == NULL) return FIRE_GRAPH_ERR_NOMEM;
    cur = (int)target_index;
    for (i = len; i > 0U; i--) {
        path[i - 1U] = (size_t)cur;
        cur = result->predecessor[cur];
    }
    *out_path = path;
    *out_length = len;
    return FIRE_GRAPH_OK;
}

int fire_floydwarshall_run(const fire_graph *graph, fire_floydwarshall_result *out)
{
    const int INF = INT_MAX / 4;
    size_t i, j, k, n;
    if (graph == NULL || out == NULL) return FIRE_GRAPH_ERR_INVAL;
    fw_result_zero(out);
    n = graph->vertex_count;
    out->distance = (int *)malloc(n * n * sizeof(*out->distance));
    out->next = (int *)malloc(n * n * sizeof(*out->next));
    if (out->distance == NULL || out->next == NULL) {
        fire_floydwarshall_result_free(out);
        return FIRE_GRAPH_ERR_NOMEM;
    }
    out->vertex_count = n;

    for (i = 0U; i < n; i++) {
        for (j = 0U; j < n; j++) {
            size_t idx = fw_index(n, i, j);
            out->distance[idx] = (i == j) ? 0 : INF;
            out->next[idx] = (i == j) ? (int)i : -1;
        }
    }

    for (i = 0U; i < graph->edge_count; i++) {
        size_t from = graph->edges[i].from;
        size_t to = graph->edges[i].to;
        size_t idx = fw_index(n, from, to);
        if (graph->edges[i].weight < out->distance[idx]) {
            out->distance[idx] = graph->edges[i].weight;
            out->next[idx] = (int)to;
        }
    }

    for (k = 0U; k < n; k++) {
        for (i = 0U; i < n; i++) {
            for (j = 0U; j < n; j++) {
                int dik = out->distance[fw_index(n, i, k)];
                int dkj = out->distance[fw_index(n, k, j)];
                int candidate;
                size_t idx_ij = fw_index(n, i, j);
                if (dik >= INF || dkj >= INF) continue;
                if (checked_add_int(dik, dkj, &candidate) != FIRE_GRAPH_OK) continue;
                if (candidate < out->distance[idx_ij]) {
                    out->distance[idx_ij] = candidate;
                    out->next[idx_ij] = out->next[fw_index(n, i, k)];
                }
            }
        }
    }

    for (i = 0U; i < n; i++) {
        if (out->distance[fw_index(n, i, i)] < 0) {
            out->has_negative_cycle = 1;
            return FIRE_GRAPH_ERR_NEGATIVE_CYCLE;
        }
    }
    return FIRE_GRAPH_OK;
}

void fire_floydwarshall_result_free(fire_floydwarshall_result *result)
{
    if (result == NULL) return;
    free(result->distance);
    free(result->next);
    fw_result_zero(result);
}

int fire_floydwarshall_build_path(const fire_floydwarshall_result *result, size_t source_index, size_t target_index, size_t **out_path, size_t *out_length)
{
    const int INF = INT_MAX / 4;
    size_t n, cap, len;
    size_t *path;
    int cur;
    if (result == NULL || out_path == NULL || out_length == NULL) return FIRE_GRAPH_ERR_INVAL;
    n = result->vertex_count;
    if (source_index >= n || target_index >= n) return FIRE_GRAPH_ERR_INVAL;
    if (result->distance == NULL || result->next == NULL) return FIRE_GRAPH_ERR_INVAL;
    if (result->distance[fw_index(n, source_index, target_index)] >= INF) return FIRE_GRAPH_ERR_UNREACHABLE;
    if (result->next[fw_index(n, source_index, target_index)] < 0) return FIRE_GRAPH_ERR_UNREACHABLE;

    cap = n + 1U;
    path = (size_t *)malloc(cap * sizeof(*path));
    if (path == NULL) return FIRE_GRAPH_ERR_NOMEM;
    len = 0U;
    cur = (int)source_index;
    path[len++] = source_index;
    while ((size_t)cur != target_index) {
        cur = result->next[fw_index(n, (size_t)cur, target_index)];
        if (cur < 0) {
            free(path);
            return FIRE_GRAPH_ERR_UNREACHABLE;
        }
        if (len >= cap) {
            free(path);
            return FIRE_GRAPH_ERR_LIMIT;
        }
        path[len++] = (size_t)cur;
    }
    *out_path = path;
    *out_length = len;
    return FIRE_GRAPH_OK;
}

void fire_graph_print_path_labels(const fire_graph *graph, const size_t *path, size_t length)
{
    size_t i;
    if (graph == NULL || path == NULL || length == 0U) return;
    for (i = 0U; i < length; i++) {
        printf("%s", graph->labels[path[i]]);
        if (i + 1U < length) printf(" -> ");
    }
    printf("\n");
}

const char *fire_graph_vertex_label(const fire_graph *graph, size_t index)
{
    if (graph == NULL || index >= graph->vertex_count) return NULL;
    return graph->labels[index];
}

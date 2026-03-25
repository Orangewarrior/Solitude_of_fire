#define _POSIX_C_SOURCE 200809L
/**
 * @file main.c
 * @brief Full example and benchmark for the fire_graph module.
 *
 * This example builds a weighted route graph, prints every edge and its cost,
 * runs Bellman-Ford and Floyd-Warshall, reconstructs the shortest route from
 * A to J, and benchmarks both algorithms on the same graph.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/fire_graph.h"

static int add_vertex_named(fire_graph *g, const char *label)
{
    return fire_graph_add_vertex(g, label, NULL);
}

static int add_edge_named(fire_graph *g, const char *from, const char *to, int weight)
{
    size_t u, v;
    if (fire_graph_find_vertex(g, from, &u) != FIRE_GRAPH_OK) return FIRE_GRAPH_ERR_NOTFOUND;
    if (fire_graph_find_vertex(g, to, &v) != FIRE_GRAPH_OK) return FIRE_GRAPH_ERR_NOTFOUND;
    return fire_graph_add_edge(g, u, v, weight);
}

static long long elapsed_ns(struct timespec start, struct timespec end)
{
    long long s = (long long)(end.tv_sec - start.tv_sec) * 1000000000LL;
    long long n = (long long)(end.tv_nsec - start.tv_nsec);
    return s + n;
}

static void print_path_with_weights(const fire_graph *g, const size_t *path, size_t path_len)
{
    size_t i;
    int total = 0;
    if (g == NULL || path == NULL || path_len == 0U) return;
    for (i = 0U; i + 1U < path_len; i++) {
        int weight = 0;
        const char *a = fire_graph_vertex_label(g, path[i]);
        const char *b = fire_graph_vertex_label(g, path[i + 1U]);
        if (fire_graph_get_edge_weight(g, path[i], path[i + 1U], &weight) == FIRE_GRAPH_OK) {
            printf("  %s -> %s : %d\n", a, b, weight);
            total += weight;
        } else {
            printf("  %s -> %s : <missing edge>\n", a, b);
        }
    }
    printf("  total cost = %d\n", total);
}

int main(void)
{
    fire_graph *g = NULL;
    fire_bellmanford_result bf;
    fire_floydwarshall_result fw;
    size_t *path = NULL;
    size_t path_len = 0U;
    size_t source, target;
    int rc;
    const char *labels[] = {"A","B","C","D","E","F","G","H","I","J"};
    size_t i;
    struct timespec t1, t2;
    long long bf_ns = 0, fw_ns = 0;
    const int rounds = 5000;

    rc = fire_graph_create(&g, 16U, 64U);
    if (rc != FIRE_GRAPH_OK) {
        fprintf(stderr, "graph creation failed: %d\n", rc);
        return 1;
    }

    for (i = 0U; i < sizeof(labels) / sizeof(labels[0]); i++) {
        rc = add_vertex_named(g, labels[i]);
        if (rc != FIRE_GRAPH_OK) {
            fprintf(stderr, "vertex creation failed: %d\n", rc);
            fire_graph_destroy(&g);
            return 1;
        }
    }

    add_edge_named(g, "A", "B", 6);
    add_edge_named(g, "A", "C", 7);
    add_edge_named(g, "A", "J", 50);
    add_edge_named(g, "B", "C", 8);
    add_edge_named(g, "B", "D", 5);
    add_edge_named(g, "B", "E", -4);
    add_edge_named(g, "C", "D", -3);
    add_edge_named(g, "C", "F", 9);
    add_edge_named(g, "C", "I", 20);
    add_edge_named(g, "D", "B", -2);
    add_edge_named(g, "D", "E", 7);
    add_edge_named(g, "D", "G", 4);
    add_edge_named(g, "E", "H", 3);
    add_edge_named(g, "E", "J", 15);
    add_edge_named(g, "F", "G", 2);
    add_edge_named(g, "G", "H", 2);
    add_edge_named(g, "H", "I", 2);
    add_edge_named(g, "I", "J", 2);

    puts("Adjacency representation:");
    fire_graph_print(g);
    puts("\nExact route table for reproduction/debug:");
    fire_graph_print_edges(g);

    fire_graph_find_vertex(g, "A", &source);
    fire_graph_find_vertex(g, "J", &target);

    rc = fire_bellmanford_run(g, source, &bf);
    if (rc == FIRE_GRAPH_ERR_NEGATIVE_CYCLE) {
        fprintf(stderr, "negative cycle detected from source %s\n", fire_graph_vertex_label(g, source));
        fire_graph_destroy(&g);
        return 1;
    }
    if (rc != FIRE_GRAPH_OK) {
        fprintf(stderr, "bellman-ford failed: %d\n", rc);
        fire_graph_destroy(&g);
        return 1;
    }

    rc = fire_bellmanford_build_path(&bf, target, &path, &path_len);
    if (rc != FIRE_GRAPH_OK) {
        fprintf(stderr, "Bellman-Ford path reconstruction failed: %d\n", rc);
        fire_bellmanford_result_free(&bf);
        fire_graph_destroy(&g);
        return 1;
    }

    printf("\nBellman-Ford shortest route from %s to %s\n", fire_graph_vertex_label(g, source), fire_graph_vertex_label(g, target));
    printf("Cost: %d\n", bf.distance[target]);
    printf("Path: ");
    fire_graph_print_path_labels(g, path, path_len);
    puts("Edges used by Bellman-Ford:");
    print_path_with_weights(g, path, path_len);
    free(path);
    path = NULL;

    rc = fire_floydwarshall_run(g, &fw);
    if (rc == FIRE_GRAPH_ERR_NEGATIVE_CYCLE) {
        fprintf(stderr, "Floyd-Warshall detected a negative cycle\n");
        fire_bellmanford_result_free(&bf);
        fire_graph_destroy(&g);
        return 1;
    }
    if (rc != FIRE_GRAPH_OK) {
        fprintf(stderr, "floyd-warshall failed: %d\n", rc);
        fire_bellmanford_result_free(&bf);
        fire_graph_destroy(&g);
        return 1;
    }

    rc = fire_floydwarshall_build_path(&fw, source, target, &path, &path_len);
    if (rc != FIRE_GRAPH_OK) {
        fprintf(stderr, "Floyd-Warshall path reconstruction failed: %d\n", rc);
        fire_floydwarshall_result_free(&fw);
        fire_bellmanford_result_free(&bf);
        fire_graph_destroy(&g);
        return 1;
    }

    printf("\nFloyd-Warshall shortest route from %s to %s\n", fire_graph_vertex_label(g, source), fire_graph_vertex_label(g, target));
    printf("Cost: %d\n", fw.distance[source * fw.vertex_count + target]);
    printf("Path: ");
    fire_graph_print_path_labels(g, path, path_len);
    puts("Edges used by Floyd-Warshall:");
    print_path_with_weights(g, path, path_len);
    free(path);
    path = NULL;

    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (i = 0U; i < (size_t)rounds; i++) {
        fire_bellmanford_result tmp;
        if (fire_bellmanford_run(g, source, &tmp) != FIRE_GRAPH_OK) break;
        fire_bellmanford_result_free(&tmp);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);
    bf_ns = elapsed_ns(t1, t2);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    for (i = 0U; i < (size_t)rounds; i++) {
        fire_floydwarshall_result tmp;
        if (fire_floydwarshall_run(g, &tmp) != FIRE_GRAPH_OK) break;
        fire_floydwarshall_result_free(&tmp);
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);
    fw_ns = elapsed_ns(t1, t2);

    printf("\nBenchmark (%d rounds)\n", rounds);
    printf("Bellman-Ford total:   %.3f ms\n", (double)bf_ns / 1000000.0);
    printf("Bellman-Ford average: %.3f us\n", (double)bf_ns / (double)rounds / 1000.0);
    printf("Floyd-Warshall total: %.3f ms\n", (double)fw_ns / 1000000.0);
    printf("Floyd-Warshall avg:   %.3f us\n", (double)fw_ns / (double)rounds / 1000.0);

    fire_floydwarshall_result_free(&fw);
    fire_bellmanford_result_free(&bf);
    fire_graph_destroy(&g);
    return 0;
}

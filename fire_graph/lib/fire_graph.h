#ifndef FIRE_GRAPH_H
#define FIRE_GRAPH_H

#include <stddef.h>

/**
 * @file fire_graph.h
 * @brief Weighted directed graph API with Bellman-Ford and Floyd-Warshall.
 *
 * This module is intentionally didactic: it exposes a compact graph structure,
 * explicit limits for vertices and edges, and two classic shortest-path
 * algorithms. Memory is allocated dynamically to avoid fixed-size static
 * matrices while still keeping hard limits that prevent accidental unbounded
 * growth.
 */

typedef enum fire_graph_status {
    FIRE_GRAPH_OK = 0,
    FIRE_GRAPH_ERR_INVAL = -1,
    FIRE_GRAPH_ERR_NOMEM = -2,
    FIRE_GRAPH_ERR_LIMIT = -3,
    FIRE_GRAPH_ERR_NOTFOUND = -4,
    FIRE_GRAPH_ERR_NEGATIVE_CYCLE = -5,
    FIRE_GRAPH_ERR_UNREACHABLE = -6,
    FIRE_GRAPH_ERR_OVERFLOW = -7
} fire_graph_status;

typedef struct fire_graph fire_graph;

/**
 * @brief Result object for a single-source Bellman-Ford run.
 */
typedef struct fire_bellmanford_result {
    size_t vertex_count;      /**< Number of vertices captured in the result. */
    int *distance;            /**< Distance array indexed by vertex id. */
    int *predecessor;         /**< Predecessor array used for path reconstruction. */
    size_t source_index;      /**< Source vertex index used for the run. */
    int has_negative_cycle;   /**< Non-zero if a reachable negative cycle was detected. */
} fire_bellmanford_result;

/**
 * @brief Result object for an all-pairs Floyd-Warshall run.
 */
typedef struct fire_floydwarshall_result {
    size_t vertex_count;      /**< Number of vertices captured in the result. */
    int *distance;            /**< Flattened VxV distance matrix. */
    int *next;                /**< Flattened VxV next-hop matrix for path rebuild. */
    int has_negative_cycle;   /**< Non-zero if any negative cycle was detected. */
} fire_floydwarshall_result;

int fire_graph_create(fire_graph **out_graph, size_t max_vertices, size_t max_edges);
void fire_graph_destroy(fire_graph **graph);
int fire_graph_add_vertex(fire_graph *graph, const char *label, size_t *out_index);
int fire_graph_find_vertex(const fire_graph *graph, const char *label, size_t *out_index);
int fire_graph_add_edge(fire_graph *graph, size_t from, size_t to, int weight);
void fire_graph_print(const fire_graph *graph);

/**
 * @brief Print the edge table in insertion order.
 *
 * This is useful in examples and debugging because it makes it easy to copy the
 * exact weighted route layout into another tool or implementation.
 */
void fire_graph_print_edges(const fire_graph *graph);

/**
 * @brief Query a directed edge weight.
 * @param graph Graph handle.
 * @param from Source vertex index.
 * @param to Destination vertex index.
 * @param out_weight Receives edge weight.
 * @return FIRE_GRAPH_OK if the edge exists, FIRE_GRAPH_ERR_NOTFOUND otherwise.
 */
int fire_graph_get_edge_weight(const fire_graph *graph, size_t from, size_t to, int *out_weight);

int fire_bellmanford_run(const fire_graph *graph, size_t source_index, fire_bellmanford_result *out);
void fire_bellmanford_result_free(fire_bellmanford_result *result);
int fire_bellmanford_build_path(const fire_bellmanford_result *result, size_t target_index, size_t **out_path, size_t *out_length);

/**
 * @brief Run Floyd-Warshall for all-pairs shortest paths.
 * @param graph Graph handle.
 * @param out Result object to initialize.
 * @return FIRE_GRAPH_OK on success.
 */
int fire_floydwarshall_run(const fire_graph *graph, fire_floydwarshall_result *out);

/**
 * @brief Release matrices stored in a Floyd-Warshall result.
 */
void fire_floydwarshall_result_free(fire_floydwarshall_result *result);

/**
 * @brief Rebuild one shortest path from @p source_index to @p target_index.
 */
int fire_floydwarshall_build_path(const fire_floydwarshall_result *result, size_t source_index, size_t target_index, size_t **out_path, size_t *out_length);

void fire_graph_print_path_labels(const fire_graph *graph, const size_t *path, size_t length);
const char *fire_graph_vertex_label(const fire_graph *graph, size_t index);

#endif

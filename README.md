# Fortress of Solitude - Fire Edition

A security-focused and documented refactor of the original low-level C modules.

This edition preserves the original "full examples" style from each module while:

- improving API safety
- documenting public and internal code with Doxygen
- adding module pages and usage examples

## Modules

- `fire_queue`
- `fire_stack`
- `fire_doubly_linked_list`
- `fire_narytree`
- `fire_vector`
- `arit_eval`
- `fire_graph`

## Project tree
```
```

## Generate Documentation

```bash
sudo dnf install doxygen graphviz
doxygen Doxyfile
xdg-open docs/html/index.html
```

## Build and run examples

### fire_queue
```bash
make -C fire_queue
./fire_queue/bin/test_queue
```

### fire_stack
```bash
make -C fire_stack
./fire_stack/bin/test_stack
```

### fire_doubly_linked_list
```bash
make -C fire_doubly_linked_list
./fire_doubly_linked_list/bin/test_doubly_linked
```

### fire_narytree
```bash
make -C fire_narytree
./fire_narytree/bin/test_tree
```

### fire_vector
```bash
make -C fire_vector
./fire_vector/bin_test/fire_vector_test
```

### arit_eval
```bash
make -C arit_eval
./arit_eval/bin/arit_eval "10 + 20 * 3"
```


### fire_graph
```bash
make -C fire_graph
./fire_graph/bin/test_graph
```

## fire_graph example

The `fire_graph` example builds a weighted route network from **A** to **J** and
uses **Bellman-Ford** to find the cheapest route. It includes a few
negative-weight edges, but no negative cycle, to demonstrate a classic
GPS-like shortest-path use case.


## fire_graph example

The graph example prints the exact weighted edge table, then runs Bellman-Ford and Floyd-Warshall to show the shortest route from `A` to `J`, including the sequence of edges used and a small benchmark between the two algorithms.

```bash
make -C fire_graph
./fire_graph/bin/test_graph
```


## Audit logs

Used tools valgrind and cppcheck:
```
cppcheck \
  --enable=all \
  --inconclusive \
  --std=c11 \
  --force \
  --check-level=exhaustive \
  --inline-suppr \
  --suppress=missingIncludeSystem \
  -I. \
  . 2 > log_cppcheck.txt
```

## Author

Orangewarrior

![C](https://img.shields.io/badge/language-C-blue)
![Build](https://img.shields.io/badge/build-manual-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)
# 🔥 Fortress of Solitude — Fire Edition

Low-level C implementations of classic data structures and algorithms, written for study, experimentation and long-term technical reference.

> ⚠️ This project is intentionally **low-level and explicit**, focusing on memory, pointers, and internal behavior — not abstraction or frameworks.

---

## 🧠 About

This repository is a collection of **hand-crafted data structures and algorithms in C**, originally built for learning and later refactored with:

- safer memory handling
- better modularization
- Doxygen documentation
- improved portability (Linux / BSD)

It represents foundational work in:

- Pointers and memory management
- Manual data structure implementation
- Algorithmic thinking
- Low-level debugging mindset

---

## 📦 Modules

### 🧱 Data Structures

- `fire_queue` → FIFO queue
- `fire_stack` → LIFO stack
- `fire_doubly_linked_list` → bidirectional list with positional access
- `fire_vector` → dynamic array
- `fire_narytree` → tree using child/sibling representation

---

### 🧮 Algorithms

- `arit_eval` → arithmetic expression parser + evaluator (with overflow-safe operations)

---

### 🌐 Graph Algorithms

- `fire_graph`
  - weighted directed graph
  - dynamic adjacency structure
  - Bellman-Ford (shortest path)
  - Floyd-Warshall (all-pairs shortest path)
  - negative cycle detection
  - path reconstruction

---

## 📊 Example — Shortest Path (Bellman-Ford)

```text
Graph edges:
A -> B (6)
A -> C (2)
C -> D (3)
D -> B (-2)
B -> E (1)
E -> H (2)
H -> I (1)
I -> J (-2)

Shortest route from A to J:
A -> C -> D -> B -> E -> H -> I -> J

Total cost: 5
```

---

## ⚙️ Build

Each module is independent:

```bash
make -C fire_queue
make -C fire_stack
make -C fire_doubly_linked_list
make -C fire_narytree
make -C fire_vector
make -C arit_eval
make -C fire_graph
```

---

## ▶️ Run Examples

```bash
./fire_graph/bin/test_graph
```

---

## 📚 Documentation

Generate full documentation:

```bash
sudo dnf install doxygen graphviz
doxygen Doxyfile
xdg-open docs/html/index.html
```

Includes:

- API documentation
- internal implementation notes
- module pages
- real usage examples

---

## 🎯 Design Philosophy

This project intentionally avoids:

- STL-like abstractions
- hidden allocations
- magic behavior

Instead, it focuses on:

- explicit memory ownership
- predictable behavior
- debuggability
- portability

---

## 🔐 Security Notes

- No unchecked arithmetic (arit_eval uses safe operations)
- Explicit frees in all modules
- Bounded structures (graph limits, queue safety)
- Defensive programming style
- Audit resources with Valgrind(audit Heap for leaks) and cppchecker(SAST)

---

## 📌 Why this project exists

This is not meant to be:

❌ production-ready library  
❌ optimized for performance  
❌ generic framework  

This **is**:

✅ a study of fundamentals  
✅ a reference for low-level C  
✅ proof of hands-on systems knowledge  

---

## 🧠 Future Work

- Dijkstra implementation
- graph serialization
- stress tests
- optional lock-free structures


## 📦 Project Structure
Note, the path "src/main.c" its a complete example to use each module.
```
Solitude_of_fire/
├── arit_eval
│   ├── audit_log
│   │   ├── audit_heap.txt
│   │   └── log_cppcheck.txt
│   ├── bin
│   │   └── arit_eval
│   ├── LICENSE
│   ├── Makefile
│   └── src
│       ├── arithmetic_eval.c
│       ├── arithmetic_eval.h
│       ├── AST.c
│       ├── AST.h
│       ├── global.h
│       ├── main.c
│       ├── parser.c
│       ├── parser.h
│       ├── secure_operation.c
│       └── secure_operation.h
├── docs
│   ├── arit_eval_page.dox
│   ├── fire_dll_page.dox
│   ├── fire_graph_page.dox
│   ├── fire_queue_page.dox
│   ├── fire_stack_page.dox
│   ├── fire_tree_page.dox
│   ├── fire_vector_page.dox
│   ├── html
│   └── mainpage.dox...
├── Doxyfile
├── fire_doubly_linked_list
│   ├── audit_log
│   │   ├── audit_heap.txt
│   │   └── log_cppcheck.txt
│   ├── bin
│   │   └── test_doubly_linked
│   ├── lib
│   │   ├── fire_doubly_linked.c
│   │   └── fire_doubly_linked.h
│   ├── LICENSE
│   ├── Makefile
│   └── src
│       └── main.c
├── fire_graph
│   ├── bin
│   │   └── test_graph
│   ├── lib
│   │   ├── fire_graph.c
│   │   └── fire_graph.h
│   ├── log_audit
│   │   ├── log_cppcheck.txt
│   │   └── log_valgrind.txt
│   ├── Makefile
│   └── src
│       └── main.c
├── fire_narytree
│   ├── audit_log
│   │   ├── audit_heap.txt
│   │   └── log_cppcheck.txt
│   ├── bin
│   │   └── test_tree
│   ├── lib
│   │   ├── fire_narytree.c
│   │   └── fire_narytree.h
│   ├── LICENSE
│   ├── Makefile
│   └── src
│       └── main.c
├── fire_queue
│   ├── audit_log
│   │   ├── audit_heap.txt
│   │   └── log_cppcheck.txt
│   ├── bin
│   │   └── test_queue
│   ├── lib
│   │   ├── fire_queue.c
│   │   └── fire_queue.h
│   ├── LICENSE
│   ├── Makefile
│   └── src
│       └── main.c
├── fire_stack
│   ├── audit_log
│   │   ├── audit_heap.txt
│   │   └── log_cppcheck.txt
│   ├── bin
│   │   └── test_stack
│   ├── lib
│   │   ├── fire_stack.c
│   │   └── fire_stack.h
│   ├── LICENSE
│   ├── Makefile
│   └── src
│       └── main.c
├── fire_vector
│   ├── audit_log
│   │   ├── audit_heap.txt
│   │   └── log_cppcheck.txt
│   ├── bin_test
│   │   └── fire_vector_test
│   ├── lib
│   │   ├── fire_vector.c
│   │   ├── fire_vector.h
│   │   ├── mem_ops.c
│   │   └── mem_ops.h
│   ├── LICENSE
│   ├── Makefile
│   └── src_test
│       └── main.c
├── LICENSE
└── README.md

38 directories, 321 files

```

## Generate Documentation

```bash
sudo dnf install doxygen graphviz
doxygen Doxyfile
xdg-open docs/html/index.html
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

## 🧑‍💻 Author

**Orangewarrior**

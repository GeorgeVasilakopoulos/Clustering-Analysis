# Software Development for Algorithmic Problems
## Assignment 1







### File Structure

<pre>
.
├── Makefile
├── README.md
├── src
│		 ├── approximators
│		 │		 ├── cube
│		 │		 │		 ├── main.cpp
│		 │		 │		 ├── include
│		 │		 │		 │		 ├── cube_hash.hpp
│		 │		 │		 │		 └── cube.hpp
│		 │		 │		 └── modules
│		 │		 │		     └── cube.cpp
│		 │		 └── lsh
│		 │		     ├── main.cpp
│		 │		     ├── include
│		 │		     │		 ├── lsh_hash.hpp
│		 │		     │		 └── lsh.hpp
│		 │		     └── modules
│		 │		         └── lsh.cpp
│		 ├── cluster
│		 │		 ├── config.conf
│		 │		 ├── main.cpp
│		 │		 ├── include
│		 │		 │		 ├── cluster.hpp
│		 │		 │		 └── Clustering.hpp
│		 │		 └── modules
│		 │		     ├── cluster.cpp
│		 │		     └── Clustering.cpp
│		 └── common
│		     ├── main.cpp
│		     ├── include
│		     │		 ├── Approximator.hpp
│		     │		 ├── ArgParser.hpp
│		     │		 ├── FileParser.hpp
│		     │		 ├── HashTable.hpp
│		     │		 ├── utils.hpp
│		     │		 └── Vector.hpp
│		     └── modules
│		         ├── Approximator.cpp
│		         ├── ArgParser.tcc
│		         ├── Distances.tcc
│		         ├── FileParser.cpp
│		         ├── HashTable.tcc
│		         ├── utils.cpp
│		         └── Vector.tcc
├── test_images
└── train_images
</pre>

### Data Handling

- The input data is loaded into a `DataSet` object, where each data instance is represented by a `DataPoint` object. 

- Each `DataPoint` encapsulates a unique ID and a vector of type `Vector<uint8_t>`.

- The `Vector<>` template class allows convenient manipulation of vectorized data, through constructs such as overloaded operators, type conversions and indexing. 

Related Modules: `common/modules/Vector.tcc`, `common/modules/utils.cpp`


### Approximators

The `Approximator` abstract class is a 'base' class that stores a `DataSet` reference and provides functions for

- K Nearest Neighbors `kNN( )`
- K Approximate Nearest Neighbors `kANN( )` (pure virtual)
- Range Search `RangeSearch( )` (pure virtual)

Each of these functions takes as arguments:

- A metric function (such as the `l2` metric)
- A query `DataPoint`
- Algorithm-related parameters

And returns the unique vector IDs that were found by the respective search algorithm.

Both `LSH` and `Cube` are subclasses of `Approximator` and implement `kANN( )` and `RangeSearch( )` accordingly.

#### LSH

```
$ make lsh
$ ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
```

#### Cube

#### Clustering

- The `Cluster` class stores pointers to `DataPoint` objects that are members of the cluster. It provides several functionalities that are essential for the management of a cluster:

	- Inclusion of a datapoint in the cluster, `add( )`
	- Removal of a datapoint from the cluster, `remove( )`
	- Center calculation, `update( )` 

- `Clusterer` is an abstract class, the objects of which are **handles** for clustering algorithms. Upon construction, a `Clusterer` object stores a reference to a `DataSet` object and performs the *k-means++* algorithm for the initialization of the clusters.

- `Lloyd` is a subclass of `Clusterer` that implements the classical clustering algorithm augmented with the `MacQueen` update step, through the `apply( )` function.

- `RAssignment` is also a subclass of `Clusterer`. Additionally to the common parameters, it also stores an `Approximator` object (`LSH` or `Cube`), that is used in order to accelerate the clustering process, with the tradeoff of finding *approximate* neighbors.


















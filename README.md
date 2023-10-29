# Software Development for Algorithmic Problems

## Authors
- `1115202000018`: Georgios-Alexandros Vasilakopoulos
- `1115202000154`: Georgios Nikolaou

## Description

Development of approximal searching and vector clustering algorithms in C++: 

- *k-Nearest Neighbours* and *Range Search*, using:
    - Locality Sensitive Hashing
    - Hypercube Random Projection


- **Lloyds's** clustering algorithm
- **Reverse Search** clustering, enhanced with approximal searching.


The algorithms are benchmarked on the [MNIST handwritten digit](http://yann.lecun.com/exdb/mnist/) dataset, where each 28 x 28 image is represented as a 784 - dimensional vector.

## File Structure

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

## Data Handling

- The input data is loaded into a `DataSet` object, where each data instance is represented by a `DataPoint` object. 

- Each `DataPoint` encapsulates a unique ID and a vector of type `Vector<uint8_t>`.

- The `Vector<>` template class allows convenient manipulation of vectorized data, through constructs such as overloaded operators, type conversions and indexing. 

Related Modules: `common/modules/Vector.tcc`, `common/modules/utils.cpp`


## Approximators

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

### LSH

```
$ make lsh
$ ./lsh –d <input file> –q <query file> –k <int> -L <int> -ο <output file> -Ν <number of nearest> -R <radius>
```

The `LshAmplifiedHash` class implements the `LSH` algorithm. It encapsulates multiple `LshHash` object, each of which maps a vector to a *random hyperplane*, defined by a random vector `v` and a random scalar `t`. When applied to a vector `p`, it produces a randomly weighted linear combination of the `LshHash` outputs, applied on `p`.

The `LSH` class contains several hashtables, each defined by a unique `LshAmplifiedHash`. Each hashtable is populated with the entire dataset. When applying a search algorithm for some query, the *candidate neigbours* are those that are contained in the hashtable buckets that the query would be placed in. 

### Cube

```
$ make cube
$ ./cube –d <input file> –q <query file> –k <int> -M <int> -probes <int> -ο <output file> -Ν <number of nearest> -R <radius>
```

The `CubeHash` class implements the *Hypercube Projection* algorithm. It encapsulates multiple `LshHash` objects. When applied to a vector `p`, it produces a random projection into binary vector that corresponds to a hypercube vertex. 

The `Cube` class contains a single hashtable, defined by a unique `CubeHash` and populated with the entire dataset. The number of buckets is equal the number of vertices of the *k*-dimensional hypercube (*2^k*). When applying a search algorithm for some query, the *candidate neigbours* are searched in hypercube vertices of ascending hamming distance in relation to the vertex that the query would be placed in. 



## Clustering

```
$ make cluster
$ ./cluster –i <input file> –c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>
```

- The `Cluster` class stores pointers to `DataPoint` objects that are members of the cluster. It provides several functionalities that are essential for the management of a cluster:

	- Inclusion of a datapoint in the cluster, `add( )`
	- Removal of a datapoint from the cluster, `remove( )`
	- Center calculation, `update( )` 

- `Clusterer` is an abstract class, the objects of which are **handles** for clustering algorithms. Upon construction, a `Clusterer` object stores a reference to a `DataSet` object and performs the *k-means++* algorithm for the initialization of the clusters.

- `Lloyd` is a subclass of `Clusterer` that implements the classical clustering algorithm augmented with the *MacQueen* update step, through the `apply( )` function.

- `RAssignment` is also a subclass of `Clusterer`. Additionally to the common parameters, it also stores an `Approximator` object (`LSH` or `Cube`), that is used in order to accelerate the clustering process, with the tradeoff of finding *approximate* neighbors.


## Evaluation

### Approximate K-NN

We evaluate the performance of both approximate K-NN algorithms (`LSH` and `Cube`) with the following two metrics:
- **Relative Time Perfomance**: Execution time of the approximal algorthm (`LSH` or `Cube`), divided by the execution time of the *naive* algortihm. Closer to 0 is better.
- **Approximation Factor**: The average distance between approximate and true nearest neighbours. Closer to 1 is better.

Each algorithm is executed 10 times and the values reported are an average of these executions.

In both cases, the results prove that the algorithms approximate the nearest neighbours exceptionally well, while taking a fraction of the time. 

It is worth noting that the appropriate selection of hyper-parameters is essential to achieving good performance.

#### LSH Parameters and Performance

- **Relative Time Perfomance**: 0.2486
- **Approximation Factor**: 1.0418

Using parameters: `–k 4 -L 5 -Ν 10`,&nbsp; and `table size = n / 8`, &nbsp; `window = 2600` 

#### Cube Parameters and Performance

- **Relative Time Perfomance**: 0.1204
- **Approximation Factor**: 1.1093

Using parameters: `–k 7 -M 6000 -probes 10 -Ν 10`,&nbsp; and `window = 2600` 

### Clustering

The clustering performance is evaluated using the [Silhouette coefficient](https://en.wikipedia.org/wiki/Silhouette_(clustering)), as well as the total clustering time. The results prove that the clustering algorithms using approximal methods produce results significantly faster than the traditional approach, while maintaining accuracy.

In all cases, we search for 10 clusters, one for each digit.

For Reverse Assignment we use the parameters mentioned in the previous section.

#### Lloyd's Algorithm
- Clustering time: `33.760`
- Silhouette for each cluster: `[0.118, 0.137, 0.156, 0.369, 0.191, 0.230, 0.100, 0.283, 0.179, 0.099]`
- Silhouette coefficient: `0.179`


#### Reverse Assignment using LSH
- Clustering time: `4.166`
- Silhouette for each cluster: `[0.088, 0.149, 0.045, 0.347, 0.017, 0.068, 0.069, 0.074, 0.105, 0.068]`
- Silhouette coefficient: `0.099`


#### Reverse Assignment using Cube
- Clustering time: `1.345`
- Silhouette for each cluster: `[0.037, 0.139, 0.075, 0.196, 0.107, 0.213, 0.084, 0.059, 0.091, 0.127]`
- Silhouette coefficient: `0.127`







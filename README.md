# Software Development for Algorithmic Problems
## Assignment 1







### File Structure

<pre>
.
├── cluster.conf
├── Makefile
├── out.txt
├── README.md
├── src
│		 ├── approximators
│		 │		 ├── cube
│		 │		 │		 ├── include
│		 │		 │		 │		 ├── cube_hash.hpp
│		 │		 │		 │		 └── cube.hpp
│		 │		 │		 ├── main.cpp
│		 │		 │		 └── modules
│		 │		 │		     └── cube.cpp
│		 │		 └── lsh
│		 │		     ├── include
│		 │		     │		 ├── lsh_hash.hpp
│		 │		     │		 └── lsh.hpp
│		 │		     ├── main.cpp
│		 │		     └── modules
│		 │		         └── lsh.cpp
│		 ├── cluster
│		 │		 ├── config.conf
│		 │		 ├── include
│		 │		 │		 ├── cluster.hpp
│		 │		 │		 └── Clustering.hpp
│		 │		 ├── main.cpp
│		 │		 └── modules
│		 │		     ├── cluster.cpp
│		 │		     └── Clustering.cpp
│		 └── common
│		     ├── include
│		     │		 ├── Approximator.hpp
│		     │		 ├── ArgParser.hpp
│		     │		 ├── FileParser.hpp
│		     │		 ├── HashTable.hpp
│		     │		 ├── utils.hpp
│		     │		 └── Vector.hpp
│		     ├── main.cpp
│		     ├── metrics.cpp
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







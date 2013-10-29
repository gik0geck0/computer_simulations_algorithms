computer_simulations_algorithms
===============================

# Introduction
This repository was designed to house a collection of libraries created in a
collaborative environment for the Computer Simulations course at CSM. The current
projects are listed and deseribed below.

# Lehmer Random Number generators
In the lehmer folder, there's a collection of libraries for a variety of different
languages. The C++ library has the most amount of work put into it, and is also
the most stable. It should be seen as the star pupil of these Lehmer Random
Number Generators.
### Stable. In order of stability and work put in
* C++
* C

### Unstable/In Progress/Experimental
* Python
* Ruby
* Haskell.

## Installation and Use

#### C/C++
```
cd lehmer/cpp
make
```
There is currently no shared object file for the library, so directly copying/linking
is required.

#### Ruby/Python/Haskell
Directly require_relative or import the file

## API functionality, Usage, and Examples
The imparative languages have a common-framework to them, so usage of them
should be pretty similar, but with minor language-caveats. Haskell on the otherhand,
is almost non-comparable to the imperative languages. It's immutability and
functional nature requires a different API framework, which is detailed separately.

#### C++/C/Python/Ruby provided functions
Documentation not done yet

#### Haskell Exported Functions
Experimental library. No documentation available

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>
#include <assert.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// random number generation based on current time
// For testing set this to 0

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
uniform_real_distribution<float> distribution(0.0, 1.0);

// float threshold(int n, int dim)
//    Determines what the cutoff should be to prune edges in our graph
//    Is based on n (the number of nodes) and dim (the number of dimensions).
float threshold(int n, int dim)
{
  if (dim == 0){
    if (n > 1000) {
      return pow(6.994 * n, -0.418);
    }
  }
  else if (dim == 2){

    if (n > 1000)
    {
      return pow(1.131 * n, -0.391);
    }
  }
  else if (dim == 3)
  {
    if (n > 1000)
    {
      return pow(1.827 * n, -0.25);
    }
  }
  else if (dim == 4)
  {
    if (n > 2000)
    {
      return pow(1.9 * n, -0.2);
    }
  }
  else {
    return -1;
  }
  return 1;
};

// class BinaryHeap
//    Creates the binary heap used for our prim's algorithm.
//    The key at each node is less than or equal to the keys of its children.
//    Uses a vector to store the heap.
//    Supports operations like insertion, extraction of the minimum element, 
//    and checking emptiness.
class BinaryHeap
{
private:
  vector<tuple<float, int>> heap;
  int capacity;
  int size = 0;

public:
  // initialize empty heap
  BinaryHeap() : capacity(0), size(0) {}

  // check if heap is empty
  bool empty() const
  {
    return size == 0;
  }

  // insert an element into the heap.
  void insert(const tuple<float, int> &val)
  {
    heap.push_back(val);
    size++;
    int i = size - 1;
    while (i > 0 && (get<0>(heap[parent(i)])) > (get<0>(heap[i])))
    {
      swap(heap[i], heap[parent(i)]);
      i = parent(i);
    }
  }

  // get the minimum element in the heap without removing it
  tuple<float, int> &peek()
  {
    return heap[0];
  }

  // extract the minimum element from the heap
  tuple<float, int> extractMin()
  {
    if (size == 0)
    {
      cout << "Heap is empty" << endl;
      throw runtime_error("Heap is empty");
    }
    tuple<float, int> root = heap[0];
    heap.pop_back();
    heap[0] = heap[size - 1];
    size--;
    minHeapify(0);
    return root;
  }

  // debugging function, print the elements of the heap.
  void printHeap()
  {
    for (int i = 0; i < size + 1; i++)
    {
      cout << get<0>(heap[i]) << " ";
    }
    cout << endl;
  }

private:
  // get the index of the parent of a node.
  int parent(int i) const
  {
    return (i - 1) / 2;
  }

  // perform min-heapify operation starting from a given index.
  void minHeapify(int i)
  {
    int l = leftChild(i);
    int r = rightChild(i);
    int smallest = i;
    if (l < size && get<0>(heap[l]) < get<0>(heap[i]))
    {
      smallest = l;
    }
    if (r < size && get<0>(heap[r]) < get<0>(heap[smallest]))
    {
      smallest = r;
    }
    if (smallest != i)
    {
      swap(heap[i], heap[smallest]);
      minHeapify(smallest);
    }
  }

  // get the index of the left child of a node
  int leftChild(int i) const
  {
    return 2 * i + 1;
  }

  // get the index of the right child of a node
  int rightChild(int i) const
  {
    return 2 * i + 2;
  }
};

// float pow(float x, int y)
//     Used to speed up the exponentiation in our calculations
float pow(float x, int y)
{
  float result = 1;
  for (int i = 0; i < y; i++)
  {
    result *= x;
  }
  return result;
}

// vector<vector<pair<float, int>>> make_graph(int n, int dim)
//    generates a graph represented as an adjacency list based on nodes and dim
vector<vector<pair<float, int>>> make_graph(int n, int dim)
{
  // pair <int, float> is an edge
  // vector<pair<int, float>> is a list of edges
  // vector<vector<pair<int, float>>> stores an edge list for each node
  vector<vector<pair<float, int>>> adj;
  adj.resize(n);

  // holds the dimensions in an array of correct size
  vector<vector<float>> coordinates(n, vector<float>(dim));
  float cutoff = threshold(n, dim); // calculate threshold

  if (dim == 0)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        float w = distribution(rng);
        if (w > cutoff)
        {
          continue; // skip edges greater than threshold
        }
        adj[i].push_back(make_pair(w, j));
        adj[j].push_back(make_pair(w, i));
      }
    }
  }
  else if (dim == 2)
  {
    for (int i = 0; i < n; i++)
    {
      coordinates[i][0] = distribution(rng);
      coordinates[i][1] = distribution(rng);
    }
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        float w = sqrt(pow(coordinates[i][0] - coordinates[j][0], 2) + pow(coordinates[i][1] - coordinates[j][1], 2));
        if (w > cutoff)
        {
          continue; // skip edges greater than threshold
        }
        adj[i].push_back(make_pair(w, j));
        adj[j].push_back(make_pair(w, i));
      }
    }
    return adj;
  }
  else if (dim == 3)
  {
    for (int i = 0; i < n; i++)
    {
      coordinates[i][0] = distribution(rng);
      coordinates[i][1] = distribution(rng);
      coordinates[i][2] = distribution(rng);
    }
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        float w = sqrt(pow(coordinates[i][0] - coordinates[j][0], 2) + pow(coordinates[i][1] - coordinates[j][1], 2) + pow(coordinates[i][2] - coordinates[j][2], 2));
        if (w > cutoff)
        {
          continue; // skip edges greater than threshold
        }
        adj[i].push_back(make_pair(w, j));
        adj[j].push_back(make_pair(w, i));
      }
    }
    return adj;
  }
  else if (dim == 4)
  {
    for (int i = 0; i < n; i++)
    {
      coordinates[i][0] = distribution(rng);
      coordinates[i][1] = distribution(rng);
      coordinates[i][2] = distribution(rng);
      coordinates[i][3] = distribution(rng);
    }
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        float w = sqrt(pow(coordinates[i][0] - coordinates[j][0], 2) + pow(coordinates[i][1] - coordinates[j][1], 2) + pow(coordinates[i][2] - coordinates[j][2], 2) + pow(coordinates[i][3] - coordinates[j][3], 2));
        if (w > cutoff)
        {
          continue; // skip edges greater than threshold
        }
        adj[i].push_back(make_pair(w, j));
        adj[j].push_back(make_pair(w, i));
      }
    }
  }
  return adj;
}

// double prim(vector<vector<pair<float, int>>> adj, int n)
//    finds the minimum spanning tree weight of a graph using Prim's algorithm
double prim(vector<vector<pair<float, int>>> adj, int n, int counter = 0)
{
  BinaryHeap heap;
  vector<float> dist(n, 1e9);
  vector<bool> visited(n, false);
  double totalWeight = 0.0;
  visited[0] = true;
  dist[0] = 0;

  for (const auto &edge : adj[0])
  {
    heap.insert(edge);
    int new_to = get<1>(edge);
    float new_weight = get<0>(edge);
    dist[new_to] = new_weight;
  }

  while (!heap.empty())
  {
    auto minEdge = heap.extractMin();
    float minWeight = get<0>(minEdge);
    int to = get<1>(minEdge);

    if (visited[to])
      continue;
    // include the edge in MST
    totalWeight += minWeight;
    counter++;
    visited[to] = true;
    // add adjacent vertices to the priority queue
    for (const auto &edge : adj[to])
    {
      int new_to = get<1>(edge);
      float new_weight = get<0>(edge);
      if (dist[new_to] > new_weight && !visited[new_to])
      {
        dist[new_to] = new_weight;
        heap.insert(edge);
      }
    }
  }
  if (counter < n - 1){
    totalWeight = prim(adj, n);
  }
  return totalWeight;
}

int main(int argc, char **argv)
{
  // stoi is string to int
  int test = stoi(argv[1]);

  int n = stoi(argv[2]);
  int trials = stoi(argv[3]);
  int dim = stoi(argv[4]);

  float sum = 0.;
  float cutoff = 0.1;

  // code that was used to print the result for each dimension

  // if (test == 1)
  // {
  //   int arr[1] = {262144};
  //   int dims[4] = {0, 2, 3, 4};
  //   for (int i = 0; i < 10; i++)
  //   {
  //     for (int j = 0; j < 4; j++)
  //     {
  //       auto start = high_resolution_clock::now();
  //       sum = 0;
  //       cout << "Nodes: " << arr[i] << " Dim: " << dims[j] << " ";
  //       for (int k = 0; k < trials; k++)
  //       {
  //         vector<vector<pair<float, int>>> adj = make_graph(arr[i], dims[j]);
  //         sum += prim(adj, arr[i]);
  //       }
  //       float average = sum / trials;
  //       auto end = high_resolution_clock::now();
  //       auto duration = duration_cast<milliseconds>(end - start);

  //       cout << "MST: " << average << " Time: " << duration.count() / 1000.0 << endl;
  //     }
  //   }
  //   return 0;
  // }
  for (int i = 0; i < trials; i++)
  {

    vector<vector<pair<float, int>>> adj = make_graph(n, dim);
    // fun the algorithm
    sum += prim(adj, n);
  }
  float average = sum / trials;
  cout << average << " " << n << " " << trials << " " << dim << endl;
  return 0;
}
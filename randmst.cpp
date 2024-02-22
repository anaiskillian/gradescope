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
// distribution(rng) returns a random number between 0 and 1

float threshold(int n, int dim)
{
  // Calculate the threshold based on n and dim
  float thres = 1.0;

  if (dim == 0){
    if (n < 1000){
      return pow(1.351 * n, -0.692);
    }
    else {
      return pow(5.294 * n, -0.918);
    }
  }
  else if (dim == 2){
    if (n < 1000)
    {
      return pow(6.839 * n, -0.701);
    }
    else
    {
      return pow(1.031 * n, -0.401);
    }
  }
  else if (dim == 3)
  {
    if (n < 1000)
    {
      return pow(1.228 * n, -0.296);
    }
    else
    {
      return pow(1.827 * n, -0.352);
    }
  }
  else if (dim == 4)
  {
    if (n < 1000)
    {
      return pow(1.45 * n, -0.246);
    }
    else
    {
      return pow(1.7 * n, -0.22);
    }
  }
  else {
    return -1;
  }
};

class BinaryHeap
{
private:
  vector<tuple<float, int>> heap;
  int capacity;
  int size = 0;

public:
  BinaryHeap() : capacity(0), size(0) {}

  bool empty() const
  {
    return size == 0;
  }

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

  tuple<float, int> &peek()
  {
    return heap[0];
  }

  tuple<float, int> extractMin()
  {
    if (size == 0)
    {
      cout << "Heap is empty" << endl;
      // Return a default value or throw an exception
      throw runtime_error("Heap is empty");
    }
    tuple<float, int> root = heap[0];
    heap.pop_back();
    heap[0] = heap[size - 1];
    size--;
    minHeapify(0);

    return root;
  }

  void printHeap()
  {
    for (int i = 0; i < size + 1; i++)
    {
      cout << get<0>(heap[i]) << " ";
    }
    cout << endl;
  }

private:
  int parent(int i) const
  {
    return (i - 1) / 2;
  }

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

  int leftChild(int i) const
  {
    return 2 * i + 1;
  }

  int rightChild(int i) const
  {
    return 2 * i + 2;
  }
};

vector<vector<pair<float, int>>> make_graph(int n, int dim)
{
  // pair <int, float> is an edge
  // vector<pair<int, float>> is a list of edges
  // vector<vector<pair<int, float>>> stores an edge list for each node
  vector<vector<pair<float, int>>> adj;
  adj.resize(n);

  // holds the dimensions in an array of correct size
  vector<vector<float>> coordinates(n, vector<float>(dim));
  float cutoff = threshold(n, dim); // Calculate threshold

  if (dim == 0)
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        float w = distribution(rng);
        if (w > cutoff)
        {
          continue; // Skip edges greater than threshold
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
          continue; // Skip edges greater than threshold
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
          continue; // Skip edges greater than threshold
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
          continue; // Skip edges greater than threshold
        }
        adj[i].push_back(make_pair(w, j));
        adj[j].push_back(make_pair(w, i));
      }
    }
  }
  return adj;
}

double prim(vector<vector<pair<float, int>>> adj, int n)
{
  BinaryHeap heap;
  vector<float> dist(n, 1e9);
  vector<bool> visited(n, false);
  double totalWeight = 0.0;
  int counter = 0;
  //  Start from vertex 0

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
    // Include the edge in MST
    totalWeight += minWeight;

    visited[to] = true;

    // Add adjacent vertices to the priority queue
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

  return totalWeight;
}

int main(int argc, char **argv)
{
  // stoi is string to int

  int n = stoi(argv[2]);
  int trials = stoi(argv[3]);
  int dim = stoi(argv[4]);

  float sum = 0.;
  float cutoff = 0.1;

  float largestEdge = 0.0; // Variable to store the largest edge

  for (int i = 0; i < trials; i++)
  {
    vector<vector<pair<float, int>>> adj = make_graph(n, dim);
    // Run the algorithm
    sum += prim(adj, n);  }
  float average = sum / trials;

  cout << average << " " << n << " " << trials << " " << dim << endl;
  // cout << "Largest edge found: " << largestEdge << endl; // Print the largest edge
  return 0;
}
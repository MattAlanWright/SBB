#include "Dataset.hpp"

// Effolkronium random library
#include "random.hpp"
using Random = effolkronium::random_static;

Dataset::Dataset(std::vector< std::vector<float> > &X,
                 std::vector<int>                  &y,
                 int   num_classes,
                 int   num_features,
                 int   num_samples)
    : num_classes(num_classes),
      num_features(num_features)
{
    dataset.reserve(num_samples);

    for(int i = 0; i < num_samples; i++) {
        // y[i] = Integer class label
        // X[i] = std::vector<float> of featurs
        Point p(X[i], y[i]);

        dataset[y[i]].push_back(p);
    }
}

Point Dataset::getRandomExemplar() {

    // Get random class
    int c = -1;
    while(c == -1) {
        // Get a random class
        int random_class_index = Random::get<int>(0, num_classes);

        // Check to see if any Points under that class remain in the dataset.
        // If so, that's our class: assign it to c.
        if( dataset[random_class_index].size() > 0 ) {
            c = random_class_index;
        }
    }

    // Get a random exemplar of that class
    int random_point_index = Random::get<int>(0, dataset[c].size());

    Point p = dataset[c][random_point_index];

    // Remove the Point from the dataset
    dataset[c].erase(dataset.begin() + random_point_index);

    return p;
}


void Dataset::insertPoint(Point p) {

    dataset[p.y].push_back(p);

}

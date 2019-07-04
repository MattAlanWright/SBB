#include <iostream>

#include "Environment.hpp"
#include "iris.hpp"


int main() {

    IrisDataset * iris = new IrisDataset();

    ClassificationEnvironment env = ClassificationEnvironment(
        iris->num_classes,
        iris->num_features,
        iris->num_training_samples,
        100,
        70,
        iris->num_training_samples,
        iris->train_X,
        iris->train_y,
        iris->test_X,
        iris->test_y);

    delete iris;

    env.train(500);

    return 0;
}

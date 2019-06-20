#ifndef __IRIS_HPP
#define __IRIS_HPP

#include "csv.h"

// shuffle algorithm example
#include <iostream>     // std::cout
#include <algorithm>    // std::shuffle
#include <vector>       // std::vector
#include <array>
#include <random>       // std::default_random_engine
#include <string>

const std::string setosa     = "Iris-setosa";
const std::string versicolor = "Iris-versicolor";
const std::string virginia   = "Iris-virginica";

enum IrisClasses {
    SETOSA,
    VERSICOLOR,
    VIRGINIA
};


class IrisDataset {

public:
    const int num_training_samples = 112;
    const int num_test_samples     = 38;
    const int num_samples          = 150;
    const int num_classes          = 3;
    const int num_features         = 4;

    std::vector< std::vector<float> > train_X;
    std::vector<int>                  train_y;

    std::vector< std::vector<float> > test_X;
    std::vector<int>                  test_y;

    IrisDataset() {

        // Fields
        float sepal_length;
        float sepal_width;
        float petal_length;
        float petal_width;
        std::string iris_class;

        std::vector< std::vector<float> > data;
        std::vector<int>                  labels;

        io::CSVReader<5> in("iris.data");


        while( in.read_row(sepal_length,
                           sepal_width,
                           petal_length,
                           petal_width,
                           iris_class) )
        {
            data.push_back({
                sepal_length,
                sepal_width,
                petal_length,
                petal_width
            });

            if( iris_class == setosa ) {
                labels.push_back(SETOSA);
            } else if (iris_class == versicolor ) {
                labels.push_back(VERSICOLOR);
            } else {
                labels.push_back(VIRGINIA);
            }
        }

        // Shuffle data
        std::default_random_engine e;
        e.seed(10);
        std::shuffle(std::begin(data), std::end(data), e);
        e.seed(10);
        std::shuffle(std::begin(labels), std::end(labels), e);

        // Read values directly into raw float arrays
        train_X.insert(train_X.begin(), data.begin(), data.begin() + num_training_samples);
        train_y.insert(train_y.begin(), labels.begin(), labels.begin() + num_training_samples);

        test_X.insert(test_X.begin(), data.begin() + num_training_samples, data.end());
        test_y.insert(test_y.begin(), labels.begin() + num_training_samples, labels.end());
    }
};

#endif //__IRIS_HPP

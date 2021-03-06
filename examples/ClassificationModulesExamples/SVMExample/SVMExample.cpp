/*
 GRT MIT License
 Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 GRT SVM Example
 his examples demonstrates how to initialize, train, and use the SVM algorithm for classification. 
 
 The Support Vector Machine (SVM) classifier is a powerful classifier that works well on a wide range of classification problems, even problems in high dimensions and that are not linearly separable. 
 
 In this example we create an instance of a SVM algorithm and then train the algorithm using some pre-recorded training data.
 The trained SVM algorithm is then used to predict the class label of some test data.
 
 This example shows you how to:
 - Create an initialize the SVM algorithm using a LINEAR kernel
 - Load some LabelledClassificationData from a file and partition the training data into a training dataset and a test dataset
 - Train the SVM algorithm using the training dataset
 - Test the SVM algorithm using the test dataset
 - Manually compute the accuracy of the classifier
*/

//You might need to set the specific path of the GRT header relative to your project
#include "GRT.h"
using namespace GRT;

int main (int argc, const char * argv[])
{
    //Create a new SVM classifier with a linear kernel
    //Other kernel options you could choose are: POLY_KERNEL, RBF_KERNEL, SIGMOID_KERNEL, PRECOMPUTED_KERNEL
    SVM svm(SVM::LINEAR_KERNEL);
    
    //The SVM will typically work much better if we scale the training and prediction data, so turn scaling on
    svm.enableScaling( true );
    
    //Train the classifier with some training data
    ClassificationData trainingData;
    
    if( !trainingData.loadDatasetFromFile("SVMTrainingData.txt") ){
        cout << "Failed to load training data!\n";
        return EXIT_FAILURE;
    }
    
    //Use 20% of the training dataset to create a test dataset
    ClassificationData testData = trainingData.partition( 80 );
    
    //Train the classifier
    bool trainSuccess = svm.train( trainingData );
    
    if( !trainSuccess ){
        cout << "Failed to train classifier!\n";
        return EXIT_FAILURE;
    }
    
    //Save the svm model to a file
    bool saveSuccess = svm.saveModelToFile("SVMModel.txt");
    
    if( !saveSuccess ){
        cout << "Failed to save the classifier model!\n";
        return EXIT_FAILURE;
    }
    
    //Load the knn model from a file
    bool loadSuccess = svm.loadModelFromFile("SVMModel.txt");
    
    if( !loadSuccess ){
        cout << "Failed to load the classifier model!\n";
        return EXIT_FAILURE;
    }
    
    //Use the test dataset to test the SVM model
    double accuracy = 0;
    for(UINT i=0; i<testData.getNumSamples(); i++){
        //Get the i'th test sample
        UINT classLabel = testData[i].getClassLabel();
        vector< double > inputVector = testData[i].getSample();
        
        //Perform a prediction using the classifier
        bool predictSuccess = svm.predict( inputVector );
        
        if( !predictSuccess ){
            cout << "Failed to perform prediction for test sampel: " << i <<"\n";
            return EXIT_FAILURE;
        }
        
        //Get the predicted class label
        UINT predictedClassLabel = svm.getPredictedClassLabel();
        vector< double > classLikelihoods = svm.getClassLikelihoods();
        vector< double > classDistances = svm.getClassDistances();
        
        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
        
        cout << "TestSample: " << i <<  " ClassLabel: " << classLabel << " PredictedClassLabel: " << predictedClassLabel << endl;
    }
    
    cout << "Test Accuracy: " << accuracy/double(testData.getNumSamples())*100.0 << "%" << endl;
    
    return EXIT_SUCCESS;
}